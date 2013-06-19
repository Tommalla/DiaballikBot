/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */
#include <cassert>
#include <sstream>
#include "GTPParser.h"
#include "CommunicationHandler.h"
#include "../DiaballikEngine/src/functions.h"

GTPParser::GTPParser() : Singleton< GTPParser >() {
	this->ai = NULL;
	
	this->commandMap.insert({"play", GTP_PLAY});
	this->commandMap.insert({"gen_move", GTP_GEN_MOVE});
	this->commandMap.insert({"undo_turn", GTP_UNDO_TURN});
	this->commandMap.insert({"new_game", GTP_NEW_GAME});
}

void GTPParser::setAI(AI* ai) {
	this->ai = ai;
}

const string GTPParser::executeCommand(const string& command) {
	//TODO: Assertions/error handling?
	assert(this->ai != NULL);
	
	vector<string> args = engine::splitString(command);
	
	unordered_map<string, GTPCommand>::iterator iter = this->commandMap.find(args[0]);
	
	if (iter == this->commandMap.end())
		return "? Unknown command: " + args[0] + "\n\n";
	
	CommunicationHandler::getInstance().printDebug("Args:");
	for(string str: args)
		CommunicationHandler::getInstance().printDebug(str);
	
	vector<Move> moves;
	
	switch (iter->second) {
		case GTP_PLAY:
			assert(args.size() == 3);
			
			moves = engine::convertToMoves(args[2]);
			
			this->ai->play(moves);
			break;
		case GTP_GEN_MOVE:
			assert(args.size() == 2);
			
			{
				moves = this->ai->gen_move((args[1][0] == 'w') ? GAME_PLAYER_B : GAME_PLAYER_A);
				
				string res = "= ";
				
				for(Move move: moves)
					res += engine::convertFromMove(move).first + engine::convertFromMove(move).second;
				
				CommunicationHandler::getInstance().printDebug("Dupa");
				
				return res + "\n\n";
			}
			break;
		case GTP_UNDO_TURN:
			assert(args.size() == 3);
			
			moves = engine::convertToMoves(args[2]);
			
			this->ai->undo_turn(moves);
			break;
		case GTP_NEW_GAME:
		{
			vector<Point> black, white, balls = engine::convertToPoints(args[2]), 
				tmp = engine::convertToPoints(args[1]);
			int i;
			for (i = 0; i < (int)tmp.size() / 2; ++i)
				black.push_back(tmp[i]);
			for (; i < (int)tmp.size(); ++i)
				white.push_back(tmp[i]);
			
			this->ai->new_game(black, white, balls, (args[3] == "w") ? GAME_PLAYER_B : GAME_PLAYER_A);
			
			break;
		}
		default:
			return "? Unknown in-game command: " + iter->second;
	}
	
	return "=\n\n";
}

