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

# as multiple compiled version of petsc with different architectures
# can reside in the root directory, the architecture has to be specified
# manually using PETSC_ARCH
# with both the ROOT_DIR and ARCH every needed path can be generated

INCLUDE(FindPackageHandleStandardArgs)

# check if everything went fine
IF(NOT PETSC_ROOT_DIR)
	MESSAGE(SEND_ERROR
		"PETSc has not been found. "
		"Please set PETSC_ROOT_DIR to the directory, "
		"where you have installed PETSc. "
	)
ENDIF(NOT PETSC_ROOT_DIR)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(petsc DEFAULT_MSG
    PETSC_ROOT_DIR
)

SET(PETSC_INCLUDE_DIRS
	${PETSC_ROOT_DIR}/include/
	${PETSC_ROOT_DIR}/bmake/${PETSC_ARCH}/
)

IF (NOT PETSC_USES_MPI)
	SET(PETSC_INCLUDE_DIRS
		${PETSC_INCLUDE_DIRS}
		${PETSC_ROOT_DIR}/include/mpiuni/
	)
ENDIF (NOT PETSC_USES_MPI)


MARK_AS_ADVANCED(
    PETSC_ROOT_DIR
    PETSC_CONFIG_DIR
)