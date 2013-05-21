/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <ctime>
#include "MCTSAI.h"

MCTSAI::MCTSAI() {
	srand(time(NULL));
}

const std::vector< Move, std::allocator< Move > > MCTSAI::gen_move (const GamePlayer& player) {
	vector<Move> res = AI::gen_move (player);
	
	return res;
}

void MCTSAI::play (vector< Move > moves) {
	AI::play(moves);
	//TODO remove everything except the chosen situation
}


