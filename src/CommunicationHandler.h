/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef COMMUNICATIONHANDLER_H
#define COMMUNICATIONHANDLER_H
#include <cstdio>
#include "Singleton.h"

/**
 * @brief A Singleton class for the communication with the judge.
 * Supposedly has the main loop (maintains controll)
 **/
class CommunicationHandler : public Singleton<CommunicationHandler> {
	public:
		/**
		 * @brief Initializes the infinite loop
		 **/
		void init();
};

#endif // COMMUNICATIONHANDLER_H
