# Test suite of application
IF(BUILD_TESTING)
	FIND_PACKAGE(Gnuplot)
	INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
	INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${charon-utils_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${CIMG_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${QT_INCLUDE_DIRS})
	IF(USE_PETSC)
		INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
	ENDIF()

	ADD_DEFINITIONS(-DTESTDIR="${CMAKE_CURRENT_SOURCE_DIR}")
	GET_TARGET_PROPERTY(Roi_LOC roi LOCATION)
	GET_FILENAME_COMPONENT(GLOBAL_PLUGIN_DIR ${Roi_LOC} PATH)
	GET_TARGET_PROPERTY(Diff2d_LOC diff2d LOCATION)
	GET_FILENAME_COMPONENT(LOCAL_PLUGIN_DIR ${Diff2d_LOC} PATH)
	ADD_DEFINITIONS(-DGLOBAL_PLUGIN_DIR="${GLOBAL_PLUGIN_DIR}")
	ADD_DEFINITIONS(-DLOCAL_PLUGIN_DIR="${LOCAL_PLUGIN_DIR}")
	SET(tests)
	SET(testTargets)

	# test petsc initialization
	IF(USE_PETSC)
		ADD_EXECUTABLE(petsc1_init petsc1_Init.cpp)
		IF(MSVC)
			SET_TARGET_PROPERTIES(petsc1_init
				PROPERTIES LINK_FLAGS
					/NODEFAULTLIB:"LIBCMT"
			)
		ENDIF(MSVC)
		TARGET_LINK_LIBRARIES(petsc1_init ${PETSC_LIBRARIES})
		ADD_TEST(
			NAME PETSc1:_PetscInitialization
			COMMAND petsc1_init
		)
		LIST(APPEND tests PETSc1:_PetscInitialization)
		LIST(APPEND testTargets petsc1_init)
	ENDIF(USE_PETSC)

	# checking creation of petsc plugins
	IF(USE_PETSC)
		ADD_EXECUTABLE(petsc2_plugins petsc2_Plugins.cpp)
		IF(MSVC)
			SET_TARGET_PROPERTIES(petsc2_plugins
				PROPERTIES LINK_FLAGS
					/NODEFAULTLIB:"LIBCMT"
			)
		ENDIF(MSVC)
		TARGET_LINK_LIBRARIES(petsc2_plugins
			charon-core
			${CIMG_LINK_LIBRARIES}
			roi
			gbcce
			l2norm
			brightnessmodels_constant
			motionmodels_localconstant
			channelconverter
			filereader
			filewriter
			mask1d
			linearfilter
			petscsolver
		)
		ADD_TEST(
			NAME PETSc2:_ModuleCreation
			COMMAND petsc2_plugins
		)
		LIST(APPEND tests PETSc2:_ModuleCreation)
		LIST(APPEND testTargets petsc2_plugins)
	ENDIF(USE_PETSC)

	# check loading and unloading petsc plugin
	IF(USE_PETSC)
		ADD_EXECUTABLE(petsc3_pluginLoad petsc3_PluginLoad.cpp)
		TARGET_LINK_LIBRARIES(petsc3_pluginLoad
			charon-core
			charon-plugins
			${CIMG_LINK_LIBRARIES}
		)
		ADD_TEST(
			NAME PETSc3:_Load_PetscSolver_SharedLib
			COMMAND petsc3_pluginLoad
		)
		LIST(APPEND tests PETSc3:_Load_PetscSolver_SharedLib)
		LIST(APPEND testTargets petsc3_pluginLoad)
	ENDIF(USE_PETSC)

	IF(UNIX AND USE_PETSC)
		# check loading and unloading petsc shared library
		# this is known to fail e.g. using the ubuntu package
		# petsc 3.0.0.dfsg-6ubuntu1
		# use a more recent version in this case,
		# e.g. from the charon-suite ppa (see
		# https://launchpad.net/~jmgottfried/+archive/charon)
		ADD_LIBRARY(petsc4_lib SHARED petsc4_PetscSharedLib.cpp)
		TARGET_LINK_LIBRARIES(petsc4_lib
			${PETSC_LIBRARIES}
		)
		ADD_EXECUTABLE(petsc4_libLoad petsc4_PetscSharedLibLoad.cpp)
		TARGET_LINK_LIBRARIES(petsc4_libLoad
			dl
		)
		ADD_TEST(
			NAME PETSc4:_Load_PETSc_SharedLib
			COMMAND petsc4_libLoad
		)
		LIST(APPEND tests PETSc4:_Load_PETSc_SharedLib)
		LIST(APPEND testTargets petsc4_libLoad)
	ENDIF(UNIX AND USE_PETSC)

	# executing test workflow
	ADD_EXECUTABLE(flow1_hsSinus flow1_HS_Sinus.cpp)
	TARGET_LINK_LIBRARIES(flow1_hsSinus
		charon-core
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Flow1:_HornSchunck_Sinus
		COMMAND flow1_hsSinus
	)
	LIST(APPEND tests Flow1:_HornSchunck_Sinus)
	LIST(APPEND testTargets flow1_hsSinus)

	# testing Horn&Schunck Method on Yosemite Sequence
	ADD_EXECUTABLE(flow2_hsYosemite flow2_HS_Yosemite.cpp)
	TARGET_LINK_LIBRARIES(flow2_hsYosemite
		flowcomparator
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Flow2:_HornSchunck_Yosemite
		COMMAND flow2_hsYosemite
	)
	LIST(APPEND tests Flow2:_HornSchunck_Yosemite)
	LIST(APPEND testTargets flow2_hsYosemite)

	# testing Horn&Schunk without plugin usage to simplify profiling
	IF(USE_PETSC)
		ADD_EXECUTABLE(flow3_hsYosemiteNP
			flow3_HS_Yosemite_NP.cpp
		)
		IF(MSVC)
			SET_TARGET_PROPERTIES(flow3_hsYosemiteNP
				PROPERTIES LINK_FLAGS
					/NODEFAULTLIB:"LIBCMT"
			)
		ENDIF(MSVC)
		TARGET_LINK_LIBRARIES(flow3_hsYosemiteNP
			charon-core
			filereader # needs FileReaderWatcher
			${PETSC_LIBRARIES}
			${CIMG_LINK_LIBRARIES}
		)
		ADD_TEST(
			NAME Flow3:_HornSchunck_Yosemite_NoPlugins
			COMMAND flow3_hsYosemiteNP
		)
		LIST(APPEND tests Flow3:_HornSchunck_Yosemite_NoPlugins)
		LIST(APPEND testTargets flow3_hsYosemiteNP)
	ENDIF(USE_PETSC)

	# tests for iteration plugins
	ADD_EXECUTABLE(iterator1_simple iterator1_simple.cpp)
	TARGET_LINK_LIBRARIES(iterator1_simple
		simpleiterator
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Iterations1:_SimpleIterator
		COMMAND iterator1_simple
	)
	LIST(APPEND tests Iterations1:_SimpleIterator)
	LIST(APPEND testTargets iterator1_simple)

	ADD_EXECUTABLE(iterator2_nested iterator2_nested.cpp)
	TARGET_LINK_LIBRARIES(iterator2_nested
		simpleiterator
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Iterations2:_Nested_Iterators
		COMMAND iterator2_nested
	)
	LIST(APPEND tests Iterations2:_Nested_Iterators)
	LIST(APPEND testTargets iterator2_nested)

	ADD_EXECUTABLE(iterator3_relaxing iterator3_relaxing.cpp)
	TARGET_LINK_LIBRARIES(iterator3_relaxing
		charon-plugins
		relaxingiterator
		l2norm
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Iterations3:_RelaxingIterator
		COMMAND iterator3_relaxing
	)
	LIST(APPEND tests Iterations3:_RelaxingIterator)
	LIST(APPEND testTargets iterator3_relaxing)

	# iterative flow estimation
	ADD_EXECUTABLE(flowIterated1_HS_Sinus flowIterated1_HS_Sinus.cpp)
	TARGET_LINK_LIBRARIES(flowIterated1_HS_Sinus
		flowcomparator
		simpleiterator
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME FlowIterated1:_HornSchunck_Sinus
		COMMAND flowIterated1_HS_Sinus
	)
	LIST(APPEND tests FlowIterated1:_HornSchunck_Sinus)
	LIST(APPEND testTargets flowIterated1_HS_Sinus)

	# CG solver HS flow estimation
	ADD_EXECUTABLE(cgflow_HS cgflow_HS.cpp)
	TARGET_LINK_LIBRARIES(cgflow_HS
		flowcomparator
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME CgFlow:_HornSchunck
		COMMAND cgflow_HS
		CONFIGURATIONS Release
	)
	LIST(APPEND tests CgFlow:_HornSchunck)
	LIST(APPEND testTargets cgflow_HS)

	ADD_EXECUTABLE(flowIterated2_HS_Yosemite
		flowIterated2_HS_Yosemite.cpp)
	TARGET_LINK_LIBRARIES(flowIterated2_HS_Yosemite
		flowcomparator
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME FlowIterated2:_HornSchunck_Yosemite
		COMMAND flowIterated2_HS_Yosemite
	)
	LIST(APPEND tests FlowIterated2:_HornSchunck_Yosemite)
	LIST(APPEND testTargets flowIterated2_HS_Yosemite)

	ADD_EXECUTABLE(flowIterated3_DataConstant flowIterated3_DataConstant.cpp)
	TARGET_LINK_LIBRARIES(flowIterated3_DataConstant
		relaxingiterator
		flowcomparator
		charon-plugins
		${CIMG_LINK_LIBRARIES})
	ADD_TEST(
		NAME FlowIterated3:_DataConstant
		CONFIGURATIONS Release
		COMMAND flowIterated3_DataConstant
	)
	LIST(APPEND tests FlowIterated3:_DataConstant)
	LIST(APPEND testTargets flowIterated3_DataConstant)

	ADD_EXECUTABLE(flowIterated4_SpatialPW flowIterated4_SpatialPW.cpp)
	TARGET_LINK_LIBRARIES(flowIterated4_SpatialPW
		relaxingiterator
		flowcomparator
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME FlowIterated4:_SpatialPW
		CONFIGURATIONS Release
		COMMAND flowIterated4_SpatialPW
	)
	LIST(APPEND tests FlowIterated4:_SpatialPW)
	LIST(APPEND testTargets flowIterated4_SpatialPW)

	ADD_EXECUTABLE(flowIterated5_Papenberg flowIterated5_Papenberg.cpp)
	TARGET_LINK_LIBRARIES(flowIterated5_Papenberg
		relaxingiterator
		flowcomparator
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME FlowIterated5:_Papenberg
		CONFIGURATIONS Release
		COMMAND flowIterated5_Papenberg
	)
	LIST(APPEND tests FlowIterated5:_Papenberg)
	LIST(APPEND testTargets flowIterated5_Papenberg)

	ADD_EXECUTABLE(flowIterated6_Pyramid flowIterated6_Pyramid.cpp)
	TARGET_LINK_LIBRARIES(flowIterated6_Pyramid
		iteratorhelper
		simpleiterator
		relaxingiterator
		flowcomparator
		pyramidrescale
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME FlowIterated6:_Pyramid
		CONFIGURATIONS Release
		COMMAND flowIterated6_Pyramid
	)
	LIST(APPEND tests FlowIterated6:_Pyramid)
	LIST(APPEND testTargets flowIterated6_Pyramid)

	# Sample generation and Contrastive Divergence fitter
	ADD_EXECUTABLE(fit1_SampleGeneration
		fit1_SampleGeneration.cpp
	)
	TARGET_LINK_LIBRARIES(fit1_SampleGeneration
		charon-plugins
		filewriter
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Fit1:_SampleGeneration
		COMMAND fit1_SampleGeneration
	)
	LIST(APPEND tests Fit1:_SampleGeneration)
	LIST(APPEND testTargets fit1_SampleGeneration)

	ADD_EXECUTABLE(fit2_CdFitter
		fit2_CdFitter.cpp
	)
	TARGET_LINK_LIBRARIES(fit2_CdFitter
		charon-plugins
		mixtureofgaussians
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Fit2:_CdFitter
		COMMAND fit2_CdFitter
	)
	LIST(APPEND tests Fit2:_CdFitter)
	LIST(APPEND testTargets fit2_CdFitter)

	ADD_EXECUTABLE(fit3_GSM
		fit3_GSM.cpp
		fit3_GSM.plt
	)
	TARGET_LINK_LIBRARIES(fit3_GSM
		charon-plugins
		mixtureofgaussians
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Fit3:_GSM
		COMMAND fit3_GSM
	)
	LIST(APPEND tests Fit3:_GSM)
	LIST(APPEND testTargets fit3_GSM)
	IF(GNUPLOT_FOUND)
		CONFIGURE_FILE(fit3_GSM.plt fit3_GSM.plt COPYONLY)
		ADD_TEST(
			NAME Fit3:_GSM_plot
			COMMAND "${GNUPLOT_EXECUTABLE}" fit3_GSM.plt
		)
		SET_TESTS_PROPERTIES(Fit3:_GSM_plot PROPERTIES DEPENDS Fit3:_GSM)
		LIST(APPEND tests Fit3:_GSM_plot)
	ENDIF(GNUPLOT_FOUND)

	IF(WIN32)
		# copy needed dll for tests and application run before installing
		GET_TARGET_PROPERTY(CLOC charon-core LOCATION)
		GET_TARGET_PROPERTY(Roi_LOC roi LOCATION)
		GET_TARGET_PROPERTY(Filereader_LOC filereader LOCATION)
		GET_TARGET_PROPERTY(Filewriter_LOC filewriter LOCATION)
		GET_TARGET_PROPERTY(Mask1d_LOC mask1d LOCATION)
		GET_TARGET_PROPERTY(LinearFilter_LOC linearfilter LOCATION)
		GET_TARGET_PROPERTY(ChannelConverter_LOC channelconverter LOCATION)
		GET_TARGET_PROPERTY(InterpolatorLinear_LOC interpolatorlinear LOCATION)
		GET_TARGET_PROPERTY(Crop_LOC crop LOCATION)
		IF(NOT SKIP_LOCAL_DLL_COPY)
			ADD_CUSTOM_COMMAND(TARGET POST_BUILD flow1_hsSinus
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CLOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Roi_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Filereader_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Filewriter_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${LinearFilter_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${ChannelConverter_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Mask1d_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Crop_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${InterpolatorLinear_LOC}" "${LOCAL_PLUGIN_DIR}"
			)
		ENDIF(NOT SKIP_LOCAL_DLL_COPY)
		FOREACH(X ${ADDITIONAL_DLL_DEPENDS})
			ADD_CUSTOM_COMMAND(TARGET POST_BUILD flow1_hsSinus
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${X}" "${LOCAL_PLUGIN_DIR}"
			)
		ENDFOREACH(X)
	ENDIF(WIN32)

	# add test labels for dashboard submission
	SET_TESTS_PROPERTIES(${tests} PROPERTIES LABELS charon)
	SET_TARGET_PROPERTIES(${testTargets} PROPERTIES FOLDER "charon-flow Tests")
ENDIF(BUILD_TESTING)
