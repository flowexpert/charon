# Test suite of application
IF(ENABLE_TEST)
	INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

	ADD_DEFINITIONS(-DTESTDIR="${CMAKE_CURRENT_SOURCE_DIR}")
	GET_TARGET_PROPERTY(Roi_LOC roi LOCATION)
	GET_FILENAME_COMPONENT(GLOBAL_PLUGIN_DIR ${Roi_LOC} PATH)
	GET_TARGET_PROPERTY(Diff2d_LOC diff2d LOCATION)
	GET_FILENAME_COMPONENT(LOCAL_PLUGIN_DIR ${Diff2d_LOC} PATH)
	ADD_DEFINITIONS(-DGLOBAL_PLUGIN_DIR="${GLOBAL_PLUGIN_DIR}")
	ADD_DEFINITIONS(-DLOCAL_PLUGIN_DIR="${LOCAL_PLUGIN_DIR}")
	#MESSAGE(STATUS "charon GLOBAL_PLUGIN_DIR: ${GLOBAL_PLUGIN_DIR}")
	#MESSAGE(STATUS "charon LOCAL_PLUGIN_DIR: ${LOCAL_PLUGIN_DIR}")

	IF(ENABLE_BLOCKMATCHING)
		# tests for imgmanip classes
		ADD_EXECUTABLE(listedpixelselectiontest listedpixelselectiontest.cpp)
		TARGET_LINK_LIBRARIES(listedpixelselectiontest
			charon-core
			${CIMG_LINK_LIBRARIES}
			roi
			filereader
		)

		ADD_EXECUTABLE(incrementorcountuptest incrementorcountuptest.cpp)
		TARGET_LINK_LIBRARIES(incrementorcountuptest
			charon-core
			${CIMG_LINK_LIBRARIES}
		)

		ADD_EXECUTABLE(objectivefunctioncomparingtest objectivefunctioncomparingtest.cpp)
		TARGET_LINK_LIBRARIES(objectivefunctioncomparingtest
			charon-core
			${CIMG_LINK_LIBRARIES}
			roi
			interpolatorlinear
			filereader
			mask1d
			linearfilter
		)

		ADD_EXECUTABLE(surfaceanalysisminchangetest surfaceanalysisminchnagetest.cpp)
		TARGET_LINK_LIBRARIES(surfaceanalysisminchangetest
			charon-core
			${CIMG_LINK_LIBRARIES}
			incrementorparameter
		)

		ADD_EXECUTABLE(blockmatchingliacstest blockmatchingliacstest.cpp)
		TARGET_LINK_LIBRARIES(blockmatchingliacstest
			charon-core
			${CIMG_LINK_LIBRARIES}
			roi
			filereader
			filewriter
			blockmatchingliacs
			surfaceanalysisminchange
			objectivefunctioncomparing
			linearfilter
			interpolatorlinear
			motionmodels_localconstant
			listedpixelselection
			mask1d
			brightnessmodels_constant
		)

		ADD_TEST(ListedPixelSelection ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/listedpixelselectiontest)
		ADD_TEST(IncrementorCountup ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/incrementorcountuptest)
		ADD_TEST(ObjectiveFunctionComparing ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectivefunctioncomparingtest)
		ADD_TEST(SurfaceAnalysisMinchange ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/surfaceanalysisminchangetest)
		ADD_TEST(BlockmatchingLiacs ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/blockmatchingliacstest)
		
		IF(WIN32)
			# copy needed dll for tests and application run before installing
			GET_TARGET_PROPERTY(CLOC charon-core LOCATION)
			GET_TARGET_PROPERTY(Filereader_LOC filereader LOCATION)
			GET_TARGET_PROPERTY(Filewriter_LOC filewriter LOCATION)
			GET_TARGET_PROPERTY(Mask1d_LOC mask1d LOCATION)
			GET_TARGET_PROPERTY(LinearFilter_LOC linearfilter LOCATION)
			GET_TARGET_PROPERTY(ChannelConverter_LOC channelconverter LOCATION)
			GET_TARGET_PROPERTY(InterpolatorLinear_LOC interpolatorlinear LOCATION)
			ADD_CUSTOM_COMMAND(TARGET POST_BUILD listedpixelselectiontest
				COMMAND ${CMAKE_COMMAND} -E copy "${CLOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Roi_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Filereader_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Filewriter_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${LinearFilter_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${ChannelConverter_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Mask1d_LOC}" "${LOCAL_PLUGIN_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${InterpolatorLinear_LOC}" "${LOCAL_PLUGIN_DIR}"
			)
		ENDIF(WIN32)
	ENDIF(ENABLE_BLOCKMATCHING)
	IF(ENABLE_SOLVERS)
		INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
		# test petsc initialization
		ADD_EXECUTABLE(petscinittest PetscInitializationTest.cpp)
		IF(MSVC)
			SET_TARGET_PROPERTIES(petscinittest
				PROPERTIES LINK_FLAGS
					/NODEFAULTLIB:"LIBCMT"
			)
		ENDIF(MSVC)
		TARGET_LINK_LIBRARIES(petscinittest ${PETSC_LIBRARIES})
		ADD_TEST(PetscInitialization ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/petscinittest)

		# checking creation of petsc plugins
		ADD_EXECUTABLE(petscplugintest PetscPluginTest.cpp)
		IF(MSVC)
			SET_TARGET_PROPERTIES(petscplugintest
				PROPERTIES LINK_FLAGS
					/NODEFAULTLIB:"LIBCMT"
			)
		ENDIF(MSVC)
		TARGET_LINK_LIBRARIES(petscplugintest
			charon-core
			charon-plugins
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
		ADD_TEST(PetscPlugin ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/petscplugintest)

		# executing test workflow
		ADD_EXECUTABLE(petscsolvertest PetscSolverTest.cpp)
		TARGET_LINK_LIBRARIES(petscsolvertest charon-core charon-plugins ${CIMG_LINK_LIBRARIES})
		ADD_TEST(Petsc_HornSchunck_Sinus ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/petscsolvertest)

		# testing Horn&Schunck Method on Yosemite Sequence
		ADD_EXECUTABLE(hornschunckyosemite HornSchunckYosemite.cpp)
		TARGET_LINK_LIBRARIES(hornschunckyosemite flowcomparator charon-plugins ${CIMG_LINK_LIBRARIES})
		ADD_TEST(Petsc_HornSchunck_Yosemite ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hornschunckyosemite)

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
	ENDIF(ENABLE_SOLVERS)
ENDIF(ENABLE_TEST)
