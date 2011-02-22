PROJECT(charon-core C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(${PROJECT_NAME}_VERSION 0.1.4)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# executables and dll files are moved to the same directory
IF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
ENDIF()

INCLUDE(InitFlags)

# set up documentation generation
FIND_PACKAGE(Doxygen QUIET)
INCLUDE(Documentation)

# CPack descriptions for package generation
INCLUDE(ReleaseSys)

# Test suite
CONFIGURE_FILE(CTestCustom.cmake.in ${BUILD_ROOT_PREFIX}CTestCustom.cmake @ONLY)
INCLUDE(CTest)

# Setup CMake config files for package finding
INCLUDE(GenConfig)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(app)
ADD_SUBDIRECTORY(test)
