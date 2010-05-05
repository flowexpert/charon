PROJECT(charon-utils C CXX)
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

# set up documentation generation
FIND_PACKAGE(charon-core REQUIRED)
FIND_PACKAGE(Doxygen QUIET)
FIND_PACKAGE(Vigra 1.7.0)
MESSAGE(STATUS "Charon-utils library configuration:")
OPTION(WITH_VIGRA "enable plugins using Vigra library" ${VIGRA_FOUND})
MESSAGE(STATUS "    Vigra        : ${WITH_VIGRA}")
INCLUDE(Documentation)
INCLUDE(InitFlags)
INCLUDE(CImgConfig)
CImgLibInfo()
INCLUDE(ReleaseSys)

# Test suite
OPTION(ENABLE_TEST "enable test suite" ON)
IF(ENABLE_TEST)
    ENABLE_TESTING()
	INCLUDE(CTest)
ENDIF(ENABLE_TEST)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
