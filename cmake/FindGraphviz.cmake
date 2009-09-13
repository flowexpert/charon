# - Find Graphviz by AT&T Research Labs
# Search for graphviz headers, libraries
#
# Usage:
#  FIND_PACKAGE(Graphviz[ REQUIRED][ QUIET])
#  INCLUDE_DIRECTORIES(${GRAPHVIZ_INCLUDE_DIRS})
#  ADD_EXECUTABLE(bla main.cpp)
#  TARGET_LINK_LIBRARIES(bla ${GRAPHVIZ_LIBRARIES})
#
# This sets the variables:
#  GRAPHVIZ_INCLUDE_DIRS			graphviz include directory
#  GRAPHVIZ_LIBRARY_DIRS			graphviz library directory
#  GRAPHVIZ_DOT_EXECUTABLE			graphviz dot executable
#  GRAPHVIZ_VERSION_STRING			dot version string (major.minor.patch.tweak)
#  GRAPHVIZ_VERSION_MAJOR			parts of the version string
#  GRAPHVIZ_VERSION_MINOR			- " -
#  GRAPHVIZ_VERSION_PATCH			- " -
#
# For usage with Visual Studio you need 
# at least version graphviz-2.22.x.msi
# (e.g. graphviz-2.22.2.msi)
# and NOT the MinGW version!
#
# If you want to compile using MinGW use
# graphviz-mingw-2.22.x.msi
# instead!

# search for header files
FIND_PATH(GRAPHVIZ_ROOT_DIR
	NAMES			include/graphviz/gvc.h
					include/graphviz/types.h
					include/graphviz/graph.h
	PATHS			[HKEY_LOCAL_MACHINE\\SOFTWARE\\AT&T\ Research\ Labs\\Graphviz;InstallPath]
					[HKEY_LOCAL_MACHINE\\SOFTWARE\\ATT\\Graphviz;InstallPath]
					"$ENV{ProgramFiles}\\Graphviz"
					"C:\\Program Files\\Graphviz"
					/usr
					/usr/local
	DOC				"graphviz root directory"
)

SET(GRAPHVIZ_LIBRARY_DIRS ${GRAPHVIZ_ROOT_DIR}/lib)
SET(GRAPHVIZ_INCLUDE_DIRS ${GRAPHVIZ_ROOT_DIR}/include ${GRAPHVIZ_ROOT_DIR}/include/graphviz)


# search for graphviz libraries
FIND_LIBRARY(GRAPHVIZ_CDT_LIBRARY
	NAMES			cdt
	PATHS			${GRAPHVIZ_LIBRARY_DIRS}
	DOC				"graphviz container types"
	NO_DEFAULT_PATH
)

FIND_LIBRARY(GRAPHVIZ_GRAPH_LIBRARY
	NAMES			graph
	PATHS			${GRAPHVIZ_LIBRARY_DIRS}
	DOC				"graphviz container types"
	NO_DEFAULT_PATH
)

FIND_LIBRARY(GRAPHVIZ_GVC_LIBRARY
	NAMES			gvc
	PATHS			${GRAPHVIZ_LIBRARY_DIRS}
	DOC				"graphviz container types"
	NO_DEFAULT_PATH
)

# search for dot executable
FIND_PROGRAM(GRAPHVIZ_DOT_EXECUTABLE
    NAMES			dot
	PATHS			${GRAPHVIZ_ROOT_DIR}
	PATH_SUFFIXES	bin
    DOC				"Graphviz dot tool"
)

MARK_AS_ADVANCED(
	GRAPHVIZ_ROOT_DIR
	GRAPHVIZ_CDT_LIBRARY
	GRAPHVIZ_GRAPH_LIBRARY
	GRAPHVIZ_GVC_LIBRARY
	GRAPHVIZ_DOT_EXECUTABLE
)

# try to get graphviz version (executing dot -V)
EXECUTE_PROCESS(
	COMMAND			${GRAPHVIZ_DOT_EXECUTABLE} -V
	RESULT_VARIABLE	GRAPHVIZ_DOT_RUN_RESULT
	OUTPUT_VARIABLE	GRAPHVIZ_DOT_RUN_OUTPUT
	ERROR_VARIABLE	GRAPHVIZ_DOT_RUN_ERROR
	OUTPUT_STRIP_TRAILING_WHITESPACE
	ERROR_STRIP_TRAILING_WHITESPACE
)

SET(GRAPHVIZ_DOT_RUN_OUTPUT ${GRAPHVIZ_DOT_RUN_OUTPUT} ${GRAPHVIZ_DOT_RUN_ERROR})

IF(GRAPHVIZ_DOT_RUN_RESULT AND Graphviz_FIND_REQUIRED)
	MESSAGE(STATUS "Dot returns exit code  ${GRAPHVIZ_DOT_RUN_RESULT}")
	MESSAGE(STATUS "Dot output: ${GRAPHVIZ_DOT_RUN_OUTPUT}")
	MESSAGE(FATAL_ERROR "Dot executable NOT working!")
ENDIF(GRAPHVIZ_DOT_RUN_RESULT AND Graphviz_FIND_REQUIRED)


# parse dot output
STRING(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+"
	GRAPHVIZ_VERSION_STRING "${GRAPHVIZ_DOT_RUN_OUTPUT}")
