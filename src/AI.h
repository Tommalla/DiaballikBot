/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef AI_H
#define AI_H
#include <string>

using namespace std;
/**
 * @brief The main AI class
 **/
class AI {
	public:
		string gen_move(string args);
		string play(string args);
};

#endif // AI_H
