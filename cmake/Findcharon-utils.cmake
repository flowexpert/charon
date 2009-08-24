# - Find charon-utils by HCI
#
# Usage:
#  FIND_PACKAGE(charon-utils[ REQUIRED][ QUIET])
#  INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
#  ADD_EXECUTABLE(bla main.cpp)
#  TARGET_LINK_LIBRARIES(bla charon-utils)
#
# This sets the variables:
#  CHARON_UTILS_ROOT_DIR            charon-utils installation directory
#  CHARON_UTILS_INCLUDE_DIRS        charon-utils include directory
#  CHARON_UTILS_HTMLDOC_DIR         path to charon-utils htmldoc (optional)
#  CHARON_UTILS_TAG_IMPORT          doxygen tag import command (if possible)

# search for header files
FIND_PATH(CHARON_UTILS_ROOT_DIR
    NAMES           include/charon-utils/charon-utils.cmake
    PATHS           [HKEY_LOCAL_MACHINE\\SOFTWARE\\Heidelberg\ Collaboratory\ for\ Image\ Processing\\charon-utils]
                    /usr
                    /usr/local
    DOC             "charon-utils root directory"
)

FILE(GLOB CHARON_UTILS_DOCDIRS
    /usr/share/doc/charon-utils-*
)

FIND_PATH(CHARON_UTILS_HTMLDOC_DIR
    NAMES           charon-utils.tag
    PATHS           ${CHARON_UTILS_ROOT_DIR}/doc
                    ${CHARON_UTILS_DOCDIRS}
    PATH_SUFFIXES   html
    DOC             "path to charon-utils htmldoc (if exists)"
)

IF(CHARON_UTILS_HTMLDOC_DIR)
    # this can be used to import the charon doxygen documentation
    # this imports the needed tag file and links it to the found htmldoc location
    SET(CHARON_UTILS_TAG_IMPORT "\"${CHARON_UTILS_HTMLDOC_DIR}/charon-utils.tag=file://${CHARON_UTILS_HTMLDOC_DIR}\"")
ENDIF(CHARON_UTILS_HTMLDOC_DIR)

INCLUDE(FindPackageHandleStandardArgs)

SET(CHARON_UTILS_IMPORT_FILE
    ${CHARON_UTILS_ROOT_DIR}/include/charon-utils/charon-utils.cmake)

# check if everything went fine
IF(NOT CHARON_UTILS_ROOT_DIR)
	MESSAGE(SEND_ERROR
		"Charon-utils has not been found. "
		"Please set CHARON_UTILS_ROOT_DIR to the directory, "
		"where you have installed charon-utils. "
		"You have to run the install target after build of charon-utils "
		"or simply use the binary package."
	)
ENDIF(NOT CHARON_UTILS_ROOT_DIR)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(charon-utils DEFAULT_MSG
    CHARON_UTILS_IMPORT_FILE
    CHARON_UTILS_ROOT_DIR
)

INCLUDE(${CHARON_UTILS_IMPORT_FILE})
SET(CHARON_UTILS_INCLUDE_DIRS ${CHARON_UTILS_ROOT_DIR}/include/charon-utils/)

MARK_AS_ADVANCED(
    CHARON_UTILS_ROOT_DIR
    CHARON_UTILS_HTMLDOC_DIR
)

# cleanup
IF(COMMAND unset)
    UNSET(CHARON_UTILS_DOCDIRS)
    UNSET(CHARON_UTILS_IMPORT_FILE)
ENDIF(COMMAND unset)
