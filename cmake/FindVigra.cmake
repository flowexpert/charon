# - Find Vigra package by Ullrich Köthe
#
# Usage:
#  FIND_PACKAGE(Vigra[ REQUIRED][ QUIET])
#  INCLUDE_DIRECTORIES(${VIGRA_INCLUDE_DIRS})
#  ADD_EXECUTABLE(bla main.cpp)
#  TARGET_LINK_LIBRARIES(bla ${VIGRA_LIBRARIES})
#
# This sets the variables:
#  VIGRA_ROOT_DIR            vigra installation directory
#  VIGRA_INCLUDE_DIRS        vigra include directories
#  VIGRA_LIBRARIES           vigra libraries

# search for header files
FIND_PATH(VIGRA_ROOT_DIR
    NAMES           include/vigra/multi_array.hxx
    PATHS           [HKEY_LOCAL_MACHINE\\SOFTWARE\\Heidelberg\ Collaboratory\ for\ Image\ Processing\\vigra]
                    /usr
                    /usr/local
    DOC             "vigra root directory"
)

INCLUDE(FindPackageHandleStandardArgs)

# check if everything went fine
IF(NOT VIGRA_ROOT_DIR)
	MESSAGE(SEND_ERROR
		"Vigra has not been found. "
		"Please set VIGRA_ROOT_DIR to the directory, "
		"where you have installed Vigra. "
		"You have to run the install target after build of vigra "
		"or simply use the binary package."
	)
ENDIF(NOT VIGRA_ROOT_DIR)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vigra DEFAULT_MSG
    VIGRA_ROOT_DIR
)

SET(VIGRA_INCLUDE_DIRS
	${VIGRA_ROOT_DIR}/include/vigra/
	${VIGRA_ROOT_DIR}/include/
)

MARK_AS_ADVANCED(
    VIGRA_ROOT_DIR
)
