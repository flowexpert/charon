#
# FindMathGL.cmake
#
# Copyright (C) 2012 Heidelberg Collaboratory for Image Processing
#
# author: Michael Baron <michael.baron@iwr.uni-heidelberg.de>
#
# This file is part of charon-free.
#

FIND_PATH(MathGL_INCLUDE_DIR NAMES mgl2/mgl.h
	PATHS
	/usr/local/include
	/usr/include
)
SET(MathGL_VERSION)
IF(MathGL_INCLUDE_DIR)
	FILE(READ ${MathGL_INCLUDE_DIR}/mgl2/define.h VERSION_FILE)
	STRING(REGEX MATCH "MGL_VER2[ \t\n]+([0-9\\.]+)" _TMP_VAR ${VERSION_FILE})
	SET(MathGL_VERSION ${CMAKE_MATCH_1})
	UNSET(_TMP_VAR)
ENDIF(MathGL_INCLUDE_DIR)
	MESSAGE(STATUS "Found MathGL Version: 2.${MathGL_VERSION}")


FIND_LIBRARY(MathGL_MGL_LIB NAMES mgl
	PATHS
	/usr/local/lib
	/usr/lib
)

FIND_LIBRARY(MathGL_MGL-QT_LIB NAMES mgl-qt
	PATHS
	/usr/local/lib
	/usr/lib
)

IF(MathGL_MGL_LIB AND MathGL_MGL-QT_LIB)
	SET(MathGL_LIBS ${MathGL_MGL_LIB} ${MathGL_MGL-QT_LIB})
ENDIF(MathGL_MGL_LIB AND MathGL_MGL-QT_LIB)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
		MathGL 
		REQUIRED_VARS MathGL_MGL_LIB MathGL_MGL-QT_LIB MathGL_INCLUDE_DIR
		VERSION_VAR MathGL_VERSION
)

mark_as_advanced(MathGL_MGL_LIB MathGL_MGL-QT_LIB MathGL_INCLUDE_DIR MathGL_VERSION)
