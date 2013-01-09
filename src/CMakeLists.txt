INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${charon-utils_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CIMG_INCLUDE_DIRS})
SET(charon_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})

IF (MSVC)
	ADD_DEFINITIONS(-DHANDLE_DLL)
	ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
ENDIF()

# list of all plugins to be built
SET(Plugins)

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIRS})

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
ADD_LIBRARY(dataconstant SHARED
	${charon_INCLUDE_DIR}/Stencils/DataConstant.h
	${charon_INCLUDE_DIR}/Stencils/DataConstant.hxx
	DataConstant.cpp
)
ADD_LIBRARY(spatialpw SHARED
	${charon_INCLUDE_DIR}/Stencils/SpatialPW.h
	${charon_INCLUDE_DIR}/Stencils/SpatialPW.hxx
	SpatialPW.cpp
)
LIST(APPEND Plugins
	gbcce
	rfmc
	l2norm
	dataconstant
	spatialpw
)

# Energy Stencils
ADD_LIBRARY(energybcc SHARED
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyBCC.h
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyBCC.hxx
	EnergyBCC.cpp
)
ADD_LIBRARY(energyclassic SHARED
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyClassic.h
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyClassic.hxx
	EnergyClassic.cpp
)
ADD_LIBRARY(energycoupling SHARED
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyCoupling.h
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyCoupling.hxx
	EnergyCoupling.cpp
)
ADD_LIBRARY(energynonlocal SHARED
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyNonLocal.h
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyNonLocal.hxx
	EnergyNonLocal.cpp
)
LIST(APPEND Plugins
	energybcc
	energyclassic
	energycoupling
	energynonlocal
)

# Penalty Functions
ADD_LIBRARY(penaltycombined SHARED
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyCombined.h
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyCombined.hxx
	PenaltyCombined.cpp
)
ADD_LIBRARY(penaltyquadratic SHARED
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyQuadratic.h
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyQuadratic.hxx
	PenaltyQuadratic.cpp
)
ADD_LIBRARY(penaltycharbonnier SHARED
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyCharbonnier.h
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyCharbonnier.hxx
	PenaltyCharbonnier.cpp
)
ADD_LIBRARY(penaltylorentzian SHARED
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyLorentzian.h
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyLorentzian.hxx
	PenaltyLorentzian.cpp
)
LIST(APPEND Plugins
	penaltycombined
	penaltyquadratic
	penaltycharbonnier
	penaltylorentzian
)

# Clique Weights
ADD_LIBRARY(cliqueweightuniform SHARED
	${charon_INCLUDE_DIR}/CliqueWeights/CliqueWeightUniform.h
	${charon_INCLUDE_DIR}/CliqueWeights/CliqueWeightUniform.hxx
	CliqueWeightUniform.cpp
)
ADD_LIBRARY(cliqueweightnonlocal SHARED
	${charon_INCLUDE_DIR}/CliqueWeights/CliqueWeightNonLocal.h
	${charon_INCLUDE_DIR}/CliqueWeights/CliqueWeightNonLocal.hxx
	CliqueWeightNonLocal.cpp
)
LIST(APPEND Plugins
	cliqueweightuniform
	cliqueweightnonlocal
)

# Solvers
# PetSc Solver
IF(USE_PETSC)
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
		debug gbcce # only needed in debug mode (why?)
	)
	LIST(APPEND Plugins
		petscsolver
	)
ENDIF(USE_PETSC)

# Conjugate gradient method
ADD_LIBRARY(cgsolverhelper SHARED
	${charon_INCLUDE_DIR}/CGSolverHelper.h
	${charon_INCLUDE_DIR}/CGSolverHelper.hxx
	CGSolverHelper.cpp
)
ADD_LIBRARY(cgsolver SHARED
	CGSolver.cpp
	${charon_INCLUDE_DIR}/Solvers/CGSolver.h
	${charon_INCLUDE_DIR}/Solvers/CGSolver.hxx
)

