PROJECT(charon-utils C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
# executables and dll files are moved to the same directory
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# set up documentation generation
FIND_PACKAGE(charon-core REQUIRED)
FIND_PACKAGE(Doxygen QUIET)
FIND_PACKAGE(Vigra 1.7.0)
IF(VIGRA_FOUND)
	OPTION(WITH_VIGRA "enable plugins using Vigra library" ON)
ELSE(VIGRA_FOUND)
	OPTION(WITH_VIGRA "enable plugins using Vigra library" OFF)
ENDIF(VIGRA_FOUND)
INCLUDE(Documentation)
INCLUDE(InitFlags)
INCLUDE(CImgConfig)
INCLUDE(ReleaseSys)

# Test suite
OPTION(ENABLE_TEST "enable test suite" ON)
IF(ENABLE_TEST)
    ENABLE_TESTING()
	INCLUDE(CTest)
ENDIF(ENABLE_TEST)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
