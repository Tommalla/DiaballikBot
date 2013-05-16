/* Tomasz [Tommalla] Zakrzewski,
MIMUW, 2013 */
//DiaballikBot v0.1 AKA NullAI
#include "CommunicationHandler.h"
#include "NullAI.h"
#include "GTPParser.h"

int main() {
	NullAI ai;
	GTPParser::getInstance().setAI(&ai);
	
	CommunicationHandler::getInstance().init();	//passing control
	
	return 0;	//quitting
}