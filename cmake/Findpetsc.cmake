# - Find petsc
#
# Usage:
#  FIND_PACKAGE(petsc[ REQUIRED][ QUIET])
#  INCLUDE_DIRECTORIES(${PETSC_INCLUDE_DIRS})
#  ADD_EXECUTABLE(bla main.cpp)
#  TARGET_LINK_LIBRARIES(bla petsc)
#
# This sets the variables:
#  PETSC_ROOT_DIR 		           petsc installation directory
#  PETSC_INCLUDE_DIRS        petsc include directory

# search for header files
FIND_PATH(PETSC_ROOT_DIR
    NAMES           include/petscksp.h
    PATHS           /usr
                    /usr/local
                    /opt
    DOC             "petsc root directory"
)

# this has to be improved to find the configure directory automatically
FIND_PATH(PETSC_CONFIG_DIR
	NAMES			include/petscconf.h
	PATHS			${PETSC_ROOT_DIR}
	DOC				"petsc configuration directory"
)

INCLUDE(FindPackageHandleStandardArgs)

# check if everything went fine
IF(NOT PETSC_ROOT_DIR)
	MESSAGE(SEND_ERROR
		"PETSc has not been found. "
		"Please set PETSC_ROOT_DIR to the directory, "
		"where you have installed PETSc. "
	)
ENDIF(NOT PETSC_ROOT_DIR)
IF(NOT PETSC_CONFIG_DIR)
	MESSAGE(SEND_ERROR
		"PETSc configuration directory has to be specified. "
		"Please set PETSC_CONFIG_DIR to the correct subpath of your PETSc "
		"directory (currently set to ${PETSC_ROOT_DIR})"
	)
ENDIF(NOT PETSC_CONFIG_DIR)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(petsc DEFAULT_MSG
    PETSC_ROOT_DIR
    PETSC_CONFIG_DIR
)

SET(PETSC_INCLUDE_DIRS
	${PETSC_ROOT_DIR}/include/
	${PETSC_CONFIG_DIR}/include/
)

MARK_AS_ADVANCED(
    PETSC_ROOT_DIR
    PETSC_CONFIG_DIR
)