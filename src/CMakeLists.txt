INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${charon-utils_INCLUDE_DIRS})
SET(charon_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})

IF (MSVC)
	ADD_DEFINITIONS(-DHANDLE_DLL)
ENDIF()

# plugins
SET(Plugins) 

IF (ENABLE_STENCILS)
	ADD_LIBRARY(gbcce SHARED
		Gbcce.cpp
		${charon_INCLUDE_DIR}/Stencils/Gbcce.h
		${charon_INCLUDE_DIR}/Stencils/Gbcce.hxx
		${charon_INCLUDE_DIR}/Stencil.h
		${charon_INCLUDE_DIR}/Stencil.hxx
		${charon_INCLUDE_DIR}/Point4D.h
		${charon_INCLUDE_DIR}/Substencil.h
		${charon_INCLUDE_DIR}/Substencil.hxx
	)
	ADD_LIBRARY(l2norm SHARED
		L2Norm.cpp
		${charon_INCLUDE_DIR}/Stencils/L2Norm.h
		${charon_INCLUDE_DIR}/Stencils/L2Norm.hxx
		${charon_INCLUDE_DIR}/Stencil.h
		${charon_INCLUDE_DIR}/Stencil.hxx
		${charon_INCLUDE_DIR}/Point4D.h
		${charon_INCLUDE_DIR}/Substencil.h
		${charon_INCLUDE_DIR}/Substencil.hxx
	)
	LIST(APPEND Plugins
		gbcce
		l2norm
	)
ENDIF (ENABLE_STENCILS)

IF (ENABLE_SOLVERS AND NOT WITH_PETSC)
	MESSAGE(SEND_ERROR
		"Trying to compile solvers without petsc "
		"is currently not supported. "
		"Please activate WITH_PETSC. "
	)
ENDIF()

IF (ENABLE_SOLVERS)
	INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
	ADD_LIBRARY(petscsolver SHARED
		PetscSolver.cpp
		${charon_INCLUDE_DIR}/Solvers/PetscSolver.h
		${charon_INCLUDE_DIR}/Solvers/PetscSolver.hxx
		${charon_INCLUDE_DIR}/Solver.h
		${charon_INCLUDE_DIR}/Solver.hxx
	)
	IF (MSVC)
		SET_TARGET_PROPERTIES(petscsolver
			PROPERTIES LINK_FLAGS
				/NODEFAULTLIB:"LIBCMT"
		)
	ENDIF()
	TARGET_LINK_LIBRARIES(petscsolver
		${PETSC_LIBRARIES}
		roi
		gbcce
	)
	LIST(APPEND Plugins
		petscsolver
	)
	SET_TARGET_PROPERTIES(petscsolver
		PROPERTIES INSTALL_RPATH ${PETSC_ROOT_DIR}/${PETSC_ARCH}/lib
	)
ENDIF (ENABLE_SOLVERS)

