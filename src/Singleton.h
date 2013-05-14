/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef SINGLETON_H
#define SINGLETON_H

/**
 * @brief Singleton class
 **/
class Singleton {
	private:
		Singleton();
		Singleton ( const Singleton& other );
		Singleton& operator= ( const Singleton& other );
	public:
		static Singleton& getInstance();
};

#endif // SINGLETON_H
