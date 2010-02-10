PROJECT(templategenerator)
SET(templategenerator_VERSION 0.1.3)
	
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# use own cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
INCLUDE(InitFlags)
FIND_PACKAGE(Doxygen)
INCLUDE(Documentation)

# CMAKE_VERSION missing in versions <cmake-2.6.3
IF(NOT CMAKE_VERSION)
	SET(CMAKE_VERSION "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
ENDIF(NOT CMAKE_VERSION)

# find QT4 with required options
IF(${CMAKE_VERSION} VERSION_LESS 2.8.0)
	# old interface (for compatibility reasons)
	SET(QT_MIN_VERSION 4.4.0)
	FIND_PACKAGE(Qt4 REQUIRED)
ELSE(${CMAKE_VERSION} VERSION_LESS 2.8.0)
	# new and intuitive FindQt4 interface
	FIND_PACKAGE(Qt4 4.4.0
		COMPONENTS QtCore QtGui
		REQUIRED
	)
ENDIF(${CMAKE_VERSION} VERSION_LESS 2.8.0)

# set up Qt environment (e.g. set QT_LIBRARIES)
INCLUDE(${QT_USE_FILE})

# recurse into subdirectories
ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (app)

INCLUDE(CPackConfig)
