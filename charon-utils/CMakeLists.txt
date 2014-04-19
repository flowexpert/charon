PROJECT(charon-utils)
# CMake 2.8.9 is required for the CMakePackageConfigHelpers module
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)
SET(charon-utils_VERSION 0.2.1)

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
FIND_PACKAGE(charon-core 0.2.4 REQUIRED HINTS $ENV{CHARON_DIR})

FIND_PACKAGE(Doxygen QUIET)
IF(NOT DEFINED USE_VIGRA OR USE_VIGRA)
	FIND_PACKAGE(Vigra 1.7.1)
	IF(NOT Vigra_FOUND)
		MESSAGE(STATUS "Set Vigra_DIR to point to the vigra lib directory")
	ENDIF(NOT Vigra_FOUND)
ENDIF()
FIND_PACKAGE(HDF5 COMPONENTS C HL)
IF(NOT DEFINED USE_MPI OR USE_MPI)
	# find script fails if fortran not available
	FIND_PACKAGE(MPI)
ELSE()
	# avoid empty expansion for macros below
	SET(MPI_FOUND FALSE)
ENDIF()
FIND_PACKAGE(Qt4 4.6.0 COMPONENTS QtCore QtGui QtOpenGL QtXml)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE(Boost 1.40)

INCLUDE(Documentation)
INCLUDE(InitFlags)
OPTION(USE_CIMG "enable plugins using CImg" ON)
IF(USE_CIMG)
	INCLUDE(CImgConfig)
ENDIF()

# Qwt5 for plotting widgets
#INCLUDE(FindQwt5)

# MathGL for quiver plots
# MathGL changed some include file names after 2.0
# the mayor version is ommitted from the version string (so we need at last 2.1.0)
FIND_PACKAGE(MathGL 1.0)

INCLUDE(CMakeDependentOption)
MESSAGE(STATUS "Charon-utils library configuration:")
OPTION(USE_VIGRA "enable plugins using Vigra library" ${Vigra_FOUND})
OPTION(USE_HDF5 "enable plugins using HDF5 library" ${HDF5_FOUND})
CMAKE_DEPENDENT_OPTION( # need MPI for parallel hdf5
	USE_MPI "enable plugins using MPI" ${MPI_FOUND}
	"NOT USE_HDF5 OR NOT HDF5_IS_PARALLEL" ON)
OPTION(USE_QT "enable plugins using QT (needs Vigra)" ${QT_FOUND})
OPTION(USE_Boost "enable plugins using QT" ${Boost_FOUND})
OPTION(USE_MathGL "enable plugins using MathGL" ${MathGL_FOUND})
#OPTION(USE_Qwt5 "enable plugins using MathGL" ${Qwt5_Qt4_FOUND})
MESSAGE(STATUS "    CImg         : ${USE_CIMG}")
MESSAGE(STATUS "    Vigra        : ${USE_VIGRA}")
MESSAGE(STATUS "    HDF5         : ${USE_HDF5}")
MESSAGE(STATUS "    MPI          : ${USE_MPI}")
MESSAGE(STATUS "    QT           : ${USE_QT}")
MESSAGE(STATUS "    Boost        : ${USE_Boost}")
MESSAGE(STATUS "    MathGL       : ${USE_MathGL}")
#MESSAGE(STATUS "    Qwt5         : ${USE_Qwt5}")

IF(USE_CIMG)
	CImgLibInfo()
ENDIF()
INCLUDE(ReleaseSys)

# Test suite
INCLUDE(CTest)

# Setup CMake config files for package finding
INCLUDE(GenConfig)
INCLUDE(InstallDlls)

ADD_SUBDIRECTORY(app)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

