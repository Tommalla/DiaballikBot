/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "AI.h"

void AI::play(vector<Move> moves) {
	for (Move move: moves)
		this->currentGame.makeMove(move);
	return;
}