IF (ENABLE_BLOCKMATCHING)
	ADD_LIBRARY(incrementorparameter SHARED
		IncrementorParameter.cpp
		${charon_INCLUDE_DIR}/IncrementorParameter.h
		${charon_INCLUDE_DIR}/IncrementorParameter.hxx
	)
	ADD_LIBRARY(incrementorcountup SHARED
		IncrementorCountUp.cpp
		${charon_INCLUDE_DIR}/Incrementors/IncrementorCountUp.h
		${charon_INCLUDE_DIR}/Incrementors/IncrementorCountUp.hxx
		${charon_INCLUDE_DIR}/Incrementor.h
		${charon_INCLUDE_DIR}/Incrementor.hxx
	)
	TARGET_LINK_LIBRARIES(incrementorcountup incrementorparameter)
	ADD_LIBRARY(objectivefunctioncomparing SHARED
		ObjectiveFunctionComparing.cpp
		${charon_INCLUDE_DIR}/ObjectiveFunctions/ObjectiveFunctionComparing.h
		${charon_INCLUDE_DIR}/ObjectiveFunctions/ObjectiveFunctionComparing.hxx
		${charon_INCLUDE_DIR}/ObjectiveFunction.h
		${charon_INCLUDE_DIR}/ObjectiveFunction.hxx
	)
	ADD_LIBRARY(listedpixelselection SHARED
		ListedPixelSelection.cpp
		${charon_INCLUDE_DIR}/PixelSelections/ListedPixelSelection.h
		${charon_INCLUDE_DIR}/PixelSelections/ListedPixelSelection.hxx
		${charon_INCLUDE_DIR}/PixelSelection.h
		${charon_INCLUDE_DIR}/PixelSelection.hxx
		${charon_INCLUDE_DIR}/Pixel.h
		${charon_INCLUDE_DIR}/Pixel.hxx
	)
	TARGET_LINK_LIBRARIES(listedpixelselection roi)
	ADD_LIBRARY(surfaceanalysisminchange SHARED
		SurfaceAnalysisMinChange.cpp
		${charon_INCLUDE_DIR}/SurfaceAnalysis/SurfaceAnalysisMinChange.h
		${charon_INCLUDE_DIR}/SurfaceAnalysis/SurfaceAnalysisMinChange.hxx
		${charon_INCLUDE_DIR}/SurfaceAnalysis.h
		${charon_INCLUDE_DIR}/SurfaceAnalysis.hxx
	)
	TARGET_LINK_LIBRARIES(surfaceanalysisminchange incrementorparameter)
	ADD_LIBRARY(blockmatchingliacs SHARED
		BlockMatchingLIACS.cpp
		${charon_INCLUDE_DIR}/BlockMatching/BlockMatchingLIACS.h
		${charon_INCLUDE_DIR}/BlockMatching/BlockMatchingLIACS.hxx
		${charon_INCLUDE_DIR}/BlockMatching.h
		${charon_INCLUDE_DIR}/BlockMatching.hxx
	)
	TARGET_LINK_LIBRARIES(blockmatchingliacs
		incrementorcountup
		listedpixelselection
	)
	LIST(APPEND Plugins
		incrementorparameter
		incrementorcountup
		objectivefunctioncomparing
		listedpixelselection
		surfaceanalysisminchange
		blockmatchingliacs
	)
ENDIF (ENABLE_BLOCKMATCHING)

IF (ENABLE_BRIGHTNESS)
	ADD_LIBRARY(brightnessmodels_constant SHARED
		BrightnessModelConstant.cpp
		${charon_INCLUDE_DIR}/BrightnessModels/Constant.h
		${charon_INCLUDE_DIR}/BrightnessModels/Constant.hxx
		${charon_INCLUDE_DIR}/BrightnessModel.h
	)
#	ADD_LIBRARY(brightnessmodels_diffusion SHARED
#		BrightnessModelDiffusion.cpp
#		${charon_INCLUDE_DIR}/BrightnessModels/Diffusion.h
#		${charon_INCLUDE_DIR}/BrightnessModels/Diffusion.hxx
#		${charon_INCLUDE_DIR}/BrightnessModel.h
#	)
#	ADD_LIBRARY(brightnessmodels_exponential SHARED
#		BrightnessModelExponential.cpp
#		${charon_INCLUDE_DIR}/BrightnessModels/Exponential.h
#		${charon_INCLUDE_DIR}/BrightnessModels/Exponential.hxx
#		${charon_INCLUDE_DIR}/BrightnessModel.h
#	)
#	ADD_LIBRARY(brightnessmodels_movingillumination SHARED
#		BrightnessModelMovingIllumination.cpp
#		${charon_INCLUDE_DIR}/BrightnessModels/MovingIllumination.h
#		${charon_INCLUDE_DIR}/BrightnessModels/MovingIllumination.hxx
#		${charon_INCLUDE_DIR}/BrightnessModel.h
#	)
	LIST(APPEND Plugins
		brightnessmodels_constant
#		brightnessmodels_diffusion
#		brightnessmodels_exponential
#		brightnessmodels_movingillumination
	)
ENDIF (ENABLE_BRIGHTNESS)

IF (ENABLE_MOTION)
	ADD_LIBRARY(flowfunctor SHARED
		FlowFunctor.cpp
		${charon_INCLUDE_DIR}/FlowFunctor.h
		${charon_INCLUDE_DIR}/FlowFunctorInterface.h
	)
