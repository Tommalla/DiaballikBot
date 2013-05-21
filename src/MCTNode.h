/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef MCTNODE_H
#define MCTNODE_H
#include <unordered_set>
#include "../DiaballikEngine/src/Game.h"

using namespace std;

//the following hash parametrization is very HACK-ish
class MCTNode;

namespace std {
	template<>
	struct hash<pair<vector<Move>, MCTNode*> > {
		inline std::size_t operator()(const pair<vector<Move>, MCTNode*>& key) const;
	};
}

/**
 * @brief Monte Carlo Tree Node
 **/
class MCTNode {
	
	private:
		Game game;
		unordered_set<pair<vector<Move>, MCTNode*> > sons;
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
		
		const string getHash() const;
};

namespace std {
	inline std::size_t hash<pair<vector<Move>, MCTNode*> >::operator()(const pair< vector< Move >, MCTNode* >& key) const {
			return hash<string>()(key.second->getHash());
	}
}

#endif // MCTNODE_H
