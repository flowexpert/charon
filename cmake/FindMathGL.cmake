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

FIND_LIBRARY(MathGL_MGL_LIB NAMES mgl
	PATHS
	/usr/local/lib
	/usr/lib
)

FIND_LIBRARY(MathGL_MGL-WND_LIB NAMES mgl-wnd
	PATHS
	/usr/local/lib
	/usr/lib
)

IF(MathGL_MGL_LIB AND MathGL_MGL-WND_LIB)
	SET(MathGL_LIBS ${MathGL_MGL-WND_LIB} ${MathGL_MGL_LIB})
ENDIF(MathGL_MGL_LIB AND MathGL_MGL-WND_LIB)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MathGL DEFAULT_MSG MathGL_MGL_LIB MathGL_MGL-WND_LIB MathGL_INCLUDE_DIR)

mark_as_advanced(MathGL_MGL_LIB MathGL_MGL-WND_LIB MathGL_INCLUDE_DIR )


