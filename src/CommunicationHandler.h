/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef COMMUNICATIONHANDLER_H
#define COMMUNICATIONHANDLER_H
#include <cstdio>
#include <string>
#include "Singleton.h"

using namespace std;

/**
 * @brief A Singleton class for the communication with the judge.
 * Supposedly has the main loop (maintains controll)
 **/
class CommunicationHandler : public Singleton<CommunicationHandler> {
	private:
		const string readLine();
	public:
		/**
		 * @brief Initializes the infinite loop
		 **/
		void init();
		void printDebug(const string &str);
};

#endif // COMMUNICATIONHANDLER_H
