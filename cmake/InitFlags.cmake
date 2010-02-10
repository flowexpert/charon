IF(NOT CMAKE_FLAGS_INIT)
	SET(CMAKE_FLAGS_INIT TRUE CACHE INTERNAL "initial flags set")
	# initial compiler flags can be set here, this is only
	# executed once in the first configure run.
	IF(CMAKE_COMPILER_IS_GNUCXX)
		IF(NOT CMAKE_C_FLAGS)
			SET(CMAKE_C_FLAGS   "-W -Wall -Wextra")
		ENDIF()
		IF(NOT CMAKE_CXX_FLAGS)
			SET(CMAKE_CXX_FLAGS "-W -Wall -Wextra")
		ENDIF()
	ENDIF(CMAKE_COMPILER_IS_GNUCXX)
	IF(MSVC)
		SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /wd4251 /wd4290")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4290")
	ENDIF(MSVC)

	# default to debug builds
	IF(CMAKE_BUILD_TYPE STREQUAL "")
		SET(CMAKE_BUILD_TYPE "Debug")
	ENDIF()

	# record values into CMakeCache
	SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
		CACHE STRING
		"Flags used by the compiler during all build types."
		FORCE
	)
	SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
		CACHE STRING
		"Flags used by the compiler during all build types."
		FORCE
	)
	SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}
		CACHE STRING
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE
	)
ENDIF(NOT CMAKE_FLAGS_INIT)
