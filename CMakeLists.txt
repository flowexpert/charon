PROJECT(charon C CXX)
# may work with 2.6; but only, if doxygen not found
# documentation install is marked as optional which works
# since cmake-2.8.0
# cmake 2.8.0 is available e.g. on *ubuntu lucid (10.4)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# executables and dll files are moved to the same directory
IF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
ENDIF()

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
MESSAGE(STATUS "Charon plugin configuration:")
OPTION(ENABLE_BRIGHTNESS "compile brightnessmodel plugins" ON)
MESSAGE(STATUS "    brightness m.: ${ENABLE_BRIGHTNESS}")
OPTION(ENABLE_MOTION "compile motionmodel plugins" ON)
MESSAGE(STATUS "    motion models: ${ENABLE_MOTION}")
OPTION(ENABLE_BLOCKMATCHING "compile blockmatching plugins" OFF)
MESSAGE(STATUS "    blockmatching: ${ENABLE_BLOCKMATCHING}")
OPTION(ENABLE_STENCILS "compile stencil pugins" ON)
MESSAGE(STATUS "    stencils     : ${ENABLE_STENCILS}")
OPTION(ENABLE_SOLVERS "compile solver pugins" ON)
MESSAGE(STATUS "    solvers      : ${ENABLE_SOLVERS}")
OPTION(ENABLE_ROBUSTNESS "compile solver pugins" OFF)
MESSAGE(STATUS "    robustness   : ${ENABLE_ROBUSTNESS}")

MESSAGE(STATUS "Charon library configuration:")
OPTION(WITH_PETSC "use PETSc library" ON)
MESSAGE(STATUS "    PETSc        : ${WITH_PETSC}")
IF(WITH_PETSC)
	FIND_PACKAGE(petsc REQUIRED)
ENDIF(WITH_PETSC)
CImgLibInfo()

# Test suite
OPTION(ENABLE_TEST "enable test suite" ON)
IF(ENABLE_TEST)
	ENABLE_TESTING()
	INCLUDE(CTest)
ENDIF(ENABLE_TEST)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
