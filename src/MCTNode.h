/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef MCTNODE_H
#define MCTNODE_H
#include "../DiaballikEngine/src/Game.h"

/**
 * @brief Monte Carlo Tree Node
 **/
class MCTNode {
	private:
		Game game;
		//TODO some container for sons, some values
	public:
		MCTNode();
		MCTNode(const MCTNode& v);
		
		MCTNode& operator=(const MCTNode& v);
		~MCTNode();
};

#endif // MCTNODE_H
