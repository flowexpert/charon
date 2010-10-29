# - Find petsc
#
# Usage:
#  FIND_PACKAGE(petsc[ REQUIRED][ QUIET])
#  INCLUDE_DIRECTORIES(${PETSC_INCLUDE_DIRS})
#  ADD_EXECUTABLE(bla main.cpp)
#  TARGET_LINK_LIBRARIES(bla ${PETSC_LIBRARIES})
#
# This sets the variables:
#  PETSC_ROOT_DIR            petsc installation directory
#  PETSC_INCLUDE_DIRS        petsc include directories
#  PETSC_LIBRARIES           petsc libraries

# search for header files
FIND_PATH(PETSC_ROOT_DIR
	NAMES           include/petscksp.h
	                include/petsc/petscksp.h
	HINTS           $ENV{PETSC_DIR}
	PATHS           /usr
	                /etc/alternatives/petsc
	                /usr/local
	                /opt/petsc
	                /opt
	DOC             "petsc root directory"
)

# as multiple compiled version of petsc with different architectures
# can reside in the root directory, the architecture has to be specified
# manually using PETSC_ARCH
# with both the ROOT_DIR and ARCH every needed path can be generated
#
# On debian based systems, the petsc package provides an one-line-script
# indicating the petsc arch variable used during package creation
# The result of this indicator may be overridden by the PETSC_ARCH
# environmental variable (or by -DPETSC_ARCH=<arch>)
IF(NOT PETSC_ARCH)
	IF(WIN32)
		SET(PETSC_ARCH "win_x86_32_mpiuni")
	ELSE()
		SET(PETSC_ARCH "linux-gnu-cxx-debug")
	ENDIF()

	# use value of debian petscarch indicator (if present)
	FIND_PROGRAM(PETSC_ARCH_INDICATOR
		NAMES petscarch
		PATHS /etc/alternatives
		DOC "debian petsc arch indicator"
	)
	IF(PETSC_ARCH_INDICATOR)
		EXECUTE_PROCESS(
			COMMAND ${PETSC_ARCH_INDICATOR}
			OUTPUT_VARIABLE PETSC_ARCH
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
	ENDIF(PETSC_ARCH_INDICATOR)

	# use value of envirionmental variable if set
	IF(NOT "$ENV{PETSC_ARCH}" STREQUAL "")
		SET(  "$ENV{PETSC_ARCH}")
	ENDIF(NOT "$ENV{PETSC_ARCH}" STREQUAL "")

	SET(PETSC_ARCH "${PETSC_ARCH}"
		CACHE STRING "select petsc architecture"
		FORCE
	)
ENDIF(NOT PETSC_ARCH)

FIND_PATH(PETSC_INCLUDE_DIR
	NAMES           petsc.h
	                petscksp.h
	HINTS           $ENV{PETSC_DIR}
	                ${PETSC_ROOT_DIR}
	PATH_SUFFIXES   include
	                include/petsc
	                petsc/include
	DOC             "petsc headers"
)
FIND_PATH(PETSC_ARCH_INCLUDE_DIR
	NAMES           petscconf.h
	HINTS           $ENV{PETSC_DIR}
	                ${PETSC_ROOT_DIR}
	PATH_SUFFIXES   ${PETSC_ARCH}/include
	                include/${PETSC_ARCH}
	                include/petsc/${PETSC_ARCH}
	                bmake/${PETSC_ARCH}
	                include
	                include/petsc
	DOC             "architecture specific headers"
)
SET(PETSC_INCLUDE_DIRS
	${PETSC_INCLUDE_DIR}
	${PETSC_ARCH_INCLUDE_DIR}
)

OPTION(WITH_MPI "does PETSc use an external MPI implementation?" ${UNIX})
OPTION(WITH_FORTRAN "was fortran used for PETSc-compilation?" ${UNIX})
OPTION(PETSC_SINGLE_LIBRARY
	"has PETSc been compiled with --with-single-library=1?" ${UNIX})

SET(PETSC_LIBRARY_SEARCH_HINT
	${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}
	${PETSC_ROOT_DIR}/${PETSC_ARCH}/lib
)

# cygwin generated msvc libraries are names using the unix
# naming scheme, therefore we have to add the names with "lib" prefix
SET(PETSC_LIBVARS)
FIND_LIBRARY(PETSC_LIBPETSC
	NAMES petsc libpetsc
	HINTS ${PETSC_LIBRARY_SEARCH_HINT}
	PATHS ${PETSC_ROOT_DIR}
	PATH_SUFFIXES lib lib/${PETSC_ARCH}
)
LIST(APPEND PETSC_LIBVARS PETSC_LIBPETSC)
IF(NOT PETSC_SINGLE_LIBRARY)
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
	LIST(APPEND PETSC_LIBVARS
		PETSC_LIBPETSCCONTRIB
		PETSC_LIBPETSCDM
		PETSC_LIBPETSCKSP
		PETSC_LIBPETSCMAT
		PETSC_LIBPETSCSNES
		PETSC_LIBPETSCTS
		PETSC_LIBPETSCVEC
	)
ENDIF(NOT PETSC_SINGLE_LIBRARY)

SET(PETSC_LIBRARIES)

#add blas/lapack of petsc
IF (LAPACK_FOUND)
	LIST(APPEND PETSC_LIBVARS
		BLAS_LIBRARIES
		LAPACK_LIBRARIES
	)
ELSE (LAPACK_FOUND)
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
ENDIF (LAPACK_FOUND)

#add MPI support
IF (WITH_MPI)
	FIND_PACKAGE(MPI REQUIRED)
	LIST(APPEND PETSC_LIBRARIES
		${MPI_LIBRARIES}
	)
	LIST(APPEND PETSC_INCLUDE_DIRS
		${MPI_INCLUDE_PATH}
	)
ELSE (WITH_MPI)
	OPTION(PETSC_NEEDS_LIBMPIUNI "require mpiuni library" ON)
	IF(PETSC_NEEDS_LIBMPIUNI)
		FIND_LIBRARY(PETSC_LIBMPIUNI
			NAMES mpiuni libmpiuni
			HINTS ${PETSC_LIBRARY_SEARCH_HINT}
			PATHS ${PETSC_ROOT_DIR}
			PATH_SUFFIXES lib lib/${PETSC_ARCH}
		)
		LIST(APPEND PETSC_LIBVARS
			PETSC_LIBMPIUNI
		)
	ENDIF(PETSC_NEEDS_LIBMPIUNI)
	LIST(APPEND PETSC_INCLUDE_DIRS
		${PETSC_INCLUDE_DIR}/mpiuni/
	)
ENDIF (WITH_MPI)


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
	PETSC_LIBPETSC
	PETSC_ROOT_DIR
	PETSC_INCLUDE_DIR
	PETSC_ARCH_INCLUDE_DIR
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
	PETSC_INCLUDE_DIR
	PETSC_ARCH_INCLUDE_DIR
	PETSC_NEEDS_LIBMPIUNI
	PETSC_SINGLE_LIBRARY
	${PETSC_LIBVARS}
)

UNSET(PETSC_LIBVARS)
