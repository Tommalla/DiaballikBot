/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <cassert>
#include <cmath>
#include "MCTNode.h"
#include "botConstants.h"
#include "CommunicationHandler.h"

unordered_set<string>* MCTNode::gamesHistory = NULL;
int MCTNode::expansionBorder = 0;
vector<Move> MCTNode::movesMade;
int MCTNode::movesAvailable[2] = {2, 1};
vector<vector<Move> > MCTNode::allMovesAvailable;

void MCTNode::copyToSelf (const MCTNode& v) {
	this->game = v.game;
	this->isMax = v.isMax;
	this->playsQty = v.playsQty;
	this->playsWon = v.playsWon;
	this->sons = v.sons;
}

bool MCTNode::playout() {
	CommunicationHandler::getInstance().printDebug("MCTNode::playout()");
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

void MCTNode::calculateAvailableMovesFor (const Game& tmpGame) {
	CommunicationHandler::getInstance().printDebug("MCTNode::calculateAvailableMovesFor(...)");
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
				
					MCTNode::movesMade.push_back(Move(pawn, dst));	//add move to queue
					
					//proceed with the recursion
					MCTNode::movesAvailable[i]--;
					MCTNode::allMovesAvailable.push_back(MCTNode::movesMade);
					
					if ((MCTNode::movesAvailable[0] > 0 || MCTNode::movesAvailable[1] > 0) &&
						!tmp2.isFinished())
						this->calculateAvailableMovesFor(tmp2);
					
					MCTNode::movesAvailable[i]++;
					MCTNode::movesMade.pop_back();	//remove move from queue
				}
			}
	}
}


double MCTNode::evaluate (const MCTNode* son) const {
	CommunicationHandler::getInstance().printDebug("MCTNode::evaluate");
	if (son->playsQty == 0)	//if there had been no playouts from this son
		return INF;	//we have to pick it
	
	return double(son->playsWon) / son->playsQty + ( (son->isMax) ? 1 : (-1)) *
	sqrt( log(this->playsQty) / son->playsQty);
}

MCTNode* MCTNode::chooseSon() {
	CommunicationHandler::getInstance().printDebug("MCTNode::chooseSon");
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
	this->sons.clear();
}

MCTNode::MCTNode (const MCTNode& v) {
	this->copyToSelf(v);
}

const bool MCTNode::isLeaf() const {
	CommunicationHandler::getInstance().printDebug("MCTNode::isLeaf()");
	return this->sons.empty();
}

void MCTNode::expand(const Game& tmpGame) {
	CommunicationHandler::getInstance().printDebug("MCTNode::expand(...)");
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
					tmp2.finishMove();
					CommunicationHandler::getInstance().printDebug(tmp2.toString());
					
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
	CommunicationHandler::getInstance().printDebug("MCTNode::randomPlayout()");
	Game current = this->game;
	unordered_set<string> tempHistory = *MCTNode::gamesHistory;
	
	while (!current.isFinished() && tempHistory.find(current.getHash()) == tempHistory.end()) {
		tempHistory.insert(current.getHash());
		MCTNode::allMovesAvailable.clear();
		CommunicationHandler::getInstance().printDebug(current.toString());
		assert(!current.isFinished());
		this->calculateAvailableMovesFor(current);	//calculating all available moves
		
		printf("dupa\n");
		assert(MCTNode::allMovesAvailable.empty() == false);
		printf("WTF?!\n");
		
		int choice = rand() % MCTNode::allMovesAvailable.size();	//randomly choosing one
		for (Move move : MCTNode::allMovesAvailable[choice])	//making the move
			if (current.isFinished())
				break;
			else
				current.makeMove(move);
			
		current.finishMove();
	}
	
	if (current.isFinished())
		return current.getWinner() == this->game.getCurrentPlayer();
	
	return false;	//FIXME draw, need to fix this
}

const vector< Move > MCTNode::getBestMoves (int playQtyLimit, const int expansionBorder) {
	CommunicationHandler::getInstance().printDebug("MCTNode::getBestMoves(...)");
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
	CommunicationHandler::getInstance().printDebug("MCTNode::forgetSon(...)");
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









