#
# FindMathGL.cmake
#
# Copyright (C) 2012 Heidelberg Collaboratory for Image Processing
#
# author: Michael Baron <michael.baron@iwr.uni-heidelberg.de>
#
# This file is part of charon-free.
#

FIND_PATH(MATHGL_INCLUDE_DIR NAMES mgl2/mgl.h
	PATHS
	/usr/local/include
	/usr/include
)

FIND_LIBRARY(MATHGL_LIB NAMES mgl
	PATHS
	/usr/local/lib
	/usr/lib
)

FIND_LIBRARY(MATHGLWND_LIB NAMES mgl-wnd
	PATHS
	/usr/local/lib
	/usr/lib
)

IF(MATHGL_INCLUDE_DIR AND MATHGL_LIB AND MATHGLWND_LIB)
	SET(MATHGL_FOUND TRUE)
	SET(MATHGL_LIBS
		${MATHGL_LIB}
		${MATHGLWND_LIB}
	)
	MESSAGE( STATUS "Found MathGL library, version 2.0" )
ENDIF(MATHGL_INCLUDE_DIR AND MATHGL_LIB AND MATHGLWND_LIB)

