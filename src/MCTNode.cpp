/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "MCTNode.h"

void MCTNode::copyToSelf (const MCTNode& v) {
	this->game = v.game;
	this->isMax = v.isMax;
	this->playsQty = v.playsQty;
	this->playsWon = v.playsWon;
	this->sons = v.sons;
}


MCTNode::MCTNode (const Game& game, const bool isMax) {
	this->isMax = isMax;
	this->game = game;
	this->playsQty = 0;
	this->playsWon = 0;
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