# second order Newton method
ADD_LIBRARY(newton2 SHARED
	${charon_INCLUDE_DIR}/Solvers/Newton2.h
	${charon_INCLUDE_DIR}/Solvers/Newton2.hxx
	Newton2.cpp
)

# Iteratively reweighted least squares
ADD_LIBRARY(irls SHARED
	${charon_INCLUDE_DIR}/Irls.h
	${charon_INCLUDE_DIR}/Irls.hxx
	Irls.cpp
)

# weighted median filtering, as proposed by Li and Osher (2010, eq. 3.13)
ADD_LIBRARY(weightedmedian SHARED
	${charon_INCLUDE_DIR}/WeightedMedian.h
	${charon_INCLUDE_DIR}/WeightedMedian.hxx
	WeightedMedian.cpp
)

# non-local weight calculation function
ADD_LIBRARY(weightsnonlocal SHARED
	${charon_INCLUDE_DIR}/WeightsNonLocal.h
	${charon_INCLUDE_DIR}/WeightsNonLocal.hxx
	WeightsNonLocal.cpp
)

# residual calculation
ADD_LIBRARY(residual SHARED
	${charon_INCLUDE_DIR}/Residual.h
	${charon_INCLUDE_DIR}/Residual.hxx
	Residual.cpp
)

# non local median filtering
ADD_LIBRARY(irlsnonlocal SHARED
	${charon_INCLUDE_DIR}/IrlsNonLocal.h
	${charon_INCLUDE_DIR}/IrlsNonLocal.hxx
	IrlsNonLocal.cpp
)
LIST(APPEND Plugins
	cgsolverhelper
	cgsolver
	newton2
	irls
	weightedmedian
	weightsnonlocal
	residual
	irlsnonlocal
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
ADD_LIBRARY(brightnessmodels_diffusion SHARED
	BrightnessModelDiffusion.cpp
	${charon_INCLUDE_DIR}/BrightnessModels/Diffusion.h
	${charon_INCLUDE_DIR}/BrightnessModels/Diffusion.hxx
	${charon_INCLUDE_DIR}/BrightnessModel.h
)
LIST(APPEND Plugins
	brightnessmodels_constant
	brightnessmodels_diffusion
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
ADD_LIBRARY(motionmodels_lucaskanade SHARED
	MotionModelLucasKanade.cpp
	${charon_INCLUDE_DIR}/MotionModels/LucasKanade.h
	${charon_INCLUDE_DIR}/MotionModels/LucasKanade.hxx
	${charon_INCLUDE_DIR}/MotionModel.h
)
LIST(APPEND Plugins
	motionmodels_localconstant
	motionmodels_lucaskanade
)

# Iterator plugins
ADD_LIBRARY(value SHARED
	${charon_INCLUDE_DIR}/Value.h
	${charon_INCLUDE_DIR}/Value.hxx
	Value.cpp
)
ADD_LIBRARY(iteratorhelper SHARED
	${charon_INCLUDE_DIR}/IteratorHelper.h
	${charon_INCLUDE_DIR}/IteratorHelper.hxx
	IteratorHelper.cpp
)
IF(QT4_FOUND)
	SET(simpleiteratorremotecontrol_HEADERS_TOMOC ${charon_INCLUDE_DIR}/SimpleIteratorRemoteControl.h)
	QT4_WRAP_CPP(simpleiteratorremotecontrol_HEADERS_MOCKED ${simpleiteratorremotecontrol_HEADERS_TOMOC})
	ADD_LIBRARY(simpleiterator SHARED
		${charon_INCLUDE_DIR}/SimpleIterator.h
		${charon_INCLUDE_DIR}/SimpleIterator.hxx
		${simpleiteratorremotecontrol_HEADERS_MOCKED}
		SimpleIteratorRemoteControl.cpp
		SimpleIterator.cpp
	)
	TARGET_LINK_LIBRARIES(simpleiterator ${QT_LIBRARIES})
ELSE(QT4_FOUND)
	ADD_LIBRARY(simpleiterator SHARED
		${charon_INCLUDE_DIR}/SimpleIterator.h
		${charon_INCLUDE_DIR}/SimpleIterator.hxx
		SimpleIterator.cpp
	)
ENDIF(QT4_FOUND)
TARGET_LINK_LIBRARIES(simpleiterator # also only needed in debug mode
	debug roi
	debug iteratorhelper
)
IF(QT4_FOUND)
	SET(simpleiteratorremotecontrol_HEADERS_TOMOC ${charon_INCLUDE_DIR}/SimpleIteratorRemoteControl.h)
	QT4_WRAP_CPP(simpleiteratorremotecontrol_HEADERS_MOCKED ${simpleiteratorremotecontrol_HEADERS_TOMOC})
	ADD_LIBRARY(relaxingiterator SHARED
		${charon_INCLUDE_DIR}/RelaxingIterator.h
		${charon_INCLUDE_DIR}/RelaxingIterator.hxx
		${simpleiteratorremotecontrol_HEADERS_MOCKED}
		SimpleIteratorRemoteControl.cpp
		RelaxingIterator.cpp
	)
	TARGET_LINK_LIBRARIES(relaxingiterator ${QT_LIBRARIES})
ELSE(QT4_FOUND)
	ADD_LIBRARY(relaxingiterator SHARED
		${charon_INCLUDE_DIR}/RelaxingIterator.h
		${charon_INCLUDE_DIR}/RelaxingIterator.hxx
		RelaxingIterator.cpp
	)
ENDIF(QT4_FOUND)
TARGET_LINK_LIBRARIES(relaxingiterator # also only needed in debug mode
	debug roi
	debug iteratorhelper
)
LIST(APPEND Plugins
	value
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
SET_TARGET_PROPERTIES(cdfitfunction PROPERTIES
	SOVERSION ${charon_VERSION}
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
ADD_LIBRARY(psifunction SHARED
	PsiFunction.cpp
	${charon_INCLUDE_DIR}/PsiFunction.h
	${charon_INCLUDE_DIR}/Functions/Function.h
	${charon_INCLUDE_DIR}/Functions/DiffFunction.h
	${charon_INCLUDE_DIR}/Functions/Diff2Function.h
	${charon_INCLUDE_DIR}/Functions/Diff2LinFunction.h
)
LIST(APPEND Plugins
	cdfitfunction
	mixtureofgaussians
	psifunction
)

# sampling and CD fitter
ADD_LIBRARY(sampler SHARED
	${charon_INCLUDE_DIR}/Sampler.h
	Sampler.cpp
)
SET_TARGET_PROPERTIES(sampler PROPERTIES
	SOVERSION ${charon_VERSION}
)
ADD_LIBRARY(mcmcsampler SHARED
	${charon_INCLUDE_DIR}/McmcSampler.h
	McmcSampler.cpp
)
SET_TARGET_PROPERTIES(mcmcsampler PROPERTIES
	SOVERSION ${charon_VERSION}
)
TARGET_LINK_LIBRARIES(mcmcsampler sampler)
ADD_LIBRARY(samplegenerator SHARED
	${charon_INCLUDE_DIR}/SampleGenerator.h
	${charon_INCLUDE_DIR}/SampleGenerator.hxx
	SampleGenerator.cpp
)
TARGET_LINK_LIBRARIES(samplegenerator sampler)
ADD_LIBRARY(cdfitter SHARED
	${charon_INCLUDE_DIR}/CdFitter.h
	${charon_INCLUDE_DIR}/CdFitter.hxx
	CdFitter.cpp
)
TARGET_LINK_LIBRARIES(cdfitter mcmcsampler cdfitfunction)

LIST(APPEND Plugins
	sampler
	mcmcsampler
	samplegenerator
	cdfitter
)

# Warpers
ADD_LIBRARY(warper SHARED
	Warper.cpp
	${charon_INCLUDE_DIR}/Warper.h
	${charon_INCLUDE_DIR}/Warper.hxx
)
LIST(APPEND Plugins
	warper
)

# Misc plugins
ADD_LIBRARY(gnc SHARED
	Gnc.cpp
	${charon_INCLUDE_DIR}/Gnc.h
	${charon_INCLUDE_DIR}/Gnc.hxx
)
ADD_LIBRARY(sequencegenerator SHARED
	SequenceGenerator.cpp
	${charon_INCLUDE_DIR}/SequenceGenerator.h
	${charon_INCLUDE_DIR}/SequenceGenerator.hxx
)
ADD_LIBRARY(edgysequencegenerator SHARED
	${charon_INCLUDE_DIR}/EdgySequenceGenerator.h
	${charon_INCLUDE_DIR}/EdgySequenceGenerator.hxx
	EdgySequenceGenerator.cpp
)
ADD_LIBRARY(diff2d SHARED
	Diff2D.cpp
	${charon_INCLUDE_DIR}/Diff2D.h
	${charon_INCLUDE_DIR}/Diff2D.hxx
)
ADD_LIBRARY(simplediff SHARED
	SimpleDiff.cpp
	${charon_INCLUDE_DIR}/SimpleDiff.h
	${charon_INCLUDE_DIR}/SimpleDiff.hxx
)
ADD_LIBRARY(lucas_kanade_smoothing SHARED
	LucasKanadeSmoothing.cpp
	${charon_INCLUDE_DIR}/LucasKanadeSmoothing.h
	${charon_INCLUDE_DIR}/LucasKanadeSmoothing.hxx
)
IF(MSVC)
	# avoid strange linker errors
	TARGET_LINK_LIBRARIES(diff2d interpolatorlinear)
ENDIF()
ADD_LIBRARY(pyramidrescale SHARED
	PyramidRescale.cpp
	${charon_INCLUDE_DIR}/PyramidRescale.h
	${charon_INCLUDE_DIR}/PyramidRescale.hxx
)
TARGET_LINK_LIBRARIES(pyramidrescale interpolatorlinear roi)
ADD_LIBRARY(pyramidrescalematlab SHARED
	PyramidRescaleMatlab.cpp
	${charon_INCLUDE_DIR}/PyramidRescaleMatlab.h
	${charon_INCLUDE_DIR}/PyramidRescaleMatlab.hxx
)
TARGET_LINK_LIBRARIES(pyramidrescalematlab interpolatorlinear roi)
ADD_LIBRARY(pyramidlowpass SHARED
	PyramidLowpass.cpp
	${charon_INCLUDE_DIR}/PyramidLowpass.h
	${charon_INCLUDE_DIR}/PyramidLowpass.hxx
)
ADD_LIBRARY(flowcomparator SHARED
	FlowComparator.cpp
	${charon_INCLUDE_DIR}/FlowComparator.h
	${charon_INCLUDE_DIR}/FlowComparator.hxx
)
LIST(APPEND Plugins
	gnc
	sequencegenerator
	edgysequencegenerator
	diff2d
	simplediff
	lucas_kanade_smoothing
	pyramidrescale
	pyramidrescalematlab
	pyramidlowpass
	flowcomparator
)

# Plugin configuration (linking, common libraries)
SET(GLOBAL_RPATH "${charon-utils_ROOT_DIR}/lib${LIB_SUFFIX}/charon-plugins")
FOREACH (X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core ${CIMG_LINK_LIBRARIES})
	SET_TARGET_PROPERTIES(${X} PROPERTIES
		FOLDER "charon-flow Modules"
		INSTALL_RPATH "${GLOBAL_RPATH}"
	)
ENDFOREACH (X)
UNSET(GLOBAL_RPATH)

# install libraries/plugins
IF(NOT EXPORT_NAME)
	SET(EXPORT_NAME ${PROJECT_NAME}-export)
ENDIF()
INSTALL(
	TARGETS         ${Plugins}
	EXPORT          ${EXPORT_NAME}
	RUNTIME         DESTINATION bin${BIN_SUFFIX}
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
