/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef GTPPARSER_H
#define GTPPARSER_H
#include <vector>
#include <string>
#include <unordered_map>
#include "AI.h"
#include "Singleton.h"
#include "botEnums.h"

/**
 * @brief A pretty static implementation (time!)
 **/
class GTPParser : public Singleton<GTPParser> {
	friend class Singleton;
	private:
		AI* ai;
		unordered_map<string, GTPCommand> commandMap;
		GTPParser();
	public:
		void setAI(AI* ai);
		const string executeCommand(const string &command);
		//TODO a namespace ?
		const vector<string> splitString(const string& str) const;
		const Move convertToMove(const string from, const string to);
		const pair<const string&, const string&> convertFromMove(const Move& move);
};

#endif // GTPPARSER_H
