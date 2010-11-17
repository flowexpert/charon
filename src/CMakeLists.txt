INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${charon-utils_INCLUDE_DIRS})
SET(charon_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})

IF (MSVC)
	ADD_DEFINITIONS(-DHANDLE_DLL)
ENDIF()

# list of all plugins to be built
SET(Plugins)

# Stencils
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
ADD_LIBRARY(rfmc SHARED
	Rfmc.cpp
	${charon_INCLUDE_DIR}/Stencils/Rfmc.h
	${charon_INCLUDE_DIR}/Stencils/Rfmc.hxx
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
	rfmc
	l2norm
)

# Solvers
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

# Brightness models (currently only constant brightness available)
# to-be-ported and yet untested (ask for work of René):
# - BrightnessModelDiffusion
# - BrightnessModelExponential
# - BrightnessModelMovingIllumination
ADD_LIBRARY(brightnessmodels_constant SHARED
	BrightnessModelConstant.cpp
	${charon_INCLUDE_DIR}/BrightnessModels/Constant.h
	${charon_INCLUDE_DIR}/BrightnessModels/Constant.hxx
	${charon_INCLUDE_DIR}/BrightnessModel.h
)
LIST(APPEND Plugins
	brightnessmodels_constant
)

# Motion models (currently only local constant motion available)
# to-be-ported and yet untested (ask for work of René):
# - MotionModelLocalAffiness
# - MotionModelLocalPlanarity
# - MotionModelLocalRotation
# - MotionModelLocalStretch
ADD_LIBRARY(motionmodels_localconstant SHARED
	MotionModelLocalConstant.cpp
	${charon_INCLUDE_DIR}/MotionModels/LocalConstant.h
	${charon_INCLUDE_DIR}/MotionModels/LocalConstant.hxx
	${charon_INCLUDE_DIR}/MotionModel.h
)
LIST(APPEND Plugins
	motionmodels_localconstant
)

# Iterator plugins
ADD_LIBRARY(iteratorhelper SHARED
	${charon_INCLUDE_DIR}/IteratorHelper.h
	${charon_INCLUDE_DIR}/IteratorHelper.hxx
	IteratorHelper.cpp
)
ADD_LIBRARY(simpleiterator SHARED
	${charon_INCLUDE_DIR}/SimpleIterator.h
	${charon_INCLUDE_DIR}/SimpleIterator.hxx
	SimpleIterator.cpp
)
TARGET_LINK_LIBRARIES(simpleiterator iteratorhelper)
ADD_LIBRARY(relaxingiterator SHARED
	${charon_INCLUDE_DIR}/RelaxingIterator.h
	${charon_INCLUDE_DIR}/RelaxingIterator.hxx
	RelaxingIterator.cpp
)
TARGET_LINK_LIBRARIES(relaxingiterator simpleiterator gbcce)
LIST(APPEND Plugins
	iteratorhelper
	simpleiterator
	relaxingiterator
)

# Function plugins
ADD_LIBRARY(cdfitfunction SHARED
	CDFitFunction.cpp
	${charon_INCLUDE_DIR}/CDFitFunction.h
	${charon_INCLUDE_DIR}/Functions/Function.h
)
ADD_LIBRARY(mixtureofgaussians SHARED
	MixtureOfGaussians.cpp
	${charon_INCLUDE_DIR}/CDFitFunction.h
	${charon_INCLUDE_DIR}/MixtureOfGaussians.h
	${charon_INCLUDE_DIR}/Functions/Function.h
	${charon_INCLUDE_DIR}/Functions/DiffFunction.h
	${charon_INCLUDE_DIR}/Functions/Diff2Function.h
	${charon_INCLUDE_DIR}/Functions/Diff2LinFunction.h
)
TARGET_LINK_LIBRARIES(mixtureofgaussians cdfitfunction)
LIST(APPEND Plugins
	cdfitfunction
	mixtureofgaussians
)

# sampling and CD fitter
ADD_LIBRARY(sampler SHARED
	${charon_INCLUDE_DIR}/Sampler.h
	Sampler.cpp
)
ADD_LIBRARY(mcmcsampler SHARED
	${charon_INCLUDE_DIR}/McmcSampler.h
	McmcSampler.cpp
)
TARGET_LINK_LIBRARIES(mcmcsampler sampler)

LIST(APPEND Plugins
	sampler
	mcmcsampler
)

# Misc plugins
ADD_LIBRARY(sequencegenerator SHARED
	SequenceGenerator.cpp
	${charon_INCLUDE_DIR}/SequenceGenerator.h
	${charon_INCLUDE_DIR}/SequenceGenerator.hxx
)
ADD_LIBRARY(diff2d SHARED
	Diff2D.cpp
	${charon_INCLUDE_DIR}/Diff2D.h
	${charon_INCLUDE_DIR}/Diff2D.hxx
)
TARGET_LINK_LIBRARIES(diff2d interpolatorlinear)
ADD_LIBRARY(flowcomparator SHARED
	FlowComparator.cpp
	${charon_INCLUDE_DIR}/FlowComparator.h
	${charon_INCLUDE_DIR}/FlowComparator.hxx
)
LIST(APPEND Plugins
	sequencegenerator
	diff2d
	flowcomparator
)

# Plugin configuration (linking, common libraries)
SET(GLOBAL_RPATH "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins")
SET(GLOBAL_RPATH "${GLOBAL_RPATH}:${charon-core_ROOT_DIR}/lib${LIB_SUFFIX}")
SET(LOCAL_RPATH "${charon-utils_ROOT_DIR}/lib${LIB_SUFFIX}/charon-plugins")
FOREACH (X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core ${CIMG_LINK_LIBRARIES})
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
	DIRECTORY       ${charon_INCLUDE_DIR}
	DESTINATION     include
	COMPONENT       headers
	PATTERN         ".svn" EXCLUDE
)

IF(NOT SKIP_EXPORT_INSTALL)
	INSTALL(
		EXPORT          ${EXPORT_NAME}
		DESTINATION     ${CONFIG_INSTALL}
		COMPONENT       libraries
	)
ENDIF() 
