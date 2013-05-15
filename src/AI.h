/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef AI_H
#define AI_H
#include <string>
#include <vector>
#include "../DiaballikEngine/src/Move.h"
#include "../DiaballikEngine/src/Game.h"

using namespace std;
/**
 * @brief The main AI class. A base class for AI's
 **/
class AI {
	protected:
		Game currentGame;
	public:
		virtual const vector<Move> gen_move() = 0;
		void play(vector<Move> moves);
};

#endif // AI_H
