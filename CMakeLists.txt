PROJECT(charon C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(charon_VERSION 0.1.5)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
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

FIND_PACKAGE(charon-core REQUIRED HINTS $ENV{CHARON_DIR})
FIND_PACKAGE(charon-utils REQUIRED HINTS $ENV{CHARON_DIR})
INCLUDE(InitFlags)
INCLUDE(CImgConfig)

# set up documentation generation
FIND_PACKAGE(Doxygen QUIET)
INCLUDE(Documentation)

# CPack descriptions for package generation
INCLUDE(ReleaseSys)

# Find and use PETSc
# This is a workaround, because the Findpetsc.cmake script throws an
# error when it doesn't find PETSc, instead of failing quietly.
FIND_PACKAGE(PETSC)
OPTION(USE_PETSC "enable plugins using PETSc" ${PETSC_FOUND})

# some modules in charon-flow like Qt4 support
# like SimpleIteratorRemoteControl
FIND_PACKAGE(Qt4 4.6.0 COMPONENTS QtCore QtGui)
INCLUDE(${QT_USE_FILE})

# Test suite
INCLUDE(CTest)

# Print infos about found packages
INCLUDE(Features)
MESSAGE(STATUS "Lib configuration:")
CImgLibInfo()
MESSAGE(STATUS "    PETSc        : ${USE_PETSC}")

# Setup CMake config files for package finding
INCLUDE(GenConfig)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
