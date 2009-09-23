INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(PixelSelections)
INCLUDE_DIRECTORIES(Incrementors)
INCLUDE_DIRECTORIES(ObjectiveFunctions)
INCLUDE_DIRECTORIES(SurfaceAnalysis)

IF(COMPILE_SOLVERS)
	INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
ENDIF(COMPILE_SOLVERS)

ADD_DEFINITIONS(/DHANDLE_DLL)

#Create shared libraries
IF (COMPILE_DERIVATIVES)
	ADD_LIBRARY(bin2 SHARED Derivatives/bin2.cpp)
	ADD_LIBRARY(sobel SHARED Derivatives/Sobel.cpp)
ENDIF (COMPILE_DERIVATIVES)
IF (COMPILE_STENCILS)
	ADD_LIBRARY(gbcce SHARED Stencils/Gbcce.cpp)
	ADD_LIBRARY(l2norm SHARED Stencils/L2Norm.cpp)
ENDIF (COMPILE_STENCILS)
IF (COMPILE_SOLVERS)
	ADD_LIBRARY(petscsolver SHARED Solvers/PetscSolver.cpp)
	TARGET_LINK_LIBRARIES(petscsolver	charon-core
						${PETSC_CONFIG_DIR}/lib/libf2cblas.a
						${PETSC_CONFIG_DIR}/lib/libf2clapack.a
						${PETSC_CONFIG_DIR}/lib/libmpiuni.a
						${PETSC_CONFIG_DIR}/lib/libpetsc.a
						# ${PETSC_CONFIG_DIR}/lib/libpetsccontrib.a
						${PETSC_CONFIG_DIR}/lib/libpetscdm.a
						${PETSC_CONFIG_DIR}/lib/libpetscksp.a
						${PETSC_CONFIG_DIR}/lib/libpetscmat.a
						# ${PETSC_CONFIG_DIR}/lib/libpetscsnes.a
						# ${PETSC_CONFIG_DIR}/lib/libpetscts.a
						${PETSC_CONFIG_DIR}/lib/libpetscvec.a)
ENDIF (COMPILE_SOLVERS)
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
ENDIF (COMPILE_BLOCKMATCHING)
IF (COMPILE_BRIGHTNESS)
	ADD_LIBRARY(brightnessmodels_constant SHARED BrightnessModels/Constant.cpp)
#	ADD_LIBRARY(brightnessmodels_diffusion SHARED BrightnessModels/Diffusion.cpp)
#	ADD_LIBRARY(brightnessmodels_exponential SHARED BrightnessModels/Exponential.cpp)
#	ADD_LIBRARY(brightnessmodels_movingillumination SHARED BrightnessModels/MovingIllumination.cpp)
ENDIF (COMPILE_BRIGHTNESS)
IF (COMPILE_MOTION)
#	ADD_LIBRARY(motionmodels_localaffiness SHARED MotionModels/LocalAffiness.cpp FlowFunctor.cpp)
	ADD_LIBRARY(motionmodels_localconstant SHARED MotionModels/LocalConstant.cpp FlowFunctor.cpp)
#	ADD_LIBRARY(motionmodels_localplanarity SHARED MotionModels/LocalPlanarity.cpp FlowFunctor.cpp)
#	ADD_LIBRARY(motionmodels_localrotation SHARED MotionModels/LocalRotation.cpp FlowFunctor.cpp)
#	ADD_LIBRARY(motionmodels_localstretch SHARED MotionModels/LocalStretch.cpp FlowFunctor.cpp)
ENDIF (COMPILE_MOTION)

SET(Plugins)
IF (COMPILE_DERIVATIVES)
SET(Plugins
	${Plugins}
	bin2
	sobel
)
ENDIF (COMPILE_DERIVATIVES)

IF (COMPILE_STENCILS)
SET(Plugins
	${Plugins}
	gbcce
	l2norm
)
ENDIF (COMPILE_STENCILS)

IF (COMPILE_SOLVERS)
SET(Plugins
	${Plugins}
	petscsolver
)
ENDIF (COMPILE_SOLVERS)

IF (COMPILE_BLOCKMATCHING)
SET(Plugins
	${Plugins}
	incrementorparameter	
	incrementorcountup
	objectivefunctioncomparing
	listedpixelselection
	surfaceanalysisminchange
	blockmatchingliacs
)
ENDIF (COMPILE_BLOCKMATCHING)

IF (COMPILE_BRIGHTNESS)
SET(Plugins
	${Plugins}
	brightnessmodels_constant
#	brightnessmodels_diffusion
#	brightnessmodels_exponential
#	brightnessmodels_movingillumination
)
ENDIF (COMPILE_BRIGHTNESS)

IF (COMPILE_MOTION)
SET(Plugins
	${Plugins}
#	motionmodels_localaffiness
	motionmodels_localconstant
#	motionmodels_localplanarity 
#	motionmodels_localrotation
#	motionmodels_localstretch
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

# install library
SET_TARGET_PROPERTIES(${Plugins}
	PROPERTIES INSTALL_RPATH
	${CHARON_UTILS_ROOT_DIR}/lib${LIB_SUFFIX}/charon-plugins:${CHARON_CORE_ROOT_DIR}/lib${LIB_SUFFIX}
)

INSTALL(
    TARGETS      	${Plugins}
    EXPORT      	${PROJECT_NAME}
    RUNTIME      	DESTINATION bin
    LIBRARY      	DESTINATION ${CHARON_UTILS_ROOT_DIR}/lib${LIB_SUFFIX}/charon-plugins
    ARCHIVE      	DESTINATION ${CHARON_UTILS_ROOT_DIR}/lib${LIB_SUFFIX}/charon-plugins
    COMPONENT    	libraries
)

# install header files to include directory
FILE(GLOB HEADER_FILES
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
