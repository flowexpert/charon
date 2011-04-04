# CMake project file for @ModuleName@.
# This file has been set up by the charon TemplateGenerator.
# It may be modified to be adapted to your needs.
PROJECT(@ModuleName@)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# set by template generator options
SET(USE_CIMG @useCImg@)
SET(USE_VIGRA @useVigra@)

# required packages
FIND_PACKAGE(charon-core REQUIRED HINTS $ENV{CHARON_DIR})
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})

# Feel free to remove what you don't need.
# Unset variables default to an empty string,
# so the TARGET_LINK_LIBRARIES below won't link
# to CImg or Vigra if USE_... is not ON.
IF(USE_CIMG)
	FIND_PACKAGE(charon-utils REQUIRED HINTS $ENV{CHARON_DIR})
	INCLUDE(CImgConfig)
	INCLUDE_DIRECTORIES(${CIMG_INCLUDE_DIRS})
ENDIF(USE_CIMG)
IF(USE_VIGRA)
	FIND_PACKAGE(Vigra 1.7.1 REQUIRED)
	INCLUDE_DIRECTORIES(${Vigra_INCLUDE_DIRS})
	SET(Vigra_LIBRARIES vigraimpex)
ENDIF(USE_VIGRA)

INCLUDE(InitFlags)

# module generation
ADD_LIBRARY(@modulename@ SHARED
	@ModuleName@.cpp
	@ModuleName@.h
	@ModuleName@.hxx
)
TARGET_LINK_LIBRARIES(@modulename@
	charon-core
	${CIMG_LINK_LIBRARIES}
	${Vigra_LIBRARIES}
)
