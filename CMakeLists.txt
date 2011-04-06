PROJECT(tuchulcha C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(tuchulcha_VERSION 0.2.1)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

IF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
ENDIF()
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

FIND_PACKAGE(Qt4 4.4.0
	COMPONENTS QtCore QtGui QtSVG QtTest QtWebkit
	REQUIRED
)
IF(APPLE)
	ADD_DEFINITIONS(-DQ_WS_MAC32)
ENDIF(APPLE)
INCLUDE(${QT_USE_FILE})

# determine project svn information if possible
# make sure to switch USE_SVN off if not building from svn sources
FIND_PACKAGE(Subversion)
IF(Subversion_FOUND)
	OPTION(USE_SVN "try to determine svn revision information" ON)
	IF(USE_SVN)
		IF(NOT tuchulcha_SVN_DIR)
			SET(tuchulcha_SVN_DIR "${PROJECT_SOURCE_DIR}")
		ENDIF()
		SET(tuchulcha_SVN_DIR "${tuchulcha_SVN_DIR}"
			CACHE PATH "Directory to query SVN information from" FORCE)
		Subversion_WC_INFO(${tuchulcha_SVN_DIR} Tuchulcha_SVN)
	ENDIF(USE_SVN)
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
INCLUDE(CTest)

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
