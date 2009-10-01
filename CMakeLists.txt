PROJECT(tuchulcha C CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# finde QT4 with required options
SET(QT_MIN_VERSION 4.4.0)
FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTTEST TRUE)

# if changes of bug #0008542 have been incorporated,
# this is recommended:
#FIND_PACKAGE(Qt4 4.4.0
#	COMPONENTS QtCore QtGui QtWebkit QtSVG
#	REQUIRED
#)

# set up QT environment
INCLUDE(${QT_USE_FILE})

# determine project svn information if possible
# make sure to switch WITN_SVN off if not building from svn sources
FIND_PACKAGE(Subversion QUIET)
IF(Subversion_FOUND)
	OPTION(WITH_SVN "try to determine svn revision information" ON)
	IF(WITH_SVN)
		Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Project)
	ENDIF(WITH_SVN)
ENDIF(Subversion_FOUND)

FIND_PACKAGE(charon-core REQUIRED)
FIND_PACKAGE(charon-utils REQUIRED)

# executables and dll files are moved to the same directory
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

IF(WIN32)
    # Provide option to install/package needed dlls as well
    OPTION(INSTALL_DLLS "install/package needed (third-party) dlls" ON)
    INCLUDE(appicon)
ENDIF(WIN32)


IF(MSVC)
    # visual stiudio is supported since version 2.22
    FIND_PACKAGE(Graphviz 2.22 REQUIRED)
ELSE(MSVC)
    # other builds can use older versions
    FIND_PACKAGE(Graphviz 2.16 REQUIRED)
ENDIF(MSVC)

IF(UNIX)
	ADD_DEFINITIONS(-DUNIX)
ELSE(UNIX)
	ADD_DEFINITIONS(-DWINDOWS)
ENDIF(UNIX)
IF(MSVC)
	ADD_DEFINITIONS(/wd4290)
ENDIF(MSVC)

IF(APPLE)
	ADD_DEFINITIONS(-DQ_WS_MAC32)
ENDIF(APPLE)

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
ADD_SUBDIRECTORY (bin)

INCLUDE(InstallDlls)
