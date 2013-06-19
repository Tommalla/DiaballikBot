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
		GamePlayer player;
	public:
		AI();
		virtual const vector<Move> gen_move(const GamePlayer& player);
		virtual void play(vector<Move> moves);
		virtual void undo_turn(vector<Move> moves);
};

#endif // AI_H
