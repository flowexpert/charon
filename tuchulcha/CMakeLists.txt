PROJECT(tuchulcha CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(tuchulcha_VERSION 0.10.1)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

IF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
ENDIF()
OPTION(USE_PROJECT_FOLDERS "Organize build targets in Folders (Only supported in MSVC Professional)" OFF)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ${USE_PROJECT_FOLDERS})
SET(CMAKE_DEBUG_POSTFIX _d)

FIND_PACKAGE(Qt4 4.4.0 COMPONENTS QtCore QtGui REQUIRED)
IF(APPLE)
	ADD_DEFINITIONS(-DQ_WS_MAC32)
ENDIF(APPLE)
INCLUDE(${QT_USE_FILE})

INCLUDE(FeatureSummary)
OPTION(USE_ASSISTANT "Use Qt Assistant for Tuchulcha help display" OFF)
ADD_FEATURE_INFO(USE_ASSISTANT USE_ASSISTANT "Qt Assistant based Help Viewer")
IF(WIN32)
	OPTION(USE_HTMLHELP "Use Htmlhelp for Tuchulcha help display" OFF)
	ADD_FEATURE_INFO(USE_HTMLHELP USE_HTMLHELP "Htmlhelp based Help Viewer")
ENDIF()

FIND_PACKAGE(charon-core 0.2.6 REQUIRED HINTS $ENV{CHARON_DIR})
INCLUDE(InitFlags)

IF(WIN32)
	# Provide option to install/package needed dlls as well
	OPTION(INSTALL_DLLS "install/package needed (third-party) dlls" OFF)
	INCLUDE(appicon)
ENDIF(WIN32)

# Test suite
INCLUDE(CTest)
# try to query VCS revision information
INCLUDE(RevInfo)
# project config file
INCLUDE(GenConfig)

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# recurse into subdirectories
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(app)

# set up documentation generation
INCLUDE(Documentation)
INCLUDE(InstallDlls)

FEATURE_SUMMARY(WHAT ALL)
