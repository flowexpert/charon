IF(NOT CMAKE_FLAGS_INIT)
	SET(CMAKE_FLAGS_INIT TRUE CACHE INTERNAL "initial flags set")
	# initial compiler flags can be set here, this is only
	# executed once in the first configure run.
	IF(CMAKE_COMPILER_IS_GNUCXX)
		IF(NOT CMAKE_C_FLAGS)
			SET(CMAKE_C_FLAGS "-W -Wall -Wextra")
		ENDIF(NOT CMAKE_C_FLAGS)
		IF(NOT CMAKE_CXX_FLAGS)
			SET(CMAKE_CXX_FLAGS	"-W -Wall -Wextra")
		ENDIF(NOT CMAKE_CXX_FLAGS)
	ENDIF(CMAKE_COMPILER_IS_GNUCXX)
	IF(MSVC)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4251 /wd4275 /wd4290")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4275 /wd4290")
	ENDIF(MSVC)

	# commit changed flags
	SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} CACHE STRING
		"Flags used by the compiler during all build types."
		FORCE
	)
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

	# use CHARON_DIR as installation prefix, if this environment variable is set
	SET(CHARON_DIR_SET "$ENV{CHARON_DIR}")
	IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND CHARON_DIR_SET)
		SET(CMAKE_INSTALL_PREFIX "$ENV{CHARON_DIR}"
			CACHE PATH "Install path prefix, prepended onto install directories."
			FORCE
		)
		MESSAGE(STATUS "Using CHARON_DIR as install prefix: ${CMAKE_INSTALL_PREFIX}")
	ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND CHARON_DIR_SET)
	IF(COMMAND UNSET)
		UNSET(CHARON_DIR_SET)
	ENDIF(COMMAND UNSET)
ENDIF(NOT CMAKE_FLAGS_INIT)

# set definitions depending on build environment
IF(WIN32)
	ADD_DEFINITIONS(-DWINDOWS)
	IF(MSVC)
		ADD_DEFINITIONS(-DMSVC)
		# Tell MSVC not to use the unholy min and max macros
		ADD_DEFINITIONS(-DNOMINMAX)
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
