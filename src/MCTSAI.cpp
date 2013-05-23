/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <ctime>
#include "MCTSAI.h"
#include "botConstants.h"
#include "CommunicationHandler.h"

void MCTSAI::updateLimits() {
	double val = (double)MCTNode::trivialWins / this->playoutsLimit;
	
	for (pair<double, int> border : expansionBorders)
		if (val >= border.first)
			this->expansionBorder = border.second;
		else break;
		
	for (pair<double, int> limit : playoutLimits)
		if (val >= limit.first)
			this->playoutsLimit = limit.second;
		else break;
}


MCTSAI::MCTSAI() {
	srand(time(NULL));
	this->MCTRoot = NULL;
	this->playoutsLimit = initialPlayoutsLimit;
	this->expansionBorder = initialExpansionBorder;
}

const std::vector< Move, std::allocator< Move > > MCTSAI::gen_move (const GamePlayer& player) {
	vector<Move> res = AI::gen_move (player);
	
	this->updateLimits();
	#ifndef NDEBUG
	fprintf(stderr, "MCTSAI: expansionBorder = %d, playoutsLimit = %d\n", this->expansionBorder, this->playoutsLimit);
	#endif
	
	if (this->MCTRoot == NULL)	//not initialized yet
		this->MCTRoot = new MCTNode(this->currentGame, true, this->currentGame.getCurrentPlayer(), &this->gamesHistory);	//a max node starting at current state
	
	return this->MCTRoot->getBestMoves(this->playoutsLimit, this->expansionBorder);
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


