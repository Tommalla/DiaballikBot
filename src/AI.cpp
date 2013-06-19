/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "AI.h"
#include "CommunicationHandler.h"
#include "../DiaballikEngine/src/functions.h"

AI::AI() {
	this->player = NONE;
	this->currentGame.newGame();
}


const vector< Move > AI::gen_move(const GamePlayer& player) {
	if (this->player == NONE)
		this->player = player;//GAME_PLAYER_A; //this->currentGame.getOppositePlayer(this->currentGame.getCurrentPlayer());
	
	if (this->currentGame.getCurrentPlayer() == NONE)
		this->currentGame.setCurrentPlayer(player);
	
	//The rest in the derived class
	return vector<Move>();
}


void AI::play(vector<Move> moves) {
	CommunicationHandler::getInstance().printDebug("AI::play");
	//determining the player
	if (this->player == NONE)
		this->player = engine::getOppositePlayer(
			engine::getPlayerFor(this->currentGame.getFieldAt(moves.begin()->from)));
		
	for (Move move: moves)
		this->currentGame.makeMove(move);
	
	this->currentGame.finishMove();
	
	CommunicationHandler::getInstance().printDebug(this->currentGame.toString());
	return;
}

void AI::undo_turn (vector< Move > moves) {
	assert(this->player != NONE);	//caus if so, then there's certainly an error/misuse of the bot
}

void AI::new_game (const vector< Point > black, const vector< Point > white, const vector< Point > ball, const GamePlayer& player) {
	this->currentGame.newGame(black, white, ball);
	this->currentGame.setCurrentPlayer(player);
}

