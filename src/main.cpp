/* Tomasz [Tommalla] Zakrzewski,
MIMUW, 2013 */
//DiaballikBot v0.2 AKA MCTSAI
#include "CommunicationHandler.h"
#include "MCTSAI.h"
#include "GTPParser.h"

int main() {
	MCTSAI ai;
	GTPParser::getInstance().setAI(&ai);
	
	CommunicationHandler::getInstance().init();	//passing control
	
	return 0;	//quitting
}