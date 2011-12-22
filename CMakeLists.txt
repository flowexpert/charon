PROJECT(charon-core C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(${PROJECT_NAME}_VERSION 0.1.7)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET(CMAKE_DEBUG_POSTFIX _d)

# executables and dll files are moved to the same directory
IF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
ENDIF()
IF(NOT EXPORT_NAME)
	SET(EXPORT_NAME ${PROJECT_NAME}-export)
ENDIF()

INCLUDE(InitFlags)

# set up documentation generation
FIND_PACKAGE(Doxygen QUIET)
INCLUDE(Documentation)

# CPack descriptions for package generation
INCLUDE(ReleaseSys)

# Test suite
INCLUDE(CTest)

# Setup CMake config files for package finding
INCLUDE(GenConfig)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(app)
ADD_SUBDIRECTORY(test)
