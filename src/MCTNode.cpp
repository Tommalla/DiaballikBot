/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "MCTNode.h"

unordered_set<string>* MCTNode::gamesHistory = NULL;

void MCTNode::copyToSelf (const MCTNode& v) {
	this->game = v.game;
	this->isMax = v.isMax;
	this->playsQty = v.playsQty;
	this->playsWon = v.playsWon;
	this->sons = v.sons;
}

bool MCTNode::play (int playQtyLimit) {
	//TODO recursive method which implements MCTS
}


MCTNode::MCTNode (const Game& game, bool isMax, unordered_set<string>* gamesHistory) {
	this->isMax = isMax;
	this->game = game;
	this->playsQty = 0;
	this->playsWon = 0;
	MCTNode::gamesHistory = gamesHistory;
}

MCTNode::MCTNode (const MCTNode& v) {
	this->copyToSelf(v);
}

const bool MCTNode::isLeaf() const {
	return this->sons.empty();
}

void MCTNode::expand() {
	//TODO implement expanding
}

bool MCTNode::playout() {
	//TODO implement playout
}

const vector< Move > MCTNode::getBestMoves (int playQtyLimit, const int expansionBorder) {

	//TODO iterate through sons members and choose the best 
}

MCTNode* MCTNode::forgetSon (const Game& sonGame) {
	//TODO implement
}


MCTNode& MCTNode::operator= (const MCTNode& v) {
	this->copyToSelf(v);
	return *this;
}

MCTNode::~MCTNode() {
	for(pair<vector<Move>, MCTNode*> iter: this->sons) {
		delete iter.second;
	}
}

const string MCTNode::getHash() const {
	return this->game.getHash();
}









