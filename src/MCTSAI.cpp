/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "MCTSAI.h"

const std::vector< Move, std::allocator< Move > > MCTSAI::gen_move (const GamePlayer& player) {
	vector<Move> res = AI::gen_move (player);
	
	return res;
}

MCTSAI::MCTSAI() {

}

