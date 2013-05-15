/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

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
	vector<string> args = this->splitString(command);
	
	unordered_map<string, GTPCommand>::iterator iter = this->commandMap.find(args[0]);
	
	if (iter == this->commandMap.end())
		return "? Unknown command: " + args[0] + "\n\n";
	
	switch (iter->second) {
		case GTP_PLAY:
			//TODO AI::play-specific
			break;
		case GTP_GEN_MOVE:
			//TODO AI::genMove-specific
			break;
	}
	
	
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


