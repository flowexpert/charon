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
#  VIGRA_VERSION             vigra version string (e.g. "1.7.0")

IF(DEFINED WITH_VIGRA AND NOT WITH_VIGRA)
	RETURN()
ENDIF()

# search for header files
FIND_PATH(VIGRA_ROOT_DIR
    NAMES           include/vigra/multi_array.hxx
    PATHS           [HKEY_LOCAL_MACHINE\\SOFTWARE\\Heidelberg\ Collaboratory\ for\ Image\ Processing\\vigra]
                    /usr
                    /usr/local
    DOC             "vigra root directory"
)

# check if everything went fine
IF(NOT VIGRA_ROOT_DIR)
	MESSAGE(WARNING
		"Vigra has not been found. "
		"Please set VIGRA_ROOT_DIR to the directory, "
		"where you have installed Vigra. "
		"You have to run the install target after build of vigra "
		"or simply use the binary package."
	)
ENDIF(NOT VIGRA_ROOT_DIR)

SET(VIGRA_INCLUDE_DIRS ${VIGRA_ROOT_DIR}/include)

# Try to compile and run a simple test file to detect
# the vigra version. This works with vigra version
# b62549894487 and above. In the current official
# repo, the Version sting is not defined.
TRY_RUN(
	VIGRA_CHECK_RUN_RESULT
	VIGRA_CHECK_COMPILE_RESULT
	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
	"${CMAKE_MODULE_PATH}/VigraCheck.cpp"
	COMPILE_DEFINITIONS -I"${VIGRA_INCLUDE_DIRS}"
	COMPILE_OUTPUT_VARIABLE VIGRA_CHECK_COMPILE
	RUN_OUTPUT_VARIABLE VIGRA_VERSION
)
IF(NOT VIGRA_CHECK_COMPILE_RESULT AND NOT Vigra_FIND_QUIETLY)
	MESSAGE(WARNING "Error compiling a simple Vigra test file!")
	MESSAGE(AUTHOR_WARNING "Compile output:\n${VIGRA_CHECK_COMPILE}")
ENDIF()
SET(VIGRA_FIND_MESSAGE "Version ${VIGRA_VERSION} in ${VIGRA_ROOT_DIR}")
IF(VIGRA_CHECK_RUN_RESULT EQUAL 0)
	SET(VIGRA_CHECK_RUN_SUCCESS TRUE)
ENDIF()

SET(VIGRA_VERSION_FINE TRUE)
IF(Vigra_FIND_VERSION)
	IF(NOT Vigra_FIND_VERSION_EXACT AND
			VIGRA_VERSION VERSION_LESS Vigra_FIND_VERSION)
		SET(VIGRA_VERSION_FINE FALSE)
		IF(NOT Vigra_FIND_QUIETLY)
			MESSAGE(WARNING
				"    Vigra version does not match requirements.\n"
				"    Expected at least ${Vigra_FIND_VERSION}\n"
				"    but found         ${VIGRA_VERSION}.\n")
		ENDIF(NOT Vigra_FIND_QUIETLY)
	ENDIF()
	IF(Vigra_FIND_VERSION_EXACT AND
			NOT VIGRA_VERSION VERSION_EQUAL Vigra_FIND_VERSION)
		SET(VIGRA_VERSION_FINE FALSE)
		IF(NOT Vigra_FIND_QUIETLY)
			MESSAGE(WARNING
				"    Vigra version does not match requirements.\n"
				"    Expected exactly ${Vigra_FIND_VERSION}\n"
				"    but found        ${VIGRA_VERSION}.\n")
		ENDIF(NOT Vigra_FIND_QUIETLY)
	ENDIF()
ENDIF(Vigra_FIND_VERSION)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vigra DEFAULT_MSG
	VIGRA_FIND_MESSAGE
    VIGRA_ROOT_DIR
	VIGRA_CHECK_COMPILE_RESULT
	VIGRA_CHECK_RUN_SUCCESS
	VIGRA_VERSION_FINE
)

IF(COMMAND UNSET)
	UNSET(VIGRA_FIND_MESSAGE)
	UNSET(VIGRA_CHECK_COMPILE_RESULT)
	UNSET(VIGRA_CHECK_COMPILE)
	UNSET(VIGRA_CHECK_RUN_RESULT)
	UNSET(VIGRA_CHECK_RUN_SUCCESS)
	UNSET(VIGRA_VERSION_FINE)
ENDIF(COMMAND UNSET)

MARK_AS_ADVANCED(
    VIGRA_ROOT_DIR
)
