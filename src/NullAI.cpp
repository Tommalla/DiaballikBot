/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "NullAI.h"
#include "CommunicationHandler.h"

const std::vector< Move > NullAI::gen_move() {
	AI::gen_move();
	
	CommunicationHandler::getInstance().printDebug("NullAI::gen_move: player = " + string({char(this->player + '0')}));
	
	return vector< Move >();
}

