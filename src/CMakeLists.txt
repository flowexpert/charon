INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})

IF (MSVC)
	ADD_DEFINITIONS(-DHANDLE_DLL)
ENDIF()

# plugins
SET(Plugins)

IF (ENABLE_STENCILS)
	ADD_LIBRARY(gbcce SHARED
		Stencils/Gbcce.cpp
		Stencils/Gbcce.h
		Stencils/Gbcce.hxx
		Stencil.h
		Stencil.hxx
		Point4D.h
		Substencil.h
		Substencil.hxx
	)
	ADD_LIBRARY(l2norm SHARED
		Stencils/L2Norm.cpp
		Stencils/L2Norm.h
		Stencils/L2Norm.hxx
		Stencil.h
		Stencil.hxx
		Point4D.h
		Substencil.h
		Substencil.hxx
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
		Solvers/PetscSolver.cpp
		Solvers/PetscSolver.h
		Solvers/PetscSolver.hxx
		Solver.h
		Solver.hxx
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
		IncrementorParameter.h
		IncrementorParameter.hxx
	)
	ADD_LIBRARY(incrementorcountup SHARED
		Incrementors/IncrementorCountUp.cpp
		Incrementors/IncrementorCountUp.h
		Incrementors/IncrementorCountUp.hxx
		Incrementor.h
		Incrementor.hxx
	)
	TARGET_LINK_LIBRARIES(incrementorcountup incrementorparameter)
	ADD_LIBRARY(objectivefunctioncomparing SHARED
		ObjectiveFunctions/ObjectiveFunctionComparing.cpp
		ObjectiveFunctions/ObjectiveFunctionComparing.h
		ObjectiveFunctions/ObjectiveFunctionComparing.hxx
		ObjectiveFunction.h
		ObjectiveFunction.hxx
	)
	ADD_LIBRARY(listedpixelselection SHARED
		PixelSelections/ListedPixelSelection.cpp
		PixelSelections/ListedPixelSelection.h
		PixelSelections/ListedPixelSelection.hxx
		PixelSelection.h
		PixelSelection.hxx
		Pixel.h
		Pixel.hxx
	)
	TARGET_LINK_LIBRARIES(listedpixelselection roi)
	ADD_LIBRARY(surfaceanalysisminchange SHARED
		SurfaceAnalysis/SurfaceAnalysisMinChange.cpp
		SurfaceAnalysis/SurfaceAnalysisMinChange.h
		SurfaceAnalysis/SurfaceAnalysisMinChange.hxx
		SurfaceAnalysis.h
		SurfaceAnalysis.hxx
	)
	TARGET_LINK_LIBRARIES(surfaceanalysisminchange incrementorparameter)
	ADD_LIBRARY(blockmatchingliacs SHARED
		BlockMatching/BlockMatchingLIACS.cpp
		BlockMatching/BlockMatchingLIACS.h
		BlockMatching/BlockMatchingLIACS.hxx
		BlockMatching.h
		BlockMatching.hxx
	)
	TARGET_LINK_LIBRARIES(blockmatchingliacs incrementorcountup listedpixelselection)
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
		BrightnessModels/Constant.cpp
		BrightnessModels/Constant.h
		BrightnessModels/Constant.hxx
		BrightnessModel.h
	)
#	ADD_LIBRARY(brightnessmodels_diffusion SHARED
#		BrightnessModels/Diffusion.cpp
#		BrightnessModels/Diffusion.h
#		BrightnessModels/Diffusion.hxx
#		BrightnessModel.h
#	)
#	ADD_LIBRARY(brightnessmodels_exponential SHARED
#		BrightnessModels/Exponential.cpp
#		BrightnessModels/Exponential.h
#		BrightnessModels/Exponential.hxx
#		BrightnessModel.h
#	)
#	ADD_LIBRARY(brightnessmodels_movingillumination SHARED
#		BrightnessModels/MovingIllumination.cpp
#		BrightnessModels/MovingIllumination.h
#		BrightnessModels/MovingIllumination.hxx
#		BrightnessModel.h
#	)
	LIST(APPEND Plugins
		brightnessmodels_constant
#		brightnessmodels_diffusion
#		brightnessmodels_exponential
#		brightnessmodels_movingillumination
	)
ENDIF (ENABLE_BRIGHTNESS)

IF (ENABLE_MOTION)
#	ADD_LIBRARY(motionmodels_localaffiness SHARED
#		MotionModels/LocalAffiness.cpp
#		MotionModels/LocalAffiness.h
#		MotionModels/LocalAffiness.hxx
#		FlowFunctor.cpp
#		FlowFunctor.h
#		FlowFunctorInterface.h
#		MotionModel.h
#	)
	ADD_LIBRARY(motionmodels_localconstant SHARED
		MotionModels/LocalConstant.cpp
		MotionModels/LocalConstant.h
		MotionModels/LocalConstant.hxx
		FlowFunctor.cpp
		FlowFunctor.h
		FlowFunctorInterface.h
		MotionModel.h
	)