#	ADD_LIBRARY(motionmodels_localaffiness SHARED
#		MotionModelLocalAffiness.cpp
#		${charon_INCLUDE_DIR}/MotionModels/LocalAffiness.h
#		${charon_INCLUDE_DIR}/MotionModels/LocalAffiness.hxx
#		${charon_INCLUDE_DIR}/FlowFunctor.h
#		${charon_INCLUDE_DIR}/FlowFunctorInterface.h
#		${charon_INCLUDE_DIR}/MotionModel.h
#	)
#	TARGET_LINK_LIBRARIES(motionmodels_localaffiness
#		flowfunctor
#	)
	ADD_LIBRARY(motionmodels_localconstant SHARED
		MotionModelLocalConstant.cpp
		${charon_INCLUDE_DIR}/MotionModels/LocalConstant.h
		${charon_INCLUDE_DIR}/MotionModels/LocalConstant.hxx
		${charon_INCLUDE_DIR}/FlowFunctor.h
		${charon_INCLUDE_DIR}/FlowFunctorInterface.h
		${charon_INCLUDE_DIR}/MotionModel.h
	)
	TARGET_LINK_LIBRARIES(motionmodels_localconstant
		flowfunctor
	)
#	ADD_LIBRARY(motionmodels_localplanarity SHARED
#		MotionModelLocalPlanarity.cpp
#		${charon_INCLUDE_DIR}/MotionModels/LocalPlanarity.h
#		${charon_INCLUDE_DIR}/MotionModels/LocalPlanarity.hxx
#		${charon_INCLUDE_DIR}/FlowFunctor.h
#		${charon_INCLUDE_DIR}/FlowFunctorInterface.h
#		${charon_INCLUDE_DIR}/MotionModel.h
#	)
#	TARGET_LINK_LIBRARIES(motionmodels_localplanarity
#		flowfunctor
#	)
#	ADD_LIBRARY(motionmodels_localrotation SHARED
#		MotionModelLocalRotation.cpp
#		${charon_INCLUDE_DIR}/MotionModels/LocalRotation.h
#		${charon_INCLUDE_DIR}/MotionModels/LocalRotation.hxx
#		${charon_INCLUDE_DIR}/FlowFunctor.h
#		${charon_INCLUDE_DIR}/FlowFunctorInterface.h
#		${charon_INCLUDE_DIR}/MotionModel.h
#	)
#	TARGET_LINK_LIBRARIES(motionmodels_localrotation
#		flowfunctor
#	)
#	ADD_LIBRARY(motionmodels_localstretch SHARED
#		MotionModelLocalStretch.cpp
#		${charon_INCLUDE_DIR}/MotionModels/LocalStretch.h
#		${charon_INCLUDE_DIR}/MotionModels/LocalStretch.hxx
#		${charon_INCLUDE_DIR}/FlowFunctor.h
#		${charon_INCLUDE_DIR}/FlowFunctorInterface.h
#		${charon_INCLUDE_DIR}/MotionModel.h
#	)
#	TARGET_LINK_LIBRARIES(motionmodels_localstretch
#		flowfunctor
#	)
	LIST(APPEND Plugins
		flowfunctor
#		motionmodels_localaffiness
		motionmodels_localconstant
#		motionmodels_localplanarity
#		motionmodels_localrotation
#		motionmodels_localstretch
	)
ENDIF (ENABLE_MOTION)

IF(ENABLE_ROBUSTNESS)
	ADD_DEFINITIONS(-DROBUSTNESS)
	ADD_LIBRARY(robustnessterm SHARED
		RobustnessTerm.cpp
		${charon_INCLUDE_DIR}/RobustnessTerm.h
	)
	ADD_LIBRARY(squarerootrobustnessterm SHARED
		SquarerootRobustnessTerm.cpp
		${charon_INCLUDE_DIR}/RobustnessTerms/SquarerootRobustnessTerm.h
	)
	TARGET_LINK_LIBRARIES(squarerootrobustnessterm robustnessterm)
	TARGET_LINK_LIBRARIES(l2norm robustnessterm)
	ADD_LIBRARY(sampleiterativesolver SHARED
		SampleIterativeSolver.cpp
		${charon_INCLUDE_DIR}/IterativeSolvers/SampleIterativeSolver.h
		${charon_INCLUDE_DIR}/IterativeSolvers/SampleIterativeSolver.hxx
		${charon_INCLUDE_DIR}/IterativeSolver.h
		${charon_INCLUDE_DIR}/IterativeSolver.hxx
		${charon_INCLUDE_DIR}/Solver.h
		${charon_INCLUDE_DIR}/Solver.hxx
	)
	TARGET_LINK_LIBRARIES(sampleiterativesolver squarerootrobustnessterm)
	ADD_LIBRARY(iteratorhelper SHARED
		IteratorHelper.cpp
		${charon_INCLUDE_DIR}/IteratorHelper.h
		${charon_INCLUDE_DIR}/IteratorHelper.hxx
	)
	ADD_LIBRARY(psistencil SHARED
		PsiStencil.cpp
		${charon_INCLUDE_DIR}/Stencils/PsiStencil.h
		${charon_INCLUDE_DIR}/Stencils/PsiStencil.hxx
		${charon_INCLUDE_DIR}/Stencil.h
		${charon_INCLUDE_DIR}/Stencil.hxx
		${charon_INCLUDE_DIR}/Point4D.h
		${charon_INCLUDE_DIR}/Substencil.h
		${charon_INCLUDE_DIR}/Substencil.hxx
	)
	TARGET_LINK_LIBRARIES(psistencil squarerootrobustnessterm)
	LIST(APPEND Plugins
		robustnessterm
		squarerootrobustnessterm
		sampleiterativesolver
		iteratorhelper
		psistencil
	)
ENDIF(ENABLE_ROBUSTNESS)

ADD_LIBRARY(sequencegenerator SHARED
	SequenceGenerator.cpp
	${charon_INCLUDE_DIR}/SequenceGenerator.h
	${charon_INCLUDE_DIR}/SequenceGenerator.hxx
)
LIST(APPEND Plugins sequencegenerator)

ADD_LIBRARY(diff2d SHARED
	Diff2D.cpp
	${charon_INCLUDE_DIR}/Diff2D.h
	${charon_INCLUDE_DIR}/Diff2D.hxx
)
TARGET_LINK_LIBRARIES(diff2d interpolatorlinear)
LIST(APPEND Plugins diff2d)

ADD_LIBRARY(flowcomparator SHARED
	FlowComparator.cpp
	${charon_INCLUDE_DIR}/FlowComparator.h
	${charon_INCLUDE_DIR}/FlowComparator.hxx
)
LIST(APPEND Plugins flowcomparator)

SET(GLOBAL_RPATH "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins")
SET(GLOBAL_RPATH "${GLOBAL_RPATH}:${charon-core_ROOT_DIR}/lib${LIB_SUFFIX}")
SET(LOCAL_RPATH "${charon-utils_ROOT_DIR}/lib${LIB_SUFFIX}/charon-plugins")
FOREACH (X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core ${CIMG_LIBRARIES})
	IF (UNIX)
		GET_TARGET_PROPERTY(PLUGIN_RPATH ${X} INSTALL_RPATH)
		IF(PLUGIN_RPATH)
			SET(PLUGIN_RPATH "${PLUGIN_RPATH}:${GLOBAL_RPATH}")
		ELSE()
			SET(PLUGIN_RPATH "${GLOBAL_RPATH}")
		ENDIF()
		IF(NOT "${CMAKE_INSTALL_PREFIX}" STREQUAL "${charon-utils_ROOT_DIR}")
			SET(PLUGIN_RPATH "${LOCAL_RPATH}:${PLUGIN_RPATH}")
		ENDIF()
		SET_TARGET_PROPERTIES(${X} PROPERTIES INSTALL_RPATH ${PLUGIN_RPATH})
		UNSET(PLUGIN_RPATH)
	ENDIF (UNIX)
ENDFOREACH (X)
UNSET(GLOBAL_RPATH)
UNSET(LOCAL_RPATH)

# install libraries/plugins
IF(NOT EXPORT_NAME)
	SET(EXPORT_NAME ${PROJECT_NAME}-export)
ENDIF()
INSTALL(
	TARGETS         ${Plugins}
	EXPORT          ${EXPORT_NAME}
	RUNTIME         DESTINATION bin
	LIBRARY         DESTINATION lib${LIB_SUFFIX}/charon-plugins
	ARCHIVE         DESTINATION lib${LIB_SUFFIX}/charon-plugins
	COMPONENT       libraries
)

# install header files to include directory
INSTALL(
	DIRECTORY       ${charon_INCLUDE_DIR}/
	DESTINATION     include
	COMPONENT       headers
)

IF(NOT SKIP_EXPORT_INSTALL)
	INSTALL(
		EXPORT          ${EXPORT_NAME}
		DESTINATION     ${CONFIG_INSTALL}
		COMPONENT       libraries
	)
ENDIF() 
