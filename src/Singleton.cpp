/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include "Singleton.h"

Singleton& Singleton::getInstance()
{
	static Singleton instance;
	return instance;
}

Singleton::Singleton() {
}


