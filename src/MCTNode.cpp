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
vector<Move> MCTNode::nextRandomMove;
int MCTNode::movesAvailable[2] = {2, 1};
vector<vector<Move> > MCTNode::allMovesAvailable;
GamePlayer MCTNode::desiredWinner = NONE;

#ifndef NDEBUG
int expansions = 0;
int trivialWins = 0;
int randomPlayouts = 0;
#endif

void MCTNode::copyToSelf (const MCTNode& v) {
	this->game = v.game;
	this->isMax = v.isMax;
	this->playsQty = v.playsQty;
	this->playsWon = v.playsWon;
	this->sons = v.sons;
}

int MCTNode::playout() {
	CommunicationHandler::getInstance().printDebug("MCTNode::playout()");
	int res;
	
	if (this->isLeaf()) {
		if (this->playsQty > MCTNode::expansionBorder) {
			#ifndef NDEBUG
			++expansions;
			fprintf(stderr, "Expanding...\n");
			#endif
			this->expand(this->game);
			res = this->chooseSon()->playout();
		} else
			res = this->randomPlayout();
	} else
		res = this->chooseSon()->playout();
	
	this->playsQty += gamesQtyMultiplier;
	this->playsWon += res;
	return res;
}

void MCTNode::calculateAvailableMovesFor (const Game& tmpGame) {
	//CommunicationHandler::getInstance().printDebug("MCTNode::calculateAvailableMovesFor(...)");
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

bool MCTNode::canWinInOneTurn (const Game& tmpGame, const GamePlayer player) {
	//FIXME: just a stub - notices the obvious things only and is poorly implemented
	vector<Point> pawns = tmpGame.getPawnsOf(player);
	Point holder;
	
	for (int i = 0; i < pawns.size(); ++i)
		if (tmpGame.getFieldAt(pawns[i]) == BALL_A || tmpGame.getFieldAt(pawns[i]) == BALL_B) {
			holder = pawns[i];
			swap(pawns[i], pawns[pawns.size() - 1]);
			pawns.pop_back();
			break;
		}
		
	int lineY = (player == GAME_PLAYER_A) ? 6 : 0;
	
	for (Point pawn : pawns)
		if (abs(lineY - pawn.y) <= 2) {	//can possibly reach the line
			
			switch(abs(lineY - pawn.y)) {
				case 0:
					if (tmpGame.isMoveValid(holder, pawn))
						return true;
					//TODO try to move the pawn 1-2 fields right/left and then check 
					break;
				case 1:
				{
					//first, try to move the pawn to the line in one move
					Game tmp2 = tmpGame;
					Point pawnDst = pawn + Point(0, lineY - pawn.y);
					if (tmp2.isMoveValid(pawn, pawnDst)) {
						tmp2.makeMove(pawn, pawnDst);
						if (tmp2.isMoveValid(holder, pawnDst))
							return true;
						//TODO: try moving left/right (as in case 0)
						
						tmp2 = tmpGame;
					}
					
					//try left->up/down | right->up/down
					break;
				}
				case 2:
				{
					//really no choice - either going up/down works or not
					Game tmp2 = tmpGame;
					Point pawnDst = pawn + Point(0, (lineY > pawn.y) ? 1 : -1);
					if (tmp2.isMoveValid(pawn, pawnDst)) {
						tmp2.makeMove(pawn, pawnDst);
						pawn = pawnDst;
						pawnDst = pawn + Point(0, (lineY > pawn.y) ? 1 : -1);
						
						if (tmp2.isMoveValid(pawn, pawnDst)) {
							tmp2.makeMove(pawn, pawnDst);
							if (tmp2.isMoveValid(holder, pawnDst))
								return true;
						}
					}
					break;
				}
			}
		}
		
	return false;	//everything else failed
}



double MCTNode::evaluate (const MCTNode* son) const {
	//CommunicationHandler::getInstance().printDebug("MCTNode::evaluate");
	if (son->playsQty == 0)	//if there had been no playouts from this son
		return ( (this->isMax) ? 1 : (-1)) * INF;	//we have to pick it
	
	#ifndef NDEBUG
	//printf("Eval for some node: %lf\n", double(son->playsWon) / son->playsQty + ( (this->isMax) ? 1 : (-1)) *
	//sqrt( log(this->playsQty) / son->playsQty));
	#endif
	
	return double(son->playsWon) / son->playsQty + ( (this->isMax) ? 1 : (-1)) *
	sqrt( log(this->playsQty) / son->playsQty);
}

MCTNode* MCTNode::chooseSon() {
	CommunicationHandler::getInstance().printDebug("MCTNode::chooseSon");
	double bestEval = 0.0;
	MCTNode* res = NULL;
	
	for(pair<vector<Move>, MCTNode*> son : this->sons) {
		double eval = this->evaluate(son.second);
		if ( ((this->isMax) ? bestEval < eval : bestEval > eval) || res == NULL) {
			bestEval = eval;
			res = son.second;
		}
	}
	#ifndef NDEBUG
	fprintf(stderr, "Chose: %lf\n", bestEval);
	#endif
	assert(res != NULL);
	
	return res;
}

void MCTNode::generateRandomMove (Game tmpGame, int depthLeft) {
	if (depthLeft == 0)
		return;
	//CommunicationHandler::getInstance().printDebug("MCTNode::generateRandomMove(...)");
	
	bool moveMade = false;
	bool ballPass;
	if (movesAvailable[0] > 0 && movesAvailable[1] > 0 )
		ballPass = ((rand() % 2) == 0);
	else
		ballPass = (movesAvailable[1] > 0);
	
	{
		vector<Point> pawns;
		vector<Point> destinations;
		pawns = tmpGame.getPawnsOf(tmpGame.getCurrentPlayer());	//get all pawns of current player
	
		FieldState src;
		int pawnId;
	
		if (ballPass) {
			for (pawnId = 0; pawnId < pawns.size(); ++pawnId) {
				src = tmpGame.getFieldAt(pawns[pawnId]);
				if (src == BALL_A || src == BALL_B) {
					destinations = tmpGame.getDestinationsFor(pawns[pawnId]);
					break;
				}
			}
			
		} else {	//if it's a move
			do {
				pawnId = rand() % pawns.size();
				src = tmpGame.getFieldAt(pawns[pawnId]);
			} while (src == BALL_A || src == BALL_B);
		
			destinations = tmpGame.getDestinationsFor(pawns[pawnId]);
		}
		
		if (destinations.size() > 0) {
			int choice = rand() % destinations.size();
			MCTNode::nextRandomMove.push_back(Move(pawns[pawnId], destinations[choice]));
			tmpGame.makeMove(MCTNode::nextRandomMove.back());
			moveMade = true;
			MCTNode::movesAvailable[(ballPass) ? 1 : 0]--;
			depthLeft--;
		} else if (ballPass)	//can't do a pass (blocked)
				return;

	}
	
	if (depthLeft > 0)
		this->generateRandomMove(tmpGame, depthLeft);
	
	if (moveMade) {
		movesAvailable[0] += (ballPass == false) ? 1 : 0;
		movesAvailable[1] += (ballPass == true) ? 1 : 0;
	}
	
	return;
}


MCTNode::MCTNode (const Game& game, bool isMax, GamePlayer desiredWinner, unordered_set<string>* gamesHistory) {
	this->isMax = isMax;
	this->game = game;
	this->playsQty = 0;
	this->playsWon = 0;
	MCTNode::desiredWinner = desiredWinner;
	MCTNode::gamesHistory = gamesHistory;
	this->sons.clear();
	this->leaf = true;
}

MCTNode::MCTNode (const MCTNode& v) {
	this->copyToSelf(v);
}

const bool MCTNode::isLeaf() const {
	//CommunicationHandler::getInstance().printDebug("MCTNode::isLeaf()");
	return this->leaf;
}

void MCTNode::expand(const Game& tmpGame) {
	//CommunicationHandler::getInstance().printDebug("MCTNode::expand(...)");
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
					assert(tmp2.getCurrentPlayer() != tmpGame.getCurrentPlayer());
					
					//FIXME possible crash?
					if (!tmp2.isFinished() && this->canWinInOneTurn(tmp2, tmp2.getCurrentPlayer()))
						continue;	//we don't want obviously losing states
					
					//CommunicationHandler::getInstance().printDebug(tmp2.toString());
					
					size_t h = hash<string>()(tmp2.getHash());
					
					if (this->sonsGameHashes.find(h) == this->sonsGameHashes.end()) {	//this hash is not present
						this->sonsGameHashes.insert(h);
						MCTNode::movesMade.push_back(Move(pawn, dst));	//add move to queue
						
						this->sons.push_back(make_pair(MCTNode::movesMade, 
									       new MCTNode(tmp2, !this->isMax)));	//add node
						
						tmp2.finishMove();
						
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
	this->leaf = false;
}

int MCTNode::randomPlayout() {
	#ifndef NDEBUG
	++randomPlayouts;
	#endif
	CommunicationHandler::getInstance().printDebug("MCTNode::randomPlayout()");
	Game current = this->game;
	unordered_set<string> tempHistory = *MCTNode::gamesHistory;
	
	while (!current.isFinished() && tempHistory.find(current.getHash()) == tempHistory.end()) {
		//checking for trivial winning
		if (this->canWinInOneTurn(current, current.getCurrentPlayer())) {
			//printf("Trivial win!");
			#ifndef NDEBUG
			++trivialWins;
			#endif
			return (current.getCurrentPlayer() == MCTNode::desiredWinner) ? winPoints : losePoints;
		}
		
		tempHistory.insert(current.getHash());
		MCTNode::nextRandomMove.clear();
		//CommunicationHandler::getInstance().printDebug(current.toString());
		//assert(!current.isFinished());
		this->generateRandomMove(current, rand() % 4);	//calculating all available moves
		
		for (Move move : MCTNode::nextRandomMove)	//making the move
			if (current.isFinished())
				break;
			else
				current.makeMove(move);
			
		current.finishMove();

	}
	
	if (current.isFinished() && current.getWinner() != NONE) {
		#ifndef NDEBUG
		fprintf(stderr, (current.getWinner() == MCTNode::desiredWinner) ? "won\n" : "lost\n");
		#endif
		return (current.getWinner() == MCTNode::desiredWinner) ? winPoints : losePoints;
	}
	
	#ifndef NDEBUG
	fprintf(stderr, "draw\n");
	#endif
	return drawPoints;
}

const vector< Move > MCTNode::getBestMoves (int playQtyLimit, const int expansionBorder) {
	#ifndef NDEBUG
	trivialWins = expansions = randomPlayouts = 0;
	#endif
	CommunicationHandler::getInstance().printDebug("MCTNode::getBestMoves(...)");
	MCTNode::expansionBorder = expansionBorder;
	
	if (this->isLeaf() == true) {
		this->expand(this->game);
		#ifndef NDEBUG
		fprintf(stderr, "Total sons: %d\n", this->sons.size());
		#endif
	}
	
	while (playQtyLimit--)
		this->playout();
	
	if (this->canWinInOneTurn(this->game, this->game.getCurrentPlayer())) {
		#ifndef DEBUG
		fprintf(stderr, "can win in one turn!\n");
		#endif
		for (pair<vector<Move>, MCTNode*> son : this->sons) {
			Game tmp = this->game;
			for (Move move: son.first)
				tmp.makeMove(move);
			if (tmp.isFinished())
				return son.first;
		}
		
		#ifndef DEBUG
		fprintf(stderr, "can win in one turn! but failed to find THE SON\n");
		#endif
		assert(false);
	}
	
	double best = 0.0, tmp;
	vector<Move> res;
	for (pair<vector<Move>, MCTNode*> son : this->sons)
		if (son.second->playsQty != 0) {
			tmp = (double)son.second->playsWon / son.second->playsQty;
			if (tmp > best || res.empty()) {
				best = tmp;
				res = son.first;
			}
		}
	
	assert(res.empty() == false);
	#ifndef NDEBUG
	fprintf(stderr, "Chose the son of result %lf\n", best);
	fprintf(stderr, "trivialWins = %d, expansions = %d, randomPlayouts = %d\n", trivialWins, expansions, randomPlayouts);
	#endif
	return res;
}

MCTNode* MCTNode::forgetSon (const Game& sonGame) {
	CommunicationHandler::getInstance().printDebug("MCTNode::forgetSon(...)");
	for (int i = 0; i < this->sons.size(); ++i)
		if (this->sons[i].second->getHash() == sonGame.getHash()) {
			MCTNode* tmp = this->sons[i].second;
			this->sons[i].second = NULL;
			CommunicationHandler::getInstance().printDebug("The right son has been found!");
			return tmp;	//NOTICE: no cleanup since the 
			//method is used right before deletion of the rest of the tree
		}
		
	//if controll reached this moment, it means we have no nodes for the state we're going to
	return NULL;
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









