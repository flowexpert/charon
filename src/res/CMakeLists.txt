# CMake project file for @ModuleName@.
# This file has been set up by the charon TemplateGenerator.
# It may be modified to be adapted to your needs.
#
# The last line (add additional modules here) serves as
# mark where the TemplateGenerator inserts new module
# code if requested, so please do not edit this line.
# new module code is inserted just above this mark.
PROJECT(@ModuleName@)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Template generator options.
# This is not changed later, i.e. if new modules are
# added. If the new modules need CImg/Vigra and this
# flag was disabled on first setup, please activate
# it by hand.
SET(USE_CIMG  @useCImg@)
SET(USE_VIGRA @useVigra@)

# required packages
FIND_PACKAGE(charon-core REQUIRED HINTS $ENV{CHARON_DIR})
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
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

SET(Plugins) # this list contains all plugin targets

# module generation
ADD_LIBRARY(@modulename@ SHARED
	@ModuleName@.cpp
	@ModuleName@.h
@ModHxx@)
LIST(APPEND Plugins @modulename@)

# add additional modules here


# common linking and install target
LIST(REMOVE_DUPLICATES Plugins)
FOREACH(Pl ${Plugins})
	TARGET_LINK_LIBRARIES(${Pl}
		charon-core
		${CIMG_LINK_LIBRARIES}
		${Vigra_LIBRARIES}
	)
ENDFOREACH(Pl)
SET_TARGET_PROPERTIES(${Plugins} PROPERTIES
	INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins
)
INSTALL(
	TARGETS          ${Plugins}
	RUNTIME          DESTINATION bin${BIN_SUFFIX}
	LIBRARY          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	ARCHIVE          DESTINATION lib${LIB_SUFFIX}/charon-plugins
)
