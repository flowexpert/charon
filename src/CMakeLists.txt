INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(PixelSelections)
INCLUDE_DIRECTORIES(Incrementors)
INCLUDE_DIRECTORIES(ObjectiveFunctions)
INCLUDE_DIRECTORIES(SurfaceAnalysis)

IF(MSVC)
	ADD_DEFINITIONS(/DHANDLE_DLL)
ENDIF(MSVC)

# plugins
ADD_LIBRARY(channelconverter SHARED ChannelConverter.cpp)
SET(Plugins
	channelconverter
)

IF (ENABLE_STENCILS)
	ADD_LIBRARY(gbcce SHARED Stencils/Gbcce.cpp)
	ADD_LIBRARY(l2norm SHARED Stencils/L2Norm.cpp)
	LIST(APPEND Plugins
		gbcce
		l2norm
	)
ENDIF (ENABLE_STENCILS)

IF(ENABLE_SOLVERS AND WITH_PETSC)
	INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
	ADD_LIBRARY(petscsolver SHARED Solvers/PetscSolver.cpp)
	TARGET_LINK_LIBRARIES(petscsolver ${PETSC_LIBRARIES})
	LIST(APPEND Plugins
		petscsolver
	)
ENDIF(ENABLE_SOLVERS AND WITH_PETSC)

IF (COMPILE_BLOCKMATCHING)
	ADD_LIBRARY(incrementorparameter SHARED IncrementorParameter.cpp)
	ADD_LIBRARY(incrementorcountup SHARED Incrementors/IncrementorCountUp.cpp)
	TARGET_LINK_LIBRARIES(incrementorcountup incrementorparameter)
	ADD_LIBRARY(objectivefunctioncomparing SHARED ObjectiveFunctions/ObjectiveFunctionComparing.cpp)
	ADD_LIBRARY(listedpixelselection SHARED PixelSelections/ListedPixelSelection.cpp)
	TARGET_LINK_LIBRARIES(listedpixelselection roi)
	ADD_LIBRARY(surfaceanalysisminchange SHARED SurfaceAnalysis/SurfaceAnalysisMinChange.cpp)
	TARGET_LINK_LIBRARIES(surfaceanalysisminchange incrementorparameter)
	ADD_LIBRARY(blockmatchingliacs SHARED BlockMatching/BlockMatchingLIACS.cpp)
	TARGET_LINK_LIBRARIES(blockmatchingliacs incrementorcountup)
	LIST(APPEND Plugins
		incrementorparameter	
		incrementorcountup
		objectivefunctioncomparing
		listedpixelselection
		surfaceanalysisminchange
		blockmatchingliacs
	)
ENDIF (COMPILE_BLOCKMATCHING)
IF (COMPILE_BRIGHTNESS)
	ADD_LIBRARY(brightnessmodels_constant SHARED BrightnessModels/Constant.cpp)
#	ADD_LIBRARY(brightnessmodels_diffusion SHARED BrightnessModels/Diffusion.cpp)
#	ADD_LIBRARY(brightnessmodels_exponential SHARED BrightnessModels/Exponential.cpp)
#	ADD_LIBRARY(brightnessmodels_movingillumination SHARED BrightnessModels/MovingIllumination.cpp)
	LIST(APPEND Plugins
		brightnessmodels_constant
#		brightnessmodels_diffusion
#		brightnessmodels_exponential
#		brightnessmodels_movingillumination
	)
ENDIF (COMPILE_BRIGHTNESS)
IF (COMPILE_MOTION)
#	ADD_LIBRARY(motionmodels_localaffiness SHARED MotionModels/LocalAffiness.cpp FlowFunctor.cpp)
	ADD_LIBRARY(motionmodels_localconstant SHARED MotionModels/LocalConstant.cpp FlowFunctor.cpp)
#	ADD_LIBRARY(motionmodels_localplanarity SHARED MotionModels/LocalPlanarity.cpp FlowFunctor.cpp)
#	ADD_LIBRARY(motionmodels_localrotation SHARED MotionModels/LocalRotation.cpp FlowFunctor.cpp)
#	ADD_LIBRARY(motionmodels_localstretch SHARED MotionModels/LocalStretch.cpp FlowFunctor.cpp)
	LIST(APPEND Plugins
#		motionmodels_localaffiness
		motionmodels_localconstant
#		motionmodels_localplanarity 
#		motionmodels_localrotation
#		motionmodels_localstretch
	)
ENDIF (COMPILE_MOTION)


FOREACH(X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core)
	IF(UNIX)
		TARGET_LINK_LIBRARIES(${X} pthread ${X11_LIBRARIES})
	ENDIF(UNIX)
	IF(WIN32)
		TARGET_LINK_LIBRARIES(${X} gdi32)
	ENDIF(WIN32)
ENDFOREACH(X)

# install libraries/plugins
IF(NOT ${CMAKE_INSTALL_PREFIX} STREQUAL ${CHARON_UTILS_ROOT_DIR})
	MESSAGE(STATUS	"Warning: Plugins will NOT be installed in standart "
					"plugin directory!")
	MESSAGE(STATUS	"Set CMAKE_INSTALL_PREFIX to ${CHARON_UTILS_ROOT_DIR} "
					"to install into global plugin path.")
ENDIF(NOT ${CMAKE_INSTALL_PREFIX} STREQUAL ${CHARON_UTILS_ROOT_DIR})
SET_TARGET_PROPERTIES(${Plugins}
	PROPERTIES INSTALL_RPATH
	${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins:${CHARON_CORE_ROOT_DIR}/lib${LIB_SUFFIX}
)

INSTALL(
    TARGETS      	${Plugins}
    EXPORT      	${PROJECT_NAME}
    RUNTIME      	DESTINATION bin
    LIBRARY      	DESTINATION lib${LIB_SUFFIX}/charon-plugins
    ARCHIVE      	DESTINATION lib${LIB_SUFFIX}/charon-plugins
    COMPONENT    	libraries
)

# install header files to include directory
FILE(GLOB_RECURSE HEADER_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.h *.hxx
)
INSTALL(
    FILES       ${HEADER_FILES}
    DESTINATION include/${PROJECT_NAME}
    COMPONENT   headers
)

INSTALL(
    EXPORT       ${PROJECT_NAME}
    DESTINATION  include/${PROJECT_NAME}
    COMPONENT    libraries
)
