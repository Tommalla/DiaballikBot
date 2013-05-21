/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef MCTSAI_H
#define MCTSAI_H

#include "AI.h"

/**
 * @brief A Monte Carlo Tree Search based AI to play Diaballik
 **/
class MCTSAI : public AI {
	public:
		virtual const std::vector< Move, std::allocator< Move > > gen_move (const GamePlayer& player);
		MCTSAI();
};

#endif // MCTSAI_H
