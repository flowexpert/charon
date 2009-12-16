# - Find petsc
#
# Usage:
#  FIND_PACKAGE(petsc[ REQUIRED][ QUIET])
#  INCLUDE_DIRECTORIES(${PETSC_INCLUDE_DIRS})
#  ADD_EXECUTABLE(bla main.cpp)
#  TARGET_LINK_LIBRARIES(bla petsc)
#
# This sets the variables:
#  PETSC_ROOT_DIR 		     petsc installation directory
#  PETSC_INCLUDE_DIRS        petsc include directory

# search for header files
FIND_PATH(PETSC_ROOT_DIR
    NAMES           include/petscksp.h
	HINTS			$ENV{PETSC_DIR}
    PATHS           /usr
                    /usr/local
                    /opt/petsc
                    /opt
    DOC             "petsc root directory"
)

# as multiple compiled version of petsc with different architectures
# can reside in the root directory, the architecture has to be specified
# manually using PETSC_ARCH
# with both the ROOT_DIR and ARCH every needed path can be generated
IF(WIN32)
	SET(PETSC_ARCH "win_x86_32_mpiuni"
		CACHE STRING "selecte petsc architecture"
)
ELSE(WIN32)
	SET(PETSC_ARCH "linux-gnu-c++-debug"
		CACHE STRING "selecte petsc architecture"
	)
ENDIF(WIN32)
# use value of envirionmental variable if set
IF(NOT "$ENV{PETSC_ARCH}" STREQUAL "")
	SET(PETSC_ARCH "$ENV{PETSC_ARCH}"
		CACHE STRING "selecte petsc architecture"
		FORCE
	)
ENDIF(NOT "$ENV{PETSC_ARCH}" STREQUAL "")
OPTION(WITH_MPI "does PETSc use an external MPI implementation?" OFF)
OPTION(WITH_FORTRAN "was fortran used for PETSc-compilation?" OFF)

SET(PETSC_LIBRARY_SEARCH_HINT
	${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}
	${PETSC_ROOT_DIR}/${PETSC_ARCH}/lib
)

# cygwin generated msvc libraries are names using the unix
# naming scheme, therefore we have to add the names with "lib" prefix
FIND_LIBRARY(PETSC_LIBPETSC
	NAMES petsc libpetsc
	HINTS ${PETSC_LIBRARY_SEARCH_HINT}
	PATHS ${PETSC_ROOT_DIR}
	PATH_SUFFIXES lib lib/${PETSC_ARCH}
)
FIND_LIBRARY(PETSC_LIBPETSCCONTRIB
	NAMES petsccontrib libpetsccontrib
	HINTS ${PETSC_LIBRARY_SEARCH_HINT}
	PATHS ${PETSC_ROOT_DIR}
	PATH_SUFFIXES lib lib/${PETSC_ARCH}
)
FIND_LIBRARY(PETSC_LIBPETSCDM
	NAMES petscdm libpetscdm
	HINTS ${PETSC_LIBRARY_SEARCH_HINT}
	PATHS ${PETSC_ROOT_DIR}
	PATH_SUFFIXES lib lib/${PETSC_ARCH}
)
FIND_LIBRARY(PETSC_LIBPETSCKSP
	NAMES petscksp libpetscksp
	HINTS ${PETSC_LIBRARY_SEARCH_HINT}
	PATHS ${PETSC_ROOT_DIR}
	PATH_SUFFIXES lib lib/${PETSC_ARCH}
)
FIND_LIBRARY(PETSC_LIBPETSCMAT
	NAMES petscmat libpetscmat
	HINTS ${PETSC_LIBRARY_SEARCH_HINT}
	PATHS ${PETSC_ROOT_DIR}
	PATH_SUFFIXES lib lib/${PETSC_ARCH}
)
FIND_LIBRARY(PETSC_LIBPETSCSNES
	NAMES petscsnes libpetscsnes
	HINTS ${PETSC_LIBRARY_SEARCH_HINT}
	PATHS ${PETSC_ROOT_DIR}
	PATH_SUFFIXES lib lib/${PETSC_ARCH}
)
FIND_LIBRARY(PETSC_LIBPETSCTS
	NAMES petscts libpetscts
	HINTS ${PETSC_LIBRARY_SEARCH_HINT}
	PATHS ${PETSC_ROOT_DIR}
	PATH_SUFFIXES lib lib/${PETSC_ARCH}
)
FIND_LIBRARY(PETSC_LIBPETSCVEC
	NAMES petscvec libpetscvec
	HINTS ${PETSC_LIBRARY_SEARCH_HINT}
	PATHS ${PETSC_ROOT_DIR}
	PATH_SUFFIXES lib lib/${PETSC_ARCH}
)
FIND_LIBRARY(PETSC_LIBPETSC
	NAMES petsc libpetsc
	HINTS ${PETSC_LIBRARY_SEARCH_HINT}
	PATHS ${PETSC_ROOT_DIR}
	PATH_SUFFIXES lib lib/${PETSC_ARCH}
)

