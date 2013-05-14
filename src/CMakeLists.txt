set (CMAKE_CXX_FLAGS "-std=c++11")

set (HEADERS
	CommunicationHandler.h
	GTPParser.h
	Singleton.h
)

set (SOURCES
	CommunicationHandler.cpp
	GTPParser.cpp
	main.cpp
)

add_executable (DiaballikBot ${SOURCES} 
	${HEADERS} 
)

include_directories (${CMAKE_CURRENT_BINARY_DIR})
