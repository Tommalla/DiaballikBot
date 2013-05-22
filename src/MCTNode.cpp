/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include <cmath>
#include "MCTNode.h"
#include "botConstants.h"

unordered_set<string>* MCTNode::gamesHistory = NULL;
int MCTNode::expansionBorder = 0;

void MCTNode::copyToSelf (const MCTNode& v) {
	this->game = v.game;
	this->isMax = v.isMax;
	this->playsQty = v.playsQty;
	this->playsWon = v.playsWon;
	this->sons = v.sons;
}

bool MCTNode::playout (int playQtyLimit) {
	//TODO recursive method which implements MCTS
}

double MCTNode::evaluate (const MCTNode* son) const {
	if (son->playsQty == 0)	//if there had been no playouts from this son
		return INF;	//we have to pick it
	
	return double(son->playsWon) / son->playsQty + ( (son->isMax) ? 1 : (-1)) *
	sqrt( log(this->playsQty) / son->playsQty);
}

MCTNode* MCTNode::chooseSon() {
	double bestEval = 0.0;
	MCTNode* res = NULL;
	
	for(pair<vector<Move>, MCTNode*> son : this->sons) {
		double eval = this->evaluate(son.second);
		if (bestEval < eval || res == NULL) {
			bestEval = eval;
			res = son.second;
		}
	}
	
	return res;
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

bool MCTNode::randomPlayout() {
	//TODO implement playout
}

const vector< Move > MCTNode::getBestMoves (int playQtyLimit, const int expansionBorder) {
	MCTNode::expansionBorder = expansionBorder;
	
	this->playout(playQtyLimit);
	
	double best = 0.0, tmp;
	vector<Move> res;
	for (pair<vector<Move>, MCTNode*> son : this->sons)
		if (son.second->playsQty != 0) {
			tmp = (double)son.second->playsWon / son.second->playsQty;
			if (best < tmp) {
				best = tmp;
				res = son.first;
			}
		}
		
	assert(res.empty() == false);
	return res;
}

MCTNode* MCTNode::forgetSon (const Game& sonGame) {
	for (pair<vector<Move>, MCTNode*> son: this->sons)
		if (son.second->getHash() == sonGame.getHash()) {
			MCTNode* tmp = son.second;
			son.second = NULL;
			return tmp;	//NOTICE: no cleanup since the 
			//method is used right before deletion of the rest of the tree
		}
		
	assert(false);
	return NULL;	//should never happen
}

MCTNode& MCTNode::operator= (const MCTNode& v) {
	this->copyToSelf(v);
	return *this;
}

MCTNode::~MCTNode() {
	for(pair<vector<Move>, MCTNode*> iter: this->sons)
		delete iter.second;
}

const string MCTNode::getHash() const {
	return this->game.getHash();
}









