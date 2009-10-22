IF(NOT CMAKE_FLAGS_INIT)
	SET(CMAKE_FLAGS_INIT TRUE CACHE INTERNAL "initial flags set")
	# initial compiler flags can be set here, this is only
	# executed once in the first configure run.
	IF(CMAKE_COMPILER_IS_GNUCXX)
		IF(NOT CMAKE_C_FLAGS)
		SET(CMAKE_C_FLAGS
			"-W -Wall -Wextra -Wshadow -pedantic"
			CACHE STRING
			"Flags used by the compiler during all build types."
			FORCE
		)
		ENDIF(NOT CMAKE_C_FLAGS)
		IF(NOT CMAKE_CXX_FLAGS)
		SET(CMAKE_CXX_FLAGS
			"-W -Wall -Wextra -Wshadow -pedantic"
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
	# set install prefix to global install path
	SET(CMAKE_INSTALL_PREFIX ${CHARON_UTILS_ROOT_DIR}
		CACHE PATH "Install path prefix, prepended onto install directories."
		FORCE
	)
	# default to debug builds
	IF(DEFINED CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "")
		SET(CMAKE_BUILD_TYPE "Debug"
			CACHE STRING
			"Choose the type of build, options are: None(CMAKE_CXX_FLAGS or"
			"CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
			FORCE
		)
	ENDIF(DEFINED CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "")
ENDIF(NOT CMAKE_FLAGS_INIT)
IF(NOT ${CMAKE_INSTALL_PREFIX} STREQUAL ${CHARON_UTILS_ROOT_DIR})
	MESSAGE(STATUS	"Warning: Plugins will NOT be installed in standart "
					"plugin directory!")
	MESSAGE(STATUS	"Set CMAKE_INSTALL_PREFIX to ${CHARON_UTILS_ROOT_DIR} "
					"to install into global plugin path.")
ENDIF(NOT ${CMAKE_INSTALL_PREFIX} STREQUAL ${CHARON_UTILS_ROOT_DIR})

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