IF(GRAPHVIZ_VERSION_STRING)
	# Version String like x.xx.x (e.g. 2.20.3)
	STRING(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+$" "\\1"
		GRAPHVIZ_VERSION_MAJOR "${GRAPHVIZ_VERSION_STRING}")
	STRING(REGEX REPLACE "^[0-9]+\\.([0-9]+)\\.[0-9]+$" "\\1"
		GRAPHVIZ_VERSION_MINOR "${GRAPHVIZ_VERSION_STRING}")
	STRING(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)$" "\\1"
		GRAPHVIZ_VERSION_PATCH "${GRAPHVIZ_VERSION_STRING}")
ELSE(GRAPHVIZ_VERSION_STRING)
	# Version String like x.xx (e.g. 2.22)
	STRING(REGEX MATCH "[0-9]+\\.[0-9]+"
		GRAPHVIZ_VERSION_STRING "${GRAPHVIZ_DOT_RUN_OUTPUT}")
	STRING(REGEX REPLACE "^([0-9]+)\\.[0-9]+$" "\\1"
		GRAPHVIZ_VERSION_MAJOR "${GRAPHVIZ_VERSION_STRING}")
	STRING(REGEX REPLACE "^[0-9]+\\.([0-9]+)$" "\\1"
		GRAPHVIZ_VERSION_MINOR "${GRAPHVIZ_VERSION_STRING}")
	SET(GRAPHVIZ_VERSION_PATCH 0)
ENDIF(GRAPHVIZ_VERSION_STRING)

# check if version matches
#TODO: change to IF(... VERSION_LESS ...) syntax
SET(GRAPHVIZ_VERSION_FINE TRUE)
IF(Graphviz_FIND_VERSION)
	IF(NOT GRAPHVIZ_VERSION_MAJOR)
		MESSAGE("Error: Graphviz version string could not be parsed!")
		MESSAGE("Version  Output: ${GRAPHVIZ_DOT_RUN_OUTPUT}")
		MESSAGE("Expected Output: dot - graphviz version x.xx.x(.xxxxxxxx) (xxxxxxxx.xxxx)")
		MESSAGE(FATAL_ERROR "Version parse Error")
	ENDIF(NOT GRAPHVIZ_VERSION_MAJOR)
	MATH(EXPR req_vers   "${Graphviz_FIND_VERSION_MAJOR}*10000000000 + ${Graphviz_FIND_VERSION_MINOR}*100000000 + ${Graphviz_FIND_VERSION_PATCH}")
	MATH(EXPR found_vers "     ${GRAPHVIZ_VERSION_MAJOR}*10000000000 +      ${GRAPHVIZ_VERSION_MINOR}*100000000 +      ${GRAPHVIZ_VERSION_PATCH}")
	IF (found_vers LESS req_vers)
      SET(GRAPHVIZ_VERSION_FINE FALSE)
    ENDIF (found_vers LESS req_vers)
ENDIF(Graphviz_FIND_VERSION)

# result output generated by FindPackageHandleStandardArgs
SET(GRAPHVIZ_FOUND_MESSAGE "${GRAPHVIZ_DOT_EXECUTABLE} (Version ${GRAPHVIZ_VERSION_STRING})")
INCLUDE(FindPackageHandleStandardArgs)

IF(APPLE)
	# check if everything went fine
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(Graphviz DEFAULT_MSG
		GRAPHVIZ_FOUND_MESSAGE
		GRAPHVIZ_GRAPH_LIBRARY
		GRAPHVIZ_ROOT_DIR
		GRAPHVIZ_CDT_LIBRARY
		GRAPHVIZ_GVC_LIBRARY
		GRAPHVIZ_DOT_EXECUTABLE
		#Disabled because checking does not work currently.
		#GRAPHVIZ_VERSION_FINE
	)
ELSE(APPLE)
	# check if everything went fine
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(Graphviz DEFAULT_MSG
		GRAPHVIZ_FOUND_MESSAGE
		GRAPHVIZ_GRAPH_LIBRARY
		GRAPHVIZ_ROOT_DIR
		GRAPHVIZ_CDT_LIBRARY
		GRAPHVIZ_GVC_LIBRARY
		GRAPHVIZ_DOT_EXECUTABLE
		GRAPHVIZ_VERSION_FINE
	)
ENDIF(APPLE)

SET(GRAPHVIZ_LIBRARIES
	${GRAPHVIZ_GRAPH_LIBRARY};
	${GRAPHVIZ_CDT_LIBRARY};
	${GRAPHVIZ_GVC_LIBRARY};
)

# cleanup
IF(COMMAND unset)
    UNSET(GRAPHVIZ_DOT_RUN_RESULT)
    UNSET(GRAPHVIZ_DOT_RUN_OUTPUT)
    UNSET(GRAPHVIZ_DOT_RUN_ERROR)
    UNSET(GRAPHVIZ_FOUND_MESSAGE)
ENDIF(COMMAND unset)

#IF(WIN32 AND INSTALL_DLLS)
#	# install dlls
#	INSTALL(FILES
#		${GRAPHVIZ_ROOT_DIR}/bin/cdt.dll
#		${GRAPHVIZ_ROOT_DIR}/bin/graph.dll
#		${GRAPHVIZ_ROOT_DIR}/bin/gvc.dll
#		${GRAPHVIZ_ROOT_DIR}/bin/libexpat.dll
#		${GRAPHVIZ_ROOT_DIR}/bin/ltdl.dll
#		${GRAPHVIZ_ROOT_DIR}/bin/Pathplan.dll
#		${GRAPHVIZ_ROOT_DIR}/bin/zlib1.dll
#		DESTINATION		bin
#		COMPONENT		dlls
#	)
#ENDIF(WIN32 AND INSTALL_DLLS)
