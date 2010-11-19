# Test suite of application
IF(BUILD_TESTING)
	INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
	INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${charon-utils_INCLUDE_DIRS})

	ADD_DEFINITIONS(-DTESTDIR="${CMAKE_CURRENT_SOURCE_DIR}")
	GET_TARGET_PROPERTY(Roi_LOC roi LOCATION)
	GET_FILENAME_COMPONENT(GLOBAL_PLUGIN_DIR ${Roi_LOC} PATH)
	GET_TARGET_PROPERTY(Diff2d_LOC diff2d LOCATION)
	GET_FILENAME_COMPONENT(LOCAL_PLUGIN_DIR ${Diff2d_LOC} PATH)
	ADD_DEFINITIONS(-DGLOBAL_PLUGIN_DIR="${GLOBAL_PLUGIN_DIR}")
	ADD_DEFINITIONS(-DLOCAL_PLUGIN_DIR="${LOCAL_PLUGIN_DIR}")

	INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
	# test petsc initialization
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

	# checking creation of petsc plugins
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

	# check loading and unloading petsc plugin
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

	IF(UNIX)
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
	ENDIF(UNIX)

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

	# testing Horn&Schunk without plugin usage to simplify profiling
	ADD_EXECUTABLE(flow3_hsYosemiteNP
		flow3_HS_Yosemite_NP.cpp
		../src/Diff2D.cpp
	)
	IF(MSVC)
		SET_TARGET_PROPERTIES(flow3_hsYosemiteNP
			PROPERTIES LINK_FLAGS
				/NODEFAULTLIB:"LIBCMT"
		)
	ENDIF(MSVC)
	TARGET_LINK_LIBRARIES(flow3_hsYosemiteNP
		charon-core
		${PETSC_LIBRARIES}
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Flow3:_HornSchunck_Yosemite_NoPlugins
		COMMAND flow3_hsYosemiteNP
	)

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
		NAME fit1_SampleGeneration
		COMMAND fit1_SampleGeneration
	)
	ADD_EXECUTABLE(fit2_CdFitter
		fit2_CdFitter.cpp
	)
	TARGET_LINK_LIBRARIES(fit2_CdFitter
		charon-plugins
		mixtureofgaussians
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME fit2_CdFitter
		COMMAND fit2_CdFitter
	)

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
		ADD_CUSTOM_COMMAND(TARGET POST_BUILD hornschunckyosemite
			COMMAND ${CMAKE_COMMAND} -E copy "${CLOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${Roi_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${Filereader_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${Filewriter_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${LinearFilter_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${ChannelConverter_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${Mask1d_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${Crop_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${InterpolatorLinear_LOC}" "${LOCAL_PLUGIN_DIR}"
		)
	ENDIF(WIN32)
ENDIF(BUILD_TESTING)
