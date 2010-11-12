PROJECT(tuchulcha C CXX)
# may work with 2.6; but only, if doxygen not found
# documentation install is marked as optional which works
# since cmake-2.8.0
# cmake 2.8.0 is available e.g. on *ubuntu lucid (10.4)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(tuchulcha_VERSION 0.2.1)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# executables and dll files are moved to the same directory
IF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
ENDIF()

# CMAKE_VERSION missing in versions <cmake-2.6.3
IF(NOT CMAKE_VERSION)
	SET(CMAKE_VERSION "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
ENDIF(NOT CMAKE_VERSION)

# find QT4 with required options using the
# new and intuitive FindQt4 interface
FIND_PACKAGE(Qt4 4.4.0
	COMPONENTS QtCore QtGui QtSVG QtTest QtWebkit
	REQUIRED
)
# set up Qt environment (e.g. set QT_LIBRARIES)
INCLUDE(${QT_USE_FILE})
IF(APPLE)
	ADD_DEFINITIONS(-DQ_WS_MAC32)
ENDIF(APPLE)

# determine project svn information if possible
# make sure to switch WITN_SVN off if not building from svn sources
FIND_PACKAGE(Subversion)
IF(Subversion_FOUND)
	OPTION(WITH_SVN "try to determine svn revision information" ON)
	IF(WITH_SVN)
		IF(NOT tuchulcha_SVN_DIR)
			SET(tuchulcha_SVN_DIR "${PROJECT_SOURCE_DIR}")
		ENDIF()
		SET(tuchulcha_SVN_DIR "${tuchulcha_SVN_DIR}"
			CACHE PATH "Directory to query SVN information from" FORCE)
		Subversion_WC_INFO(${tuchulcha_SVN_DIR} Tuchulcha_SVN)
	ENDIF(WITH_SVN)
ENDIF(Subversion_FOUND)

# Possibility to add additional build info
SET(tuchulcha_BUILD_INFO "${tuchulcha_BUILD_INFO}"
	CACHE STRING "additional build information (optional)" FORCE)

FIND_PACKAGE(charon-core 0.1.4 REQUIRED HINTS $ENV{CHARON_DIR})
INCLUDE(InitFlags)

IF(WIN32)
	# Provide option to install/package needed dlls as well
	OPTION(INSTALL_DLLS "install/package needed (third-party) dlls" OFF)
	INCLUDE(appicon)
ENDIF(WIN32)

FIND_PACKAGE(Graphviz 2.20 REQUIRED)

# set up documentation generation
FIND_PACKAGE(Doxygen)
INCLUDE(Documentation)

# CPack descriptions for package generation
INCLUDE(ReleaseSys)

# Test suite
OPTION(ENABLE_TEST "enabling test suite" ON)
IF(ENABLE_TEST)
	ENABLE_TESTING()
	INCLUDE(CTest)
ENDIF(ENABLE_TEST)

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# recurse into subdirectories
ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (test)
ADD_SUBDIRECTORY (app)

INCLUDE(InstallDlls)
INCLUDE(Features)
