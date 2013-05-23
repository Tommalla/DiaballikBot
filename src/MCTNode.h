/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef MCTNODE_H
#define MCTNODE_H
#include <unordered_set>
#include <set>
#include "../DiaballikEngine/src/Game.h"

using namespace std;

/**
 * @brief Monte Carlo Tree Node
 **/
class MCTNode {
	private:
		Game game;
		set<size_t> sonsGameHashes;
		vector<pair<vector<Move>, MCTNode*> > sons;
		int playsWon, playsQty;
		bool isMax;
		
		static unordered_set<string>* gamesHistory;
		static int expansionBorder;
		
		static vector<Move> movesMade;	//a stack for expand
		static int movesAvailable[2];	//movesAvailable for expand
		//first number is qty of moves, the second of passes
		static vector<vector<Move> > allMovesAvailable;	//a stack for calculateAvailableMovesFor
		static GamePlayer desiredWinner;	//us
		
		void copyToSelf(const MCTNode& v);
		bool playout();
		void calculateAvailableMovesFor(const Game& tmpGame);
		
		double evaluate(const MCTNode* son) const;
		MCTNode* chooseSon();
	public:
		MCTNode(const Game &game, bool isMax = true, GamePlayer desiredWinner = MCTNode::desiredWinner, 
			unordered_set<string>* gamesHistory = MCTNode::gamesHistory);
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
		void expand(const Game& tmpGame);
		/**
		 * @brief Performs a playout starting from this node.
		 * It implements the standard MCTS algorithm with UCB1
		 * @return True if the playout was won by the player doing the first move
		 **/
		bool randomPlayout();
		
		const vector<Move> getBestMoves(int playQtyLimit, const int expansionBorder);
		/**
		 * @brief Removes the entry from sons representing sonGame
		 * @param sonGame Game state for which the son will be removed
		 * @return the pointer to the son being deleted
		 **/
		MCTNode* forgetSon(const Game& sonGame);
		
		MCTNode& operator=(const MCTNode& v);
		~MCTNode();
		
		const string getHash() const;
};


#endif // MCTNODE_H
