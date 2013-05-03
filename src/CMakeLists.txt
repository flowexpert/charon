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
ADD_CHARON_PLUGIN(gbcce
	Gbcce.cpp
	${charon_INCLUDE_DIR}/Stencils/Gbcce.h
	${charon_INCLUDE_DIR}/Stencils/Gbcce.hxx
	${charon_INCLUDE_DIR}/Stencil.h
	${charon_INCLUDE_DIR}/Stencil.hxx
	${charon_INCLUDE_DIR}/Point4D.h
	${charon_INCLUDE_DIR}/Substencil.h
	${charon_INCLUDE_DIR}/Substencil.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(rfmc
	Rfmc.cpp
	${charon_INCLUDE_DIR}/Stencils/Rfmc.h
	${charon_INCLUDE_DIR}/Stencils/Rfmc.hxx
	${charon_INCLUDE_DIR}/Stencil.h
	${charon_INCLUDE_DIR}/Stencil.hxx
	${charon_INCLUDE_DIR}/Point4D.h
	${charon_INCLUDE_DIR}/Substencil.h
	${charon_INCLUDE_DIR}/Substencil.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(l2norm
	L2Norm.cpp
	${charon_INCLUDE_DIR}/Stencils/L2Norm.h
	${charon_INCLUDE_DIR}/Stencils/L2Norm.hxx
	${charon_INCLUDE_DIR}/Stencil.h
	${charon_INCLUDE_DIR}/Stencil.hxx
	${charon_INCLUDE_DIR}/Point4D.h
	${charon_INCLUDE_DIR}/Substencil.h
	${charon_INCLUDE_DIR}/Substencil.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(dataconstant
	${charon_INCLUDE_DIR}/Stencils/DataConstant.h
	${charon_INCLUDE_DIR}/Stencils/DataConstant.hxx
	DataConstant.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(spatialpw
	${charon_INCLUDE_DIR}/Stencils/SpatialPW.h
	${charon_INCLUDE_DIR}/Stencils/SpatialPW.hxx
	SpatialPW.cpp
	PLUGIN_LISTS Plugins
)

# Energy Stencils
ADD_CHARON_PLUGIN(energybcc
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyBCC.h
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyBCC.hxx
	EnergyBCC.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(energyclassic
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyClassic.h
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyClassic.hxx
	EnergyClassic.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(energycoupling
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyCoupling.h
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyCoupling.hxx
	EnergyCoupling.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(energynonlocal
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyNonLocal.h
	${charon_INCLUDE_DIR}/EnergyStencils/EnergyNonLocal.hxx
	EnergyNonLocal.cpp
	PLUGIN_LISTS Plugins
)
# Stencils for over-parameterized motion estimation
ADD_CHARON_PLUGIN(affinebcc
	${charon_INCLUDE_DIR}/Stencils/AffineBCC.h
	${charon_INCLUDE_DIR}/Stencils/AffineBCC.hxx
	AffineBCC.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(affine2translation
	${charon_INCLUDE_DIR}/Affine2Translation.h
	${charon_INCLUDE_DIR}/Affine2Translation.hxx
	Affine2Translation.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(trigonometricbcc
	${charon_INCLUDE_DIR}/Stencils/TrigonometricBCC.h
	${charon_INCLUDE_DIR}/Stencils/TrigonometricBCC.hxx
	TrigonometricBCC.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(magnituderegularization
	${charon_INCLUDE_DIR}/Stencils/MagnitudeRegularization.h
	${charon_INCLUDE_DIR}/Stencils/MagnitudeRegularization.hxx
	MagnitudeRegularization.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(matchconstraint
	${charon_INCLUDE_DIR}/Stencils/MatchConstraint.h
	${charon_INCLUDE_DIR}/Stencils/MatchConstraint.hxx
	MatchConstraint.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(imagerotation
	${charon_INCLUDE_DIR}/ImageRotation.h
	${charon_INCLUDE_DIR}/ImageRotation.hxx
	ImageRotation.cpp
	PLUGIN_LISTS Plugins
)

# Penalty Functions
ADD_CHARON_PLUGIN(penaltycombined
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyCombined.h
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyCombined.hxx
	PenaltyCombined.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(penaltyquadratic
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyQuadratic.h
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyQuadratic.hxx
	PenaltyQuadratic.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(penaltycharbonnier
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyCharbonnier.h
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyCharbonnier.hxx
	PenaltyCharbonnier.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(penaltylorentzian
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyLorentzian.h
	${charon_INCLUDE_DIR}/PenaltyFunctions/PenaltyLorentzian.hxx
	PenaltyLorentzian.cpp
	PLUGIN_LISTS Plugins
)

# Clique Weights
ADD_CHARON_PLUGIN(cliqueweightuniform
	${charon_INCLUDE_DIR}/CliqueWeights/CliqueWeightUniform.h
	${charon_INCLUDE_DIR}/CliqueWeights/CliqueWeightUniform.hxx
	CliqueWeightUniform.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(cliqueweightnonlocal
	${charon_INCLUDE_DIR}/CliqueWeights/CliqueWeightNonLocal.h
	${charon_INCLUDE_DIR}/CliqueWeights/CliqueWeightNonLocal.hxx
	CliqueWeightNonLocal.cpp
	PLUGIN_LISTS Plugins
)

# Bootstrapping weights
ADD_CHARON_PLUGIN(bootstrappingweights
	${charon_INCLUDE_DIR}/BootstrappingWeights.h
	${charon_INCLUDE_DIR}/BootstrappingWeights.hxx
	BootstrappingWeights.cpp
	PLUGIN_LISTS Plugins
)

# Solvers
# PetSc Solver
IF(USE_PETSC)
	INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
	ADD_CHARON_PLUGIN(petscsolver
		PetscSolver.cpp
		${charon_INCLUDE_DIR}/Solvers/PetscSolver.h
		${charon_INCLUDE_DIR}/Solvers/PetscSolver.hxx
		${charon_INCLUDE_DIR}/Solver.h
		${charon_INCLUDE_DIR}/Solver.hxx
		PLUGIN_LISTS Plugins
		LINK_LIBRARIES ${PETSC_LIBRARIES} debug gbcce
	)
	IF (MSVC)
		SET_TARGET_PROPERTIES(petscsolver
			PROPERTIES LINK_FLAGS
				/NODEFAULTLIB:"LIBCMT"
		)
	ENDIF()
ENDIF(USE_PETSC)

# Conjugate gradient method
ADD_CHARON_PLUGIN(cgsolverhelper
	${charon_INCLUDE_DIR}/CGSolverHelper.h
	${charon_INCLUDE_DIR}/CGSolverHelper.hxx
	CGSolverHelper.cpp
	PLUGIN_LISTS Plugins

)
ADD_CHARON_PLUGIN(cgsolver
	CGSolver.cpp
	${charon_INCLUDE_DIR}/Solvers/CGSolver.h
	${charon_INCLUDE_DIR}/Solvers/CGSolver.hxx
	PLUGIN_LISTS Plugins
)

# second order Newton method
ADD_CHARON_PLUGIN(newton2
	${charon_INCLUDE_DIR}/Solvers/Newton2.h
	${charon_INCLUDE_DIR}/Solvers/Newton2.hxx
	Newton2.cpp
	PLUGIN_LISTS Plugins
)

# Iteratively reweighted least squares
ADD_CHARON_PLUGIN(irls
	${charon_INCLUDE_DIR}/Irls.h
	${charon_INCLUDE_DIR}/Irls.hxx
	Irls.cpp
	PLUGIN_LISTS Plugins
)

# weighted median filtering, as proposed by Li and Osher (2010, eq. 3.13)
ADD_CHARON_PLUGIN(weightedmedian
	${charon_INCLUDE_DIR}/WeightedMedian.h
	${charon_INCLUDE_DIR}/WeightedMedian.hxx
	WeightedMedian.cpp
	PLUGIN_LISTS Plugins
)

# non-local weight calculation function
ADD_CHARON_PLUGIN(weightsnonlocal
	${charon_INCLUDE_DIR}/WeightsNonLocal.h
	${charon_INCLUDE_DIR}/WeightsNonLocal.hxx
	WeightsNonLocal.cpp
	PLUGIN_LISTS Plugins
)

# residual calculation
ADD_CHARON_PLUGIN(residual
	${charon_INCLUDE_DIR}/Residual.h
	${charon_INCLUDE_DIR}/Residual.hxx
	Residual.cpp
	PLUGIN_LISTS Plugins
)

# non local median filtering
ADD_CHARON_PLUGIN(irlsnonlocal
	${charon_INCLUDE_DIR}/IrlsNonLocal.h
	${charon_INCLUDE_DIR}/IrlsNonLocal.hxx
	IrlsNonLocal.cpp
	PLUGIN_LISTS Plugins
)

# Brightness models (currently only constant brightness available)
# to-be-ported and yet untested (ask for work of René):
# - BrightnessModelDiffusion
# - BrightnessModelExponential
# - BrightnessModelMovingIllumination
ADD_CHARON_PLUGIN(brightnessmodels_constant
	BrightnessModelConstant.cpp
	${charon_INCLUDE_DIR}/BrightnessModels/Constant.h
	${charon_INCLUDE_DIR}/BrightnessModels/Constant.hxx
	${charon_INCLUDE_DIR}/BrightnessModel.h
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(brightnessmodels_diffusion
	BrightnessModelDiffusion.cpp
	${charon_INCLUDE_DIR}/BrightnessModels/Diffusion.h
	${charon_INCLUDE_DIR}/BrightnessModels/Diffusion.hxx
	${charon_INCLUDE_DIR}/BrightnessModel.h
	PLUGIN_LISTS Plugins
)

# Motion models (currently only local constant motion available)
# to-be-ported and yet untested (ask for work of René):
# - MotionModelLocalAffiness
# - MotionModelLocalPlanarity
# - MotionModelLocalRotation
# - MotionModelLocalStretch
ADD_CHARON_PLUGIN(motionmodels_localconstant
	MotionModelLocalConstant.cpp
	${charon_INCLUDE_DIR}/MotionModels/LocalConstant.h
	${charon_INCLUDE_DIR}/MotionModels/LocalConstant.hxx
	${charon_INCLUDE_DIR}/MotionModel.h
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(motionmodels_lucaskanade
	MotionModelLucasKanade.cpp
	${charon_INCLUDE_DIR}/MotionModels/LucasKanade.h
	${charon_INCLUDE_DIR}/MotionModels/LucasKanade.hxx
	${charon_INCLUDE_DIR}/MotionModel.h
	PLUGIN_LISTS Plugins
)

# Iterator plugins
ADD_CHARON_PLUGIN(value
	${charon_INCLUDE_DIR}/Value.h
	${charon_INCLUDE_DIR}/Value.hxx
	Value.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(iteratorhelper
	${charon_INCLUDE_DIR}/IteratorHelper.h
	${charon_INCLUDE_DIR}/IteratorHelper.hxx
	IteratorHelper.cpp
	PLUGIN_LISTS Plugins
)
IF(QT4_FOUND)
	SET(simpleiteratorremotecontrol_HEADERS_TOMOC ${charon_INCLUDE_DIR}/SimpleIteratorRemoteControl.h)
	QT4_WRAP_CPP(simpleiteratorremotecontrol_HEADERS_MOCKED ${simpleiteratorremotecontrol_HEADERS_TOMOC})
	ADD_CHARON_PLUGIN(simpleiterator
		${charon_INCLUDE_DIR}/SimpleIterator.h
		${charon_INCLUDE_DIR}/SimpleIterator.hxx
		${simpleiteratorremotecontrol_HEADERS_MOCKED}
		SimpleIteratorRemoteControl.cpp
		SimpleIterator.cpp
		PLUGIN_LISTS Plugins
		LINK_LIBRARIES ${QT_LIBRARIES}
	)

	SET(simpleiteratorremotecontrol_HEADERS_TOMOC ${charon_INCLUDE_DIR}/SimpleIteratorRemoteControl.h)
	QT4_WRAP_CPP(simpleiteratorremotecontrol_HEADERS_MOCKED ${simpleiteratorremotecontrol_HEADERS_TOMOC})
	ADD_CHARON_PLUGIN(relaxingiterator
		${charon_INCLUDE_DIR}/RelaxingIterator.h
		${charon_INCLUDE_DIR}/RelaxingIterator.hxx
		${simpleiteratorremotecontrol_HEADERS_MOCKED}
		SimpleIteratorRemoteControl.cpp
		RelaxingIterator.cpp
		PLUGIN_LISTS Plugins
		LINK_LIBRARIES ${QT_LIBRARIES}
	)
	
ELSE(QT4_FOUND)
	ADD_CHARON_PLUGIN(simpleiterator
		${charon_INCLUDE_DIR}/SimpleIterator.h
		${charon_INCLUDE_DIR}/SimpleIterator.hxx
		SimpleIterator.cpp
		PLUGIN_LISTS Plugins
		LINK_LIBRARIES debug roi debug iteratorhelper
	)

	ADD_CHARON_PLUGIN(relaxingiterator
		${charon_INCLUDE_DIR}/RelaxingIterator.h
		${charon_INCLUDE_DIR}/RelaxingIterator.hxx
		RelaxingIterator.cpp
		PLUGIN_LISTS Plugins
		LINK_LIBRARIES debug roi debug iteratorhelper
	)
	
ENDIF(QT4_FOUND)

# Function plugins
ADD_CHARON_PLUGIN(cdfitfunction
	CDFitFunction.cpp
	${charon_INCLUDE_DIR}/CDFitFunction.h
	${charon_INCLUDE_DIR}/Functions/Function.h
		PLUGIN_LISTS Plugins
)
SET_TARGET_PROPERTIES(cdfitfunction PROPERTIES
	SOVERSION ${charon_VERSION}
)

ADD_CHARON_PLUGIN(mixtureofgaussians
	MixtureOfGaussians.cpp
	${charon_INCLUDE_DIR}/CDFitFunction.h
	${charon_INCLUDE_DIR}/MixtureOfGaussians.h
	${charon_INCLUDE_DIR}/Functions/Function.h
	${charon_INCLUDE_DIR}/Functions/DiffFunction.h
	${charon_INCLUDE_DIR}/Functions/Diff2Function.h
	${charon_INCLUDE_DIR}/Functions/Diff2LinFunction.h
	PLUGIN_LISTS Plugins
)
TARGET_LINK_LIBRARIES(mixtureofgaussians cdfitfunction)
ADD_CHARON_PLUGIN(psifunction
	PsiFunction.cpp
	${charon_INCLUDE_DIR}/PsiFunction.h
	${charon_INCLUDE_DIR}/Functions/Function.h
	${charon_INCLUDE_DIR}/Functions/DiffFunction.h
	${charon_INCLUDE_DIR}/Functions/Diff2Function.h
	${charon_INCLUDE_DIR}/Functions/Diff2LinFunction.h
	PLUGIN_LISTS Plugins
)

# sampling and CD fitter
ADD_CHARON_PLUGIN(sampler
	${charon_INCLUDE_DIR}/Sampler.h
	Sampler.cpp
	PLUGIN_LISTS Plugins
)
SET_TARGET_PROPERTIES(sampler PROPERTIES
	SOVERSION ${charon_VERSION}
)
ADD_CHARON_PLUGIN(mcmcsampler
	${charon_INCLUDE_DIR}/McmcSampler.h
	McmcSampler.cpp
	PLUGIN_LISTS Plugins
	LINK_LIBRARIES sampler
)
SET_TARGET_PROPERTIES(mcmcsampler PROPERTIES
	SOVERSION ${charon_VERSION}
)

ADD_CHARON_PLUGIN(samplegenerator
	${charon_INCLUDE_DIR}/SampleGenerator.h
	${charon_INCLUDE_DIR}/SampleGenerator.hxx
	SampleGenerator.cpp
	PLUGIN_LISTS Plugins
	LINK_LIBRARIES sampler
)

ADD_CHARON_PLUGIN(cdfitter
	${charon_INCLUDE_DIR}/CdFitter.h
	${charon_INCLUDE_DIR}/CdFitter.hxx
	CdFitter.cpp
	PLUGIN_LISTS Plugins
	LINK_LIBRARIES sampler mcmcsampler cdfitfunction
)

# Warpers
ADD_CHARON_PLUGIN(warper
	Warper.cpp
	${charon_INCLUDE_DIR}/Warper.h
	${charon_INCLUDE_DIR}/Warper.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(simplewarp
	SimpleWarp.cpp
	${charon_INCLUDE_DIR}/SimpleWarp.h
	${charon_INCLUDE_DIR}/SimpleWarp.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(warpforward
	WarpForward.cpp
	${charon_INCLUDE_DIR}/WarpForward.h
	${charon_INCLUDE_DIR}/WarpForward.hxx
	PLUGIN_LISTS Plugins
)

# Misc plugins
ADD_CHARON_PLUGIN(gnc
	Gnc.cpp
	${charon_INCLUDE_DIR}/Gnc.h
	${charon_INCLUDE_DIR}/Gnc.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(sequencegenerator
	SequenceGenerator.cpp
	${charon_INCLUDE_DIR}/SequenceGenerator.h
	${charon_INCLUDE_DIR}/SequenceGenerator.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(edgysequencegenerator
	${charon_INCLUDE_DIR}/EdgySequenceGenerator.h
	${charon_INCLUDE_DIR}/EdgySequenceGenerator.hxx
	EdgySequenceGenerator.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(diff2d
	Diff2D.cpp
	${charon_INCLUDE_DIR}/Diff2D.h
	${charon_INCLUDE_DIR}/Diff2D.hxx
	PLUGIN_LISTS Plugins
	LINK_LIBRARIES interpolatorlinear
)
ADD_CHARON_PLUGIN(simplediff
	SimpleDiff.cpp
	${charon_INCLUDE_DIR}/SimpleDiff.h
	${charon_INCLUDE_DIR}/SimpleDiff.hxx
	PLUGIN_LISTS Plugins
	LINK_LIBRARIES debug warper
)
ADD_CHARON_PLUGIN(lucas_kanade_smoothing
	LucasKanadeSmoothing.cpp
	${charon_INCLUDE_DIR}/LucasKanadeSmoothing.h
	${charon_INCLUDE_DIR}/LucasKanadeSmoothing.hxx
	PLUGIN_LISTS Plugins
)

ADD_CHARON_PLUGIN(pyramidrescale
	PyramidRescale.cpp
	${charon_INCLUDE_DIR}/PyramidRescale.h
	${charon_INCLUDE_DIR}/PyramidRescale.hxx
	PLUGIN_LISTS Plugins
	LINK_LIBRARIES interpolatorlinear roi
)
ADD_CHARON_PLUGIN(pyramidrescalematlab
	PyramidRescaleMatlab.cpp
	${charon_INCLUDE_DIR}/PyramidRescaleMatlab.h
	${charon_INCLUDE_DIR}/PyramidRescaleMatlab.hxx
	PLUGIN_LISTS Plugins
	LINK_LIBRARIES interpolatorlinear roi
)
ADD_CHARON_PLUGIN(pyramidlowpass
	PyramidLowpass.cpp
	${charon_INCLUDE_DIR}/PyramidLowpass.h
	${charon_INCLUDE_DIR}/PyramidLowpass.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(pyramidrescalemask
	PyramidRescaleMask.cpp
	${charon_INCLUDE_DIR}/PyramidRescaleMask.h
	${charon_INCLUDE_DIR}/PyramidRescaleMask.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(flowcomparator
	FlowComparator.cpp
	${charon_INCLUDE_DIR}/FlowComparator.h
	${charon_INCLUDE_DIR}/FlowComparator.hxx
	PLUGIN_LISTS Plugins
)

# unclassified plugins
ADD_CHARON_PLUGIN(selectimagefromimagelist
	${charon_INCLUDE_DIR}/SelectImageFromImageList.h
	${charon_INCLUDE_DIR}/SelectImageFromImageList.hxx
	SelectImageFromImageList.cpp
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(confidencemeasures
	${charon_INCLUDE_DIR}/ConfidenceMeasures.h
	${charon_INCLUDE_DIR}/ConfidenceMeasures.hxx
	ConfidenceMeasures.cpp
	PLUGIN_LISTS Plugins
)

# Plugin configuration (linking, common libraries)
SET(GLOBAL_RPATH "${charon-utils_ROOT_DIR}/lib${LIB_SUFFIX}/charon-plugins")
FOREACH (X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} ${CIMG_LINK_LIBRARIES})
	SET_TARGET_PROPERTIES(${X} PROPERTIES
		FOLDER "charon-flow Modules"
		INSTALL_RPATH "${GLOBAL_RPATH}"
	)
ENDFOREACH (X)
UNSET(GLOBAL_RPATH)

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
