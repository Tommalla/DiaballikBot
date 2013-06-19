#set (CMAKE_CXX_FLAGS "-DNDEBUG -O2 -std=c++11")

set (HEADERS
	AI.h
	botConstants.h
	botEnums.h
	CommunicationHandler.h
	GTPParser.h
	NullAI.h
	Singleton.h
)

set (SOURCES
	AI.cpp
	CommunicationHandler.cpp
	GTPParser.cpp
	main.cpp
	NullAI.cpp
)

add_executable (NullAI ${SOURCES} 
	${HEADERS} 
)

target_link_libraries(NullAI DiaballikEngine)

include_directories (${CMAKE_CURRENT_BINARY_DIR})