SET(PETSC_LIBVARS
	PETSC_LIBPETSC
	PETSC_LIBPETSCCONTRIB
	PETSC_LIBPETSCDM
	PETSC_LIBPETSCKSP
	PETSC_LIBPETSCMAT
	PETSC_LIBPETSCSNES
	PETSC_LIBPETSCTS
	PETSC_LIBPETSCVEC
	PETSC_LIBPETSC
)
SET(PETSC_LIBRARIES)

#add blas/lapack of petsc
IF (WITH_FORTRAN)
	FIND_LIBRARY(PETSC_LIBLAPACK
		NAMES 	flapack lapackgf lapackgf-3 lapack
		HINTS 	${PETSC_LIBRARY_SEARCH_HINT}
		PATHS	${PETSC_ROOT_DIR}
		PATH_SUFFIXES externalpackages/fblaslapack/${PETSC_ARCH}
	)
	LIST(APPEND PETSC_LIBVARS
		PETSC_LIBLAPACK
	)
ELSE (WITH_FORTRAN)
	FIND_LIBRARY(PETSC_LIBBLAS
		NAMES   f2cblas libf2cblas blas
		HINTS	${PETSC_LIBRARY_SEARCH_HINT}
		PATHS	${PETSC_ROOT_DIR}
		PATH_SUFFIXES externalpackages/f2cblaslapack/${PETSC_ARCH}
	)
	FIND_LIBRARY(PETSC_LIBLAPACK
		NAMES   f2clapack libf2clapack lapack
		HINTS	${PETSC_LIBRARY_SEARCH_HINT}
		PATHS	${PETSC_ROOT_DIR}
		PATH_SUFFIXES externalpackages/f2cblaslapack/${PETSC_ARCH}
	)
	LIST(APPEND PETSC_LIBVARS
		PETSC_LIBBLAS
		PETSC_LIBLAPACK
	)
ENDIF (WITH_FORTRAN)

#add MPI support
IF (WITH_MPI)
	FIND_PACKAGE(MPI REQUIRED)
	LIST(APPEND PETSC_LIBRARIES
		${MPI_LIBRARIES}
	)
ELSE (WITH_MPI)
	FIND_LIBRARY(PETSC_LIBMPIUNI
		NAMES mpiuni libmpiuni
		HINTS ${PETSC_LIBRARY_SEARCH_HINT}
		PATHS ${PETSC_ROOT_DIR}
		PATH_SUFFIXES lib lib/${PETSC_ARCH}
	)
	LIST(APPEND PETSC_LIBVARS
		PETSC_LIBMPIUNI
	)
ENDIF (WITH_MPI)

SET(PETSC_INCLUDE_DIRS
	${PETSC_ROOT_DIR}/include
	${PETSC_ROOT_DIR}/${PETSC_ARCH}/include
	${PETSC_ROOT_DIR}/bmake/${PETSC_ARCH}
)
IF (WITH_MPI)
	LIST(APPEND PETSC_INCLUDE_DIRS
		${MPI_INCLUDE_PATH}
	)
ELSE (WITH_MPI)
	LIST(APPEND PETSC_INCLUDE_DIRS
		${PETSC_ROOT_DIR}/include/mpiuni/
	)		
ENDIF (WITH_MPI)
MARK_AS_ADVANCED(
	${PETSC_LIBVARS}
)

# check if everything went fine
INCLUDE(FindPackageHandleStandardArgs)
IF(NOT PETSC_ROOT_DIR)
	MESSAGE(SEND_ERROR
		"PETSc has not been found. "
		"Please set PETSC_ROOT_DIR to the directory, "
		"where you have installed PETSc. "
		"Set PETSC_ARCH to the name of the library folder "
		"inside the lib directory. "
	)
ENDIF(NOT PETSC_ROOT_DIR)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(petsc DEFAULT_MSG
    PETSC_ROOT_DIR
	${PETSC_LIBVARS}
)

# determine values of each library variable and add to
# petsc library list
FOREACH(LIB ${PETSC_LIBVARS})
	LIST(APPEND PETSC_LIBRARIES
		${${LIB}}
	)
ENDFOREACH(LIB ${PETSC_LIBVARS})

MARK_AS_ADVANCED(
    PETSC_ROOT_DIR
	PETSC_ARCH
    PETSC_CONFIG_DIR
)

IF(COMMAND UNSET)
	UNSET(PETSC_LIBVARS)
ENDIF(COMMAND UNSET)
