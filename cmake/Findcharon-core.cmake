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

# search for header files
FIND_PATH(CHARON_CORE_ROOT_DIR
    NAMES           include/charon-core/charon-core.cmake
    PATHS           [HKEY_LOCAL_MACHINE\\SOFTWARE\\Heidelberg\ Collaboratory\ for\ Image\ Processing\\charon-core]
                    /usr
                    /usr/local
    DOC             "charon-core root directory"
)

FILE(GLOB CHARON_CORE_DOCDIRS
    /usr/share/doc/charon-core*
)

FIND_FILE(CHARON_CORE_TAGFILE
	NAMES			charon-core.tag
    PATHS           ${CHARON_CORE_ROOT_DIR}/doc/charon-core
    		        ${CHARON_CORE_ROOT_DIR}/doc
                    ${CHARON_CORE_DOCDIRS}
    PATH_SUFFIXES   html
	DOC				"doxygen import tag file"
)
FIND_PATH(CHARON_CORE_HTMLDOC_DIR
    NAMES           index.html
    PATHS           ${CHARON_CORE_ROOT_DIR}/doc/charon-core
    		        ${CHARON_CORE_ROOT_DIR}/doc
                    ${CHARON_CORE_DOCDIRS}
    PATH_SUFFIXES   html
    DOC             "path to charon-core htmldoc (if exists)"
)

IF(CHARON_CORE_HTMLDOC_DIR)
    # this can be used to import the charon doxygen documentation
    # this imports the needed tag file and links it to the found htmldoc location
    SET(CHARON_CORE_TAG_IMPORT "\"${CHARON_CORE_TAGFILE}=${CHARON_CORE_HTMLDOC_DIR}\"")
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

MARK_AS_ADVANCED(
    CHARON_CORE_ROOT_DIR
	CHARON_CORE_TAGFILE
    CHARON_CORE_HTMLDOC_DIR
)

# cleanup
IF(COMMAND unset)
    UNSET(CHARON_CORE_DOCDIRS)
    UNSET(CHARON_CORE_IMPORT_FILE)
ENDIF(COMMAND unset)