#	ADD_LIBRARY(motionmodels_localplanarity SHARED
#		MotionModels/LocalPlanarity.cpp
#		MotionModels/LocalPlanarity.h
#		MotionModels/LocalPlanarity.hxx
#		FlowFunctor.cpp
#		FlowFunctor.h
#		FlowFunctorInterface.h
#		MotionModel.h
#	)
#	ADD_LIBRARY(motionmodels_localrotation SHARED
#		MotionModels/LocalRotation.cpp
#		MotionModels/LocalRotation.h
#		MotionModels/LocalRotation.hxx
#		FlowFunctor.cpp
#		FlowFunctor.h
#		FlowFunctorInterface.h
#		MotionModel.h
#	)
#	ADD_LIBRARY(motionmodels_localstretch SHARED
#		MotionModels/LocalStretch.cpp
#		MotionModels/LocalStretch.h
#		MotionModels/LocalStretch.hxx
#		FlowFunctor.cpp
#		FlowFunctor.h
#		FlowFunctorInterface.h
#		MotionModel.h
#	)
	LIST(APPEND Plugins
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
		RobustnessTerm.h
		RobustnessTerm.cpp
	)
	ADD_LIBRARY(squarerootrobustnessterm SHARED
		RobustnessTerms/SquarerootRobustnessTerm.cpp
		RobustnessTerms/SquarerootRobustnessTerm.h
	)
	TARGET_LINK_LIBRARIES(squarerootrobustnessterm robustnessterm)
	ADD_LIBRARY(sampleiterativesolver SHARED
		IterativeSolvers/SampleIterativeSolver.cpp
		IterativeSolvers/SampleIterativeSolver.h
		IterativeSolvers/SampleIterativeSolver.hxx
		IterativeSolver.h
		IterativeSolver.hxx
		Solver.h
		Solver.hxx
	)
	TARGET_LINK_LIBRARIES(sampleiterativesolver squarerootrobustnessterm)
	ADD_LIBRARY(iteratorhelper SHARED
		IteratorHelper.h
		IteratorHelper.hxx
		IteratorHelper.cpp
	)
	ADD_LIBRARY(psistencil SHARED
		Stencils/PsiStencil.cpp
		Stencils/PsiStencil.h
		Stencils/PsiStencil.hxx
		Stencil.h
		Stencil.hxx
		Point4D.h
		Substencil.h
		Substencil.hxx
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
	SequenceGenerator.h
	SequenceGenerator.hxx
	SequenceGenerator.cpp)
LIST(APPEND Plugins sequencegenerator)
ADD_LIBRARY(diff2d SHARED
	Diff2D.h
	Diff2D.hxx
	Diff2D.cpp)
TARGET_LINK_LIBRARIES(diff2d interpolatorlinear)
LIST(APPEND Plugins diff2d)
ADD_LIBRARY(flowcomparator SHARED
	FlowComparator.h
	FlowComparator.hxx
	FlowComparator.cpp)
LIST(APPEND Plugins flowcomparator)

FOREACH (X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core ${CIMG_LIBRARIES})
	IF (UNIX)
		GET_TARGET_PROPERTY(PLUGIN_RPATH ${X} INSTALL_RPATH)
		IF(PLUGIN_RPATH)
			SET(PLUGIN_RPATH "${PLUGIN_RPATH}:${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins:${CHARON_CORE_ROOT_DIR}/lib${LIB_SUFFIX}")
		ELSE(PLUGIN_RPATH)
			SET(PLUGIN_RPATH "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins:${CHARON_CORE_ROOT_DIR}/lib${LIB_SUFFIX}")
		ENDIF(PLUGIN_RPATH)
		IF(NOT "${CMAKE_INSTALL_PREFIX}" STREQUAL "${CHARON_UTILS_ROOT_DIR}")
			SET(PLUGIN_RPATH "${CHARON_UTILS_ROOT_DIR}/lib${LIB_SUFFIX}/charon-plugins:${PLUGIN_RPATH}")
		ENDIF(NOT "${CMAKE_INSTALL_PREFIX}" STREQUAL "${CHARON_UTILS_ROOT_DIR}")
		SET_TARGET_PROPERTIES(${X} PROPERTIES INSTALL_RPATH ${PLUGIN_RPATH})
		IF(COMMAND UNSET)
			UNSET(PLUGIN_RPATH)
		ENDIF(COMMAND UNSET)
	ENDIF (UNIX)
ENDFOREACH (X)

# install libraries/plugins
IF(NOT EXPORT_NAME)
	SET(EXPORT_NAME ${PROJECT_NAME})
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
FILE(GLOB_RECURSE HEADER_FILES
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	*.h *.hxx
)
INSTALL(
	FILES           ${HEADER_FILES}
	DESTINATION     include/${PROJECT_NAME}
	COMPONENT       headers
)

IF(NOT SKIP_EXPORT_INSTALL)
	INSTALL(
		EXPORT          ${EXPORT_NAME}
		DESTINATION     include/${PROJECT_NAME}
		COMPONENT       libraries
	)
ENDIF()
