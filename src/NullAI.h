/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef NULLAI_H
#define NULLAI_H

#include "AI.h"


class NullAI : public AI {
	public:
		NullAI();
		virtual const std::vector< Move > gen_move(const GamePlayer& player);
};

#endif // NULLAI_H
