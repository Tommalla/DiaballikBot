/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "AI.h"

AI::AI() {
	this->player = NONE;
	this->currentGame.newGame();
}


const vector< Move > AI::gen_move() {
	if (this->player == NONE)
		this->player = GAME_PLAYER_A; //this->currentGame.getOppositePlayer(this->currentGame.getCurrentPlayer());
	
	//The rest in the derived class
	return vector<Move>();
}


void AI::play(vector<Move> moves) {
	//determining the player
	if (this->player == NONE)
		this->player = this->currentGame.getOppositePlayer(
			this->currentGame.getPlayerFor(this->currentGame.getFieldAt(moves.begin()->from)));
		
	for (Move move: moves)
		this->currentGame.makeMove(move);
	return;
}

