IF(NOT CMAKE_FLAGS_INIT)
	SET(CMAKE_FLAGS_INIT TRUE CACHE INTERNAL "initial flags set")
	# initial compiler flags can be set here, this is only
	# executed once in the first configure run.
	IF(CMAKE_COMPILER_IS_GNUCXX)
		IF(NOT CMAKE_C_FLAGS)
			SET(CMAKE_C_FLAGS
				"-W -Wall -Wextra -Wshadow"
				CACHE STRING
				"Flags used by the compiler during all build types."
				FORCE
			)
		ENDIF(NOT CMAKE_C_FLAGS)
		IF(NOT CMAKE_CXX_FLAGS)
			SET(CMAKE_CXX_FLAGS
				"-W -Wall -Wextra -Wshadow"
				CACHE STRING
				"Flags used by the compiler during all build types."
				FORCE
			)
		ENDIF(NOT CMAKE_CXX_FLAGS)
	ENDIF(CMAKE_COMPILER_IS_GNUCXX)
	IF(MSVC)
		SET(CMAKE_C_FLAGS
			"${CMAKE_C_FLAGS} /wd4251 /wd4290"
			CACHE STRING
			"Flags used by the compiler during all build types."
			FORCE
		)
		SET(CMAKE_CXX_FLAGS
			"${CMAKE_CXX_FLAGS} /wd4251 /wd4290"
			CACHE STRING
			"Flags used by the compiler during all build types."
			FORCE
		)
	ENDIF(MSVC)
ENDIF(NOT CMAKE_FLAGS_INIT)

# set definitions depending on build environment
IF(WIN32)
	ADD_DEFINITIONS(-DWINDOWS)
	IF(MSVC)
		ADD_DEFINITIONS(-DMSVC)
	ENDIF(MSVC)
	IF(MINGW)
		ADD_DEFINITIONS(-DMINGW)
	ENDIF(MINGW)
ENDIF(WIN32)
IF(UNIX)
	ADD_DEFINITIONS(-DUNIX)
	IF(APPLE)
		ADD_DEFINITIONS(-DAPPLE)
	ENDIF(APPLE)
ENDIF(UNIX)

