# - Find charon-core by HCI
#
# Usage:
#  FIND_PACKAGE(charon-core[ REQUIRED][ QUIET])
#  INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
#  ADD_EXECUTABLE(bla main.cpp)
#  TARGET_LINK_LIBRARIES(bla charon-core)
#
# This sets the variables:
#  CHARON_CORE_ROOT_DIR            charon-core installation directory
#  CHARON_CORE_INCLUDE_DIRS        charon-core include directory
#  CHARON_CORE_HTMLDOC_DIR         path to charon-core htmldoc (optional)
#  CHARON_CORE_TAG_IMPORT          doxygen tag import command (if possible)

# if charon-core is already included (e.g. in charon-meta),
# we can use the libraries and variables directly.
IF(DEFINED charon-core_SOURCE_DIR)
	SET(CHARON_CORE_ROOT_DIR ${charon-core_SOURCE_DIR})
	SET(CHARON_CORE_INCLUDE_DIRS
		${charon-core_SOURCE_DIR}/include
		${charon-core_SOURCE_DIR}/include/charon-core
	)
	SET(CHARON_CORE_HTMLDOC_DIR
		${CMAKE_INSTALL_PREFIX}/${charon-core_INSTALL_DOC}/html
	)
	SET(CHARON_CORE_TAGFILE
		${charon-core_BINARY_DIR}/doc/html/charon-core.tag
	)
	SET(CHARON_CORE_TAG_IMPORT
		"\"${CHARON_CORE_TAGFILE}=${CHARON_CORE_HTMLDOC_DIR}\""
	)
	GET_TARGET_PROPERTY(CHARON_CORE_VERSION charon-core VERSION)
	RETURN()
ENDIF(DEFINED charon-core_SOURCE_DIR)

# search for header files
FIND_PATH(CHARON_CORE_ROOT_DIR
	NAMES include/charon-core/charon-core.cmake
	PATHS
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Heidelberg\ Collaboratory\ for\ Image\ Processing\\charon-core]
		$ENV{CHARON_DIR}
		/usr
		/usr/local
	DOC "charon-core root directory"
)

FILE(GLOB CHARON_CORE_DOCDIRS
	/usr/share/doc/charon-core*
)

FIND_FILE(CHARON_CORE_TAGFILE
	NAMES charon-core.tag
	PATHS
		${CHARON_CORE_ROOT_DIR}/doc/charon-core
		${CHARON_CORE_ROOT_DIR}/doc
		${CHARON_CORE_DOCDIRS}
	PATH_SUFFIXES html
	DOC "doxygen import tag file"
	NO_DEFAULT_PATH
)
FIND_PATH(CHARON_CORE_HTMLDOC_DIR
	NAMES index.html
	PATHS
		${CHARON_CORE_ROOT_DIR}/doc/charon-core
		${CHARON_CORE_ROOT_DIR}/doc
		${CHARON_CORE_DOCDIRS}
	PATH_SUFFIXES html
	DOC "path to charon-core htmldoc (if exists)"
	NO_DEFAULT_PATH
)

IF(CHARON_CORE_HTMLDOC_DIR)
	# this can be used to import the charon doxygen documentation
	# this imports the needed tag file and links it to the
	# found htmldoc location
	SET(CHARON_CORE_TAG_IMPORT
		"\"${CHARON_CORE_TAGFILE}=${CHARON_CORE_HTMLDOC_DIR}\"")
ENDIF(CHARON_CORE_HTMLDOC_DIR)

INCLUDE(FindPackageHandleStandardArgs)

SET(CHARON_CORE_IMPORT_FILE
	${CHARON_CORE_ROOT_DIR}/include/charon-core/charon-core.cmake)

# check if everything went fine
IF(NOT CHARON_CORE_ROOT_DIR)
	MESSAGE(SEND_ERROR
		"Charon-core has not been found. "
		"Please set CHARON_CORE_ROOT_DIR to the directory, "
		"where you have installed charon-core. "
		"You have to run the install target after build of charon-core "
		"or simply use the binary package."
	)
ENDIF(NOT CHARON_CORE_ROOT_DIR)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(charon-core DEFAULT_MSG
	CHARON_CORE_IMPORT_FILE
	CHARON_CORE_ROOT_DIR
)

INCLUDE(${CHARON_CORE_IMPORT_FILE})
SET(CHARON_CORE_INCLUDE_DIRS
	${CHARON_CORE_ROOT_DIR}/include/charon-core/
	${CHARON_CORE_ROOT_DIR}/include/
)

# determine version, if possible
GET_TARGET_PROPERTY(charon_LOCATION charon LOCATION)
EXECUTE_PROCESS(
	COMMAND ${charon_LOCATION} --version
	TIMEOUT 1
	RESULT_VARIABLE charon_VERSION_RESULT
	OUTPUT_VARIABLE charon_VERSION_OUTPUT
	ERROR_VARIABLE charon_VERSION_ERROR
)
#MESSAGE("charon version run output: ${charon_VERSION_OUTPUT}")
#MESSAGE("charon version run error: ${chron_VERSION_ERROR}")
IF(charon_VERSION_RESULT EQUAL 0 AND NOT charon_VERSION_ERROR)
	STRING(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+"
		CHARON_CORE_VERSION "${charon_VERSION_OUTPUT}")
	#MESSAGE(STATUS "Charon-core version: ${CHARON_CORE_VERSION}")
ELSE()
	MESSAGE(WARNING
		"Charon-core version could not be determined! "
		"Please update charon-core package."
	)
ENDIF()

MARK_AS_ADVANCED(
	CHARON_CORE_ROOT_DIR
	CHARON_CORE_TAGFILE
	CHARON_CORE_HTMLDOC_DIR
)

# cleanup
IF(COMMAND unset)
	UNSET(CHARON_CORE_DOCDIRS)
	UNSET(CHARON_CORE_IMPORT_FILE)
	UNSET(charon_LOCATION)
	UNSET(charon_VERSION_RESULT)
	UNSET(charon_VERSION_OUTPUT)
	UNSET(charon_VERSION_ERROR)
ENDIF(COMMAND unset)
