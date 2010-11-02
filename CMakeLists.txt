PROJECT(charon-utils C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(charon-utils_VERSION 0.1.4)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# executables and dll files are moved to the same directory
IF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
ENDIF()

# set up documentation generation
FIND_PACKAGE(charon-core REQUIRED HINTS $ENV{CHARON_DIR})
FIND_PACKAGE(Doxygen QUIET)
FIND_PACKAGE(Vigra 1.7.0)
INCLUDE(Documentation)
INCLUDE(InitFlags)
INCLUDE(CImgConfig)

MESSAGE(STATUS "Charon-utils library configuration:")
OPTION(WITH_VIGRA "enable plugins using Vigra library" ${VIGRA_FOUND})
MESSAGE(STATUS "    Vigra        : ${WITH_VIGRA}")
CImgLibInfo()
INCLUDE(ReleaseSys)

# Test suite
INCLUDE(CTest)

# Setup CMake config files for package finding
INCLUDE(GenConfig)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
