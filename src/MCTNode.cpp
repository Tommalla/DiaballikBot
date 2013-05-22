/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include <cmath>
#include "MCTNode.h"
#include "botConstants.h"

unordered_set<string>* MCTNode::gamesHistory = NULL;
int MCTNode::expansionBorder = 0;
vector<Move> MCTNode::movesMade;
int MCTNode::movesAvailable[2] = {2, 1};

void MCTNode::copyToSelf (const MCTNode& v) {
	this->game = v.game;
	this->isMax = v.isMax;
	this->playsQty = v.playsQty;
	this->playsWon = v.playsWon;
	this->sons = v.sons;
}

bool MCTNode::playout() {
	bool res;
	
	if (this->isLeaf()) {
		if (this->playsQty > MCTNode::expansionBorder) {
			this->expand(this->game);
			res = this->chooseSon()->playout();
		} else
			res = this->randomPlayout();
	} else
		res = this->chooseSon()->playout();
	
	this->playsQty++;
	this->playsWon += (res) ? 1 : 0;
	return res;
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

void MCTNode::expand(const Game& tmpGame) {
	assert(!this->game.isFinished());
	
	vector<Point> pawns;
	vector<Point> destinations;
	
	pawns = tmpGame.getPawnsOf(tmpGame.getCurrentPlayer());	//get all pawns of current player
			
	for (Point pawn: pawns) {	//try to move every pawn
		FieldState src = tmpGame.getFieldAt(pawn);
		
		for (int i = 0; i < 2; ++i)	//for each pawn, try move or ball pass
			if (MCTNode::movesAvailable[i] > 0 && ( 
			(i == 0 && (src == PLAYER_A || src == PLAYER_B)) ||
			(i == 1 && (src == BALL_A || src == BALL_B)))) {	//try to make a move
				destinations = tmpGame.getDestinationsFor(pawn);
			
				for (Point dst: destinations) {	//for every destination available
					Game tmp2 = tmpGame;	//generate new game
					tmp2.makeMove(pawn, dst);	//with that move made
					
					size_t h = hash<string>()(tmp2.getHash());
					
					if (this->sonsGameHashes.find(h) == this->sonsGameHashes.end()) {	//this hash is not present
						this->sonsGameHashes.insert(h);
						MCTNode::movesMade.push_back(Move(pawn, dst));	//add move to queue
						
						this->sons.push_back(make_pair(MCTNode::movesMade, 
									       new MCTNode(tmp2, !this->isMax)));	//add node
						
						//proceed with the recursion
						MCTNode::movesAvailable[i]--;
						if (MCTNode::movesAvailable[0] > 0 || MCTNode::movesAvailable[1] > 0)
							this->expand(tmp2);
						MCTNode::movesAvailable[i]++;
						MCTNode::movesMade.pop_back();	//remove move from queue
					}
				}
			}
	}
}

bool MCTNode::randomPlayout() {
	//TODO implement playout
}

const vector< Move > MCTNode::getBestMoves (int playQtyLimit, const int expansionBorder) {
	MCTNode::expansionBorder = expansionBorder;
	
	while (playQtyLimit--)
		this->playout();
	
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









