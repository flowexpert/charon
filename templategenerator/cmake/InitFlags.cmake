IF(NOT CMAKE_FLAGS_INIT)
	SET(CMAKE_FLAGS_INIT TRUE CACHE INTERNAL "initial flags set")
	# initial compiler flags can be set here, this is only
	# executed once in the first configure run.
	IF(CMAKE_COMPILER_IS_GNUCXX)
		IF(NOT CMAKE_CXX_FLAGS)
			SET(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Wshadow -pedantic -std=c++0x")
		ENDIF(NOT CMAKE_CXX_FLAGS)
	ENDIF(CMAKE_COMPILER_IS_GNUCXX)
	IF(MSVC)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4275 /wd4290 /wd4244 /Zc:wchar_t-")
	ENDIF(MSVC)

	# commit changed flags
	SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} CACHE STRING
		"Flags used by the compiler during all build types."
		FORCE
	)

	# default to debug builds
	IF(DEFINED CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "")
		SET(CMAKE_BUILD_TYPE "Debug"
			CACHE STRING
			"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
			FORCE
		)
	ENDIF(DEFINED CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "")
ENDIF(NOT CMAKE_FLAGS_INIT)
