/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */
#include <cstring>
#include "CommunicationHandler.h"
#include "botConstants.h"
#include "GTPParser.h"

const string CommunicationHandler::readLine() {
	static char buffer[BUFFER_SIZE];
	
	fgets(buffer, BUFFER_SIZE, stdin);
	fflush(stdin);
	
	string res = buffer;
	res.pop_back();
	
	return res;
}


void CommunicationHandler::init() {
	string command;
	
	while (666) {
		command = this->readLine();
		printf("%s", (GTPParser::getInstance().executeCommand(command)).c_str());
	}
}

void CommunicationHandler::printDebug(const string& str) {
	#ifndef NDEBUG
	fprintf(stderr, "[DEBUG]: %s\n", str.c_str());
	#endif
}


