#set (CMAKE_CXX_FLAGS "-DNDEBUG -O2 -std=c++11")

set (HEADERS
	AI.h
	botConstants.h
	botEnums.h
	CommunicationHandler.h
	GTPParser.h
	MCTNode.h
	MCTSAI.h
	NullAI.h
	Singleton.h
)

set (SOURCES
	AI.cpp
	CommunicationHandler.cpp
	GTPParser.cpp
	main.cpp
	MCTSAI.cpp
	MCTNode.cpp
	NullAI.cpp
)

add_executable (DiaballikBot ${SOURCES} 
	${HEADERS} 
)

target_link_libraries(DiaballikBot DiaballikEngine)

include_directories (${CMAKE_CURRENT_BINARY_DIR})
