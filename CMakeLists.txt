PROJECT(charon-core C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


SET(${PROJECT_NAME}_ROOT_DIR ${PROJECT_SOURCE_DIR})

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# read the current version from configVersion.h
INCLUDE(ReadVersion)
MESSAGE(STATUS "Configuring ${PROJECT_NAME} version ${${PROJECT_NAME}_VERSION}")

# Project folders and Debug Suffix
OPTION(USE_PROJECT_FOLDERS "Organize build targets in Folders (Only supported in MSVC Professional)" OFF)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ${USE_PROJECT_FOLDERS})
SET(CMAKE_DEBUG_POSTFIX _d)

# executables and dll files are moved to the same directory
IF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
ENDIF()
IF(NOT EXPORT_NAME)
	SET(EXPORT_NAME ${PROJECT_NAME}-export)
ENDIF()

INCLUDE(InitFlags)
INCLUDE(PluginMakros)
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
