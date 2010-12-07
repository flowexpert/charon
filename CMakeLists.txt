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
FIND_PACKAGE(Vigra)
IF(NOT VIGRA_FOUND)
	MESSAGE(STATUS "Set Vigra_DIR to point to the vigra lib directory")
ENDIF(NOT VIGRA_FOUND)

FIND_PACKAGE(HDF5)
FIND_PACKAGE(MPI)
FIND_PACKAGE(Qt4 4.6.0 COMPONENTS QtCore QtGui QtOpenGL)

INCLUDE(Documentation)
INCLUDE(InitFlags)
INCLUDE(CImgConfig)

MESSAGE(STATUS "Charon-utils library configuration:")
OPTION(USE_VIGRA "enable plugins using Vigra library" ${Vigra_FOUND})
OPTION(USE_HDF5 "enable plugins using HDF5 library" ${HDF5_FOUND})
OPTION(USE_MPI "enable plugins using MPI" ${MPI_FOUND})
OPTION(USE_QT "enable plugins using QT" ${QT_FOUND})
MESSAGE(STATUS "    Vigra        : ${USE_VIGRA}")
MESSAGE(STATUS "    HDF5         : ${USE_HDF5}")
MESSAGE(STATUS "    MPI          : ${USE_MPI}")
MESSAGE(STATUS "    QT           : ${USE_QT}")

CImgLibInfo()
INCLUDE(ReleaseSys)

# Test suite
INCLUDE(CTest)

# Setup CMake config files for package finding
INCLUDE(GenConfig)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
