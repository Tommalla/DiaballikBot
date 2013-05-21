/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef MCTNODE_H
#define MCTNODE_H
#include <unordered_set>
#include "../DiaballikEngine/src/Game.h"

using namespace std;

/**
 * @brief Monte Carlo Tree Node
 **/
class MCTNode {
	private:
		Game game;
		unordered_set<MCTNode*> sons;
		int playsWon, playsQty;
		bool isMax;
		
		void copyToSelf(const MCTNode& v);
	public:
		MCTNode(const Game &game, const bool isMax = true);
		MCTNode(const MCTNode& v);
		
		/**
		 * @brief Returns true if the node is a leaf in MCT
		 **/
		const bool isLeaf() const;
		/**
		 * @brief Expands the node
		 * This method creates all possible sons for the node.
		 * Bear in mind that it's use is *VERY* expensive, both time and memory-wise.
		 **/
		void expand();
		/**
		 * @brief Performs a playout starting from this node.
		 * It implements the standard MCTS algorithm with UCB1
		 * @return True if the playout was won by the player doing the first move
		 **/
		bool playout();
		
		MCTNode& operator=(const MCTNode& v);
		~MCTNode();
};

#endif // MCTNODE_H
