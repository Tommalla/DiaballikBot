/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef MCTSAI_H
#define MCTSAI_H

#include <unordered_set>
#include "AI.h"
#include "MCTNode.h"

using namespace std;

/**
 * @brief A Monte Carlo Tree Search based AI to play Diaballik
 **/
class MCTSAI : public AI {
	private:
		MCTNode* MCTRoot;
		unordered_set<string> gamesHistory;
		int expansionBorder;
		int playoutsLimit;
		
		/**
		 * @brief Assigns correct values to control factors like expansionBorder
		 **/
		void updateLimits();
	public:
		MCTSAI();
		
		virtual const std::vector< Move, std::allocator< Move > > gen_move (const GamePlayer& player);
		virtual void play(vector<Move> moves);
		virtual void undo_turn (vector< Move > moves);
};

#endif // MCTSAI_H
