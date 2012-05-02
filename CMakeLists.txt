PROJECT(charon-utils C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)
# HDF5 components interface and debug/release library handling
# is available/fixed since CMake 2.8.5 (broken in 2.8.4 and ealier)
SET(charon-utils_VERSION 0.1.6)

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

# set up documentation generation
FIND_PACKAGE(charon-core REQUIRED HINTS $ENV{CHARON_DIR})

FIND_PACKAGE(Doxygen QUIET)
FIND_PACKAGE(Vigra 1.7.1)
IF(NOT Vigra_FOUND)
	MESSAGE(STATUS "Set Vigra_DIR to point to the vigra lib directory")
ENDIF(NOT Vigra_FOUND)

FIND_PACKAGE(HDF5 COMPONENTS C HL)
FIND_PACKAGE(MPI)
FIND_PACKAGE(Qt4 4.6.0 COMPONENTS QtCore QtGui QtOpenGL QtXml)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE(Boost 1.40)

INCLUDE(Documentation)
INCLUDE(InitFlags)
OPTION(USE_CIMG "enable plugins using CImg" ON)
IF(USE_CIMG)
	INCLUDE(CImgConfig)
ENDIF()

MESSAGE(STATUS "Charon-utils library configuration:")
OPTION(USE_VIGRA "enable plugins using Vigra library" ${Vigra_FOUND})
OPTION(USE_HDF5 "enable plugins using HDF5 library" ${HDF5_FOUND})
OPTION(USE_MPI "enable plugins using MPI" ${MPI_FOUND})
OPTION(USE_QT "enable plugins using QT (needs Vigra)" ${QT_FOUND})
OPTION(USE_Boost "enable plugins using QT" ${Boost_FOUND})
MESSAGE(STATUS "    CImg         : ${USE_CIMG}")
MESSAGE(STATUS "    Vigra        : ${USE_VIGRA}")
MESSAGE(STATUS "    HDF5         : ${USE_HDF5}")
MESSAGE(STATUS "    MPI          : ${USE_MPI}")
MESSAGE(STATUS "    QT           : ${USE_QT}")
MESSAGE(STATUS "    Boost        : ${USE_Boost}")

IF(USE_CIMG)
	CImgLibInfo()
ENDIF()
INCLUDE(ReleaseSys)

# Qwt5 for plotting widgets
INCLUDE(FindQwt5)
# MathGL for quiver plots
INCLUDE(FindMathGL)

# Test suite
INCLUDE(CTest)

# Setup CMake config files for package finding
INCLUDE(GenConfig)
INCLUDE(InstallDlls)

ADD_SUBDIRECTORY(app)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

