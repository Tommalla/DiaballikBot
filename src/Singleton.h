/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef SINGLETON_H
#define SINGLETON_H

/**
 * @brief Singleton class template
 **/
template <class T>
class Singleton {
	private:
		Singleton<T>( const Singleton& other );
		Singleton<T>& operator= ( const Singleton<T>& other );
	protected:
		Singleton<T>(){};
	public:
		static T& getInstance();
};

template <class T>
T& Singleton<T>::getInstance()
{
	static T instance;
	return instance;
}

#endif // SINGLETON_H
