# Test suite of application
IF(ENABLE_TEST)
	INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

	ADD_DEFINITIONS(-DPENGUINFILE="${CMAKE_CURRENT_SOURCE_DIR}/Penguin.cimg")
	ADD_DEFINITIONS(-DPENGUINHEADFILE="${CMAKE_CURRENT_SOURCE_DIR}/sequence.cimg")
	ADD_DEFINITIONS(-DTESTFILE="${CMAKE_CURRENT_SOURCE_DIR}/test.cimg")

	IF(ENABLE_BLOCKMATCHING)
		# tests for imgmanip classes
		ADD_EXECUTABLE(listedpixelselectiontest listedpixelselectiontest.cpp)
		TARGET_LINK_LIBRARIES(listedpixelselectiontest
			charon-core
			roi
			filereader
		)

		ADD_EXECUTABLE(incrementorcountuptest incrementorcountuptest.cpp)
		TARGET_LINK_LIBRARIES(incrementorcountuptest
			charon-core
		)

		ADD_EXECUTABLE(objectivefunctioncomparingtest objectivefunctioncomparingtest.cpp)
		TARGET_LINK_LIBRARIES(objectivefunctioncomparingtest
			charon-core
			roi
			interpolatorlinear
			filereader
			mask1d
			linearfilter
		)

		ADD_EXECUTABLE(surfaceanalysisminchangetest surfaceanalysisminchnagetest.cpp)
		TARGET_LINK_LIBRARIES(surfaceanalysisminchangetest
			charon-core
			incrementorparameter
		)

		ADD_EXECUTABLE(blockmatchingliacstest blockmatchingliacstest.cpp)
		TARGET_LINK_LIBRARIES(blockmatchingliacstest
			charon-core
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

		IF(UNIX)
			TARGET_LINK_LIBRARIES(listedpixelselectiontest pthread ${X11_LIBRARIES})
			TARGET_LINK_LIBRARIES(incrementorcountuptest pthread ${X11_LIBRARIES})
			TARGET_LINK_LIBRARIES(objectivefunctioncomparingtest pthread ${X11_LIBRARIES})
			TARGET_LINK_LIBRARIES(surfaceanalysisminchangetest pthread ${X11_LIBRARIES})
			TARGET_LINK_LIBRARIES(blockmatchingliacstest pthread ${X11_LIBRARIES})
		ENDIF(UNIX)
		ADD_TEST(ListedPixelSelectionTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/listedpixelselectiontest)
		ADD_TEST(IncrementorCountupTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/incrementorcountuptest)
		ADD_TEST(ObjectiveFunctionComparingTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectivefunctioncomparingtest)
		ADD_TEST(SurfaceAnalysisMinchangeTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/surfaceanalysisminchangetest)
		ADD_TEST(BlockmatchingLiacstest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/blockmatchingliacstest)
		
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
			GET_TARGET_PROPERTY(PLOC listedpixelselectiontest LOCATION)
			GET_FILENAME_COMPONENT(PLOC_DIR ${PLOC} PATH)
			ADD_CUSTOM_COMMAND(TARGET POST_BUILD listedpixelselectiontest
				COMMAND ${CMAKE_COMMAND} -E copy "${CLOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Roi_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Filereader_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Filewriter_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${LinearFilter_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${ChannelConverter_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Mask1d_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${InterpolatorLinear_LOC}" "${PLOC_DIR}"
			)
		ENDIF(WIN32)
	ENDIF(ENABLE_BLOCKMATCHING)
	IF(ENABLE_SOLVERS)
		INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
		# tests for solver classes
		ADD_EXECUTABLE(petscsolvertest PetscSolverTest.cpp)
		TARGET_LINK_LIBRARIES(petscsolvertest
			charon-core
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
		IF(UNIX)
			TARGET_LINK_LIBRARIES(petscsolvertest pthread ${X11_LIBRARIES})
		ENDIF(UNIX)
		ADD_TEST(PetscSolverTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/petscsolvertest)
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
			GET_TARGET_PROPERTY(PLOC petscsolvertest LOCATION)
			GET_FILENAME_COMPONENT(PLOC_DIR ${PLOC} PATH)
			ADD_CUSTOM_COMMAND(TARGET POST_BUILD petscsolvertest
				COMMAND ${CMAKE_COMMAND} -E copy "${CLOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Roi_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Filereader_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Filewriter_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${LinearFilter_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${ChannelConverter_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${Mask1d_LOC}" "${PLOC_DIR}"
				COMMAND ${CMAKE_COMMAND} -E copy "${InterpolatorLinear_LOC}" "${PLOC_DIR}"
			)
		ENDIF(WIN32)
	ENDIF(ENABLE_SOLVERS)
ENDIF(ENABLE_TEST)
