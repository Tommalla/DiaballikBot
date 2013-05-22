/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <ctime>
#include "MCTSAI.h"
#include "botConstants.h"
#include "CommunicationHandler.h"

MCTSAI::MCTSAI() {
	srand(time(NULL));
	this->MCTRoot = NULL;
}

const std::vector< Move, std::allocator< Move > > MCTSAI::gen_move (const GamePlayer& player) {
	vector<Move> res = AI::gen_move (player);
	
	if (this->MCTRoot == NULL)	//not initialized yet
		this->MCTRoot = new MCTNode(this->currentGame, true, &this->gamesHistory);	//a max node starting at current state
	
	return this->MCTRoot->getBestMoves(initialPlayoutsLimit, initialExpansionBorder);
}

void MCTSAI::play (vector< Move > moves) {
	AI::play(moves);
	
	gamesHistory.insert(this->currentGame.getHash());	//adding the move to the history
	
	if (this->MCTRoot != NULL) {	//removing nodes we no longer need
		MCTNode* newRoot = this->MCTRoot->forgetSon(this->currentGame);
		delete this->MCTRoot;
		
		this->MCTRoot = newRoot;
	} else
		CommunicationHandler::getInstance().printDebug("MCTRoot is NULL! :O");
}


