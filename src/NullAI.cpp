/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include <ctime>
#include "NullAI.h"
#include "CommunicationHandler.h"

NullAI::NullAI() {
	srand(time(NULL));
}


const std::vector< Move > NullAI::gen_move(const GamePlayer& player) {
	AI::gen_move(player);
	
	CommunicationHandler::getInstance().printDebug("NullAI::gen_move: player = " + string({char(this->player + '0')}));
	
	vector< Point > pawns;
	int movesQty = (rand() % 2) + 1;
	int passesQty = rand() % 2;
	vector< Move > moves;
	//vector< Move > passes;
	vector< Point > destinations;
	
	Game tmp(this->currentGame);
	vector< Move > res;
	
	while(movesQty-- > 0) {
		moves.clear();
		pawns = tmp.getPawnsOf(this->player);
		
		for(Point pawn: pawns) {
			destinations = tmp.getDestinationsFor(pawn);
			//printf("Pawn: %d %d\nDestinations:\n", pawn.x, pawn.y);
			
			for(Point dst: destinations) {	//we promote moves forward or within the same y
				if ( ((this->player == GAME_PLAYER_A) ? dst.y >= pawn.y : dst.y <= pawn.y) ) {
					//printf("%d %d\n", dst.x, dst.y);
					if (tmp.getMoveTypeFor(Move(pawn, dst)) == MOVE)
						moves.push_back(Move(pawn, dst));
				}
			}
		}
		
		assert(!moves.empty());
		
		int choice = rand() % moves.size();
		
		//printf("Chose[%d] %d %d -> %d %d\n", movesQty, moves[choice].from.x, moves[choice].from.y, moves[choice].to.x, moves[choice].to.y);
		
		res.push_back(moves[choice]);
		tmp.makeMove(res.back());
		
		CommunicationHandler::getInstance().printDebug(tmp.toString().c_str());
		
		if(tmp.isFinished())
			return res;
	}
	
	//puts("Now at passes");
	
	while(passesQty-- > 0) {
		moves.clear();
		pawns = tmp.getPawnsOf(this->player);
		for(Point pawn: pawns) {
			destinations = tmp.getDestinationsFor(pawn);
			//printf("Pawn: %d %d\nDestinations[%d]:\n", pawn.x, pawn.y, destinations.size());
			
			for(Point dst: destinations)
				if ( ((this->player == GAME_PLAYER_A) ? dst.y >= pawn.y : dst.y <= pawn.y) ) { 	//we promote moves forward or within the same y
					//printf("%d %d\n", dst.x, dst.y);
					if (tmp.getMoveTypeFor(Move(pawn, dst)) == BALL_PASS)
						moves.push_back(Move(pawn, dst));
				}
		}
		
		if (moves.empty())
			break;
		
		int choice = rand() % moves.size();
		
		//printf("Choice: %d %d -> %d %d\n", moves[choice].from.x, moves[choice].from.y, moves[choice].to.x, moves[choice].to.y);
		
		res.push_back(moves[choice]);
		tmp.makeMove(res.back());
		
		CommunicationHandler::getInstance().printDebug(tmp.toString().c_str());
	}
	
	return res;
}

