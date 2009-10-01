PROJECT(charon C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

FIND_PACKAGE(charon-core REQUIRED)
FIND_PACKAGE(charon-utils REQUIRED)

# set up documentation generation
FIND_PACKAGE(Doxygen QUIET)
INCLUDE(Documentation)

# CPack descriptions for package generation
INCLUDE(ReleaseSys)

# if off BrightnessModels and MotionModels won't be compiled
OPTION(COMPILE_BRIGHTNESS "compile brightnessmodel plugins" on)
OPTION(COMPILE_MOTION "compile motionmodel plugins" on)

# if off Blockmatching won't be compiled
OPTION(COMPILE_BLOCKMATCHING "compile blockmatching plugins" on)

OPTION(COMPILE_DERIVATIVES "compile derivative plugins" on)
OPTION(COMPILE_STENCILS "compile stencil pugins" on)
OPTION(COMPILE_SOLVERS "compile solver pugins" on)

OPTION(PETSC_USED "use PETSc library" on)

IF (COMPILE_SOLVERS AND PETSC_USED)
	SET(PETSC_ARCH win_x86_32_mpiuni)
	OPTION(PETSC_USES_MPI "does PETSc use an external MPI implementation?" off)
	OPTION(PETSC_USES_FORTRAN "was fortran used for PETSc-compilation?" off)
	FIND_PACKAGE(petsc REQUIRED)
ENDIF (COMPILE_SOLVERS AND PETSC_USED)

# Test suite
OPTION(ENABLE_TEST "enable test suite" ON)
IF(ENABLE_TEST)
    ENABLE_TESTING()
ENDIF(ENABLE_TEST)

IF(WIN32)
	SET(LIBFILE_EXT lib)
	ADD_DEFINITIONS(-DWINDOWS)
	IF(MSVC)
		ADD_DEFINITIONS(/D "MSVC" /wd4251 /wd4290)
	ENDIF(MSVC)
	IF(MINGW)
		ADD_DEFINITIONS(-DMINGW)
	ENDIF(MINGW)
ENDIF(WIN32)
IF(UNIX)
	SET(LIBFILE_EXT .a)
	FIND_PACKAGE(X11)
	ADD_DEFINITIONS(-DUNIX)
	IF(APPLE)
		ADD_DEFINITIONS(-DAPPLE)
	ENDIF(APPLE)
ENDIF(UNIX)

# executables and dll files are moved to the same directory
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(bin)
