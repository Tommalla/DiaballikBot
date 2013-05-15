/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */
#include <cassert>
#include <sstream>
#include "GTPParser.h"

GTPParser::GTPParser() : Singleton< GTPParser >() {
	this->ai = NULL;
	
	this->commandMap.insert({"play", GTP_PLAY});
	this->commandMap.insert({"gen_move", GTP_GEN_MOVE});
}

void GTPParser::setAI(AI* ai) {
	this->ai = ai;
}

const string GTPParser::executeCommand(const string& command) {
	//TODO: Assertions/error handling?
	assert(this->ai != NULL);
	
	vector<string> args = this->splitString(command);
	
	unordered_map<string, GTPCommand>::iterator iter = this->commandMap.find(args[0]);
	
	if (iter == this->commandMap.end())
		return "? Unknown command: " + args[0] + "\n\n";
	
	vector<Move> moves;
	
	switch (iter->second) {
		case GTP_PLAY:
			assert(args.size() <= 7);
			
			for (int i = 1; i < args.size(); i += 2)
				moves.push_back(this->convertToMove(args[i], args[i + 1]));
			
			this->ai->play(moves);
			break;
		case GTP_GEN_MOVE:
			assert(moves.size() <= 3);
				
			string res = "=";
				
			for(Move move: moves)
				res += " " + this->convertFromMove(move).first + " " + this->convertFromMove(move).second;
				
			return res;
			break;
	}
	
	return "=\n\n";
}

const vector< string > GTPParser::splitString(const string& str) const {
	vector<string> res;
	
	istringstream iss(str);
	
	do {
		string sub;
		iss >> sub;
		res.push_back(sub);
	} while (iss);
	
	return res;
}

const pair<const string&, const string&> GTPParser::convertFromMove(const Move& move) {
	assert(move.from.x >= 0 && move.from.x <= 7);
	assert(move.from.y >= 0 && move.from.y <= 7);
	assert(move.to.x >= 0 && move.to.x <= 7);
	assert(move.to.y >= 0 && move.to.y <= 7);
	
	return {string( {char('a' + move.from.x), char('0' + move.from.y)} ),	//Tricky hack - making 2-elements-long tables of chars
		string( {char('a' + move.to.x), char('0' + move.to.y)} )};
}


const Move GTPParser::convertToMove(const string& from, const string& to) {
	assert(from.length() == 2 && to.length() == 2);
	assert(from[0] >= 'a' && from[0] <= 'g');
	assert(from[1] >= '1' && from[1] <= '7');
	assert(to[0] >= 'a' && to[0] <= 'g');
	assert(to[1] >= '1' && to[1] <= '7');
	
	return Move(Point(from[0] - 'a', from[1] - '0'), 
		    Point(to[0] - 'a', to[1] - '0') );
}



