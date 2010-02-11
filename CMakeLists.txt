PROJECT(charon C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

FIND_PACKAGE(charon-core REQUIRED)
FIND_PACKAGE(charon-utils REQUIRED)
INCLUDE(InitFlags)
INCLUDE(CImgConfig)

# set up documentation generation
FIND_PACKAGE(Doxygen QUIET)
INCLUDE(Documentation)

# CPack descriptions for package generation
INCLUDE(ReleaseSys)

# select wich plugins to compile
OPTION(ENABLE_BRIGHTNESS "compile brightnessmodel plugins" ON)
OPTION(ENABLE_MOTION "compile motionmodel plugins" ON)
OPTION(ENABLE_BLOCKMATCHING "compile blockmatching plugins" ON)
OPTION(ENABLE_STENCILS "compile stencil pugins" ON)
OPTION(ENABLE_SOLVERS "compile solver pugins" ON)

OPTION(WITH_PETSC "use PETSc library" ON)
IF(WITH_PETSC)
	FIND_PACKAGE(petsc REQUIRED)
ENDIF(WITH_PETSC)

# Test suite
OPTION(ENABLE_TEST "enable test suite" ON)
IF(ENABLE_TEST)
	ENABLE_TESTING()
ENDIF(ENABLE_TEST)

# executables and dll files are moved to the same directory
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
