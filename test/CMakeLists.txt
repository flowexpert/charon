# Test suite of application
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
SET(CHARON_UTILS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/charon-utils)

IF(BUILD_TESTING)
	# common definitions and test file configuration
	SET(PENGUINFILE "${CMAKE_CURRENT_SOURCE_DIR}/Penguin.pgm")
	SET(PENGUINJPGFILE "${CMAKE_CURRENT_SOURCE_DIR}/Penguin.jpg")
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/Warptest.wrp.in
		${CMAKE_CURRENT_BINARY_DIR}/Warptest.wrp
		@ONLY
	)
	ADD_DEFINITIONS(-DWARPWRPFILE="${CMAKE_CURRENT_BINARY_DIR}/Warptest.wrp")
	ADD_DEFINITIONS(-DWARPTESTWRPFILE="${CMAKE_CURRENT_BINARY_DIR}/sample.wrp")
	ADD_DEFINITIONS(-DPENGUINFILE="${PENGUINFILE}")
	ADD_DEFINITIONS(-DPENGUINJPGFILE="${PENGUINJPGFILE}")
	
	ADD_DEFINITIONS(-DTESTDIR="${CMAKE_CURRENT_SOURCE_DIR}")
	IF(WIN32)
		ADD_DEFINITIONS(-DGLOBAL_PLUGIN_DIR="${CHARON_UTILS_ROOT_DIR}/bin")
		ADD_DEFINITIONS(-DLOCAL_PLUGIN_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
	ELSE(WIN32)
		ADD_DEFINITIONS(-DGLOBAL_PLUGIN_DIR="${CHARON_UTILS_ROOT_DIR}/lib${LIB_SUFFIX}/charon-plugins")
		ADD_DEFINITIONS(-DLOCAL_PLUGIN_DIR="${PROJECT_BINARY_DIR}/src")
	ENDIF(WIN32)

	SET(tests)
	SET(testTargets)

	IF(USE_CIMG)
		IF(USE_PNG)
			ADD_EXECUTABLE(pngtest pngtest.cpp)
			TARGET_LINK_LIBRARIES(pngtest ${CIMG_LINK_LIBRARIES})
			ADD_TEST(NAME CImg:PNG COMMAND pngtest)
			LIST(APPEND tests CImg:PNG)
			LIST(APPEND testTargets pngtest)
		ENDIF(USE_PNG)

		INCLUDE_DIRECTORIES(${CIMG_INCLUDE_DIRS})

		# test loading and unloading some plugins
		ADD_EXECUTABLE(pluginTest plugintest.cpp)
		TARGET_LINK_LIBRARIES(pluginTest
			charon-plugins
		)
		ADD_TEST(
			NAME PluginTest
			COMMAND pluginTest
		)
		LIST(APPEND tests PluginTest)
		LIST(APPEND testTargets pluginTest)

		# tests for imgmanip classes
		ADD_EXECUTABLE(imgManipTest imgmaniptest.cpp)
		TARGET_LINK_LIBRARIES(imgManipTest
			charon-plugins
			filewriter
			${CIMG_LINK_LIBRARIES}
		)
		ADD_TEST(
			NAME ImageManip
			COMMAND imgManipTest
		)
		LIST(APPEND tests ImageManip)
		LIST(APPEND testTargets imgManipTest)

		# tests for ImgTool routines
		ADD_EXECUTABLE(imgToolTest
			imgToolTest.cpp
			${CHARON_UTILS_INCLUDE_DIR}/ImgTool.h
			${CHARON_UTILS_INCLUDE_DIR}/ImgTool.hxx
		)
		TARGET_LINK_LIBRARIES(imgToolTest
			charon-core
			${CIMG_LINK_LIBRARIES}
		)
		ADD_TEST(
			NAME ImgTool
			COMMAND imgToolTest
		)
		LIST(APPEND tests ImgTool)
		LIST(APPEND testTargets imgToolTest)

		ADD_EXECUTABLE(cImgTest
			cimgtest.cpp
			${CHARON_UTILS_INCLUDE_DIR}/CImg.h
		)
		TARGET_LINK_LIBRARIES(cImgTest ${CIMG_LINK_LIBRARIES})
		ADD_TEST(
			NAME CImg:Info
			COMMAND cImgTest
		)
		LIST(APPEND tests CImg:Info)
		LIST(APPEND testTargets cImgTest)

		ADD_EXECUTABLE(warptest Warptest.cpp)
		TARGET_LINK_LIBRARIES(warptest
			${CIMG_LINK_LIBRARIES}
			filereader
			warp
			warpsymmetric
			charon-plugins
		)
		ADD_TEST(
			NAME Warp
			COMMAND warptest
		)
		LIST(APPEND tests Warp)
		LIST(APPEND testTargets warptest)

		ADD_EXECUTABLE(flowToHSV flowToHSV.cpp)
		TARGET_LINK_LIBRARIES(flowToHSV
			${CIMG_LINK_LIBRARIES}
			charon-plugins
			flow2hsv
			filereader
		)
		ADD_TEST(
			NAME flowToHSV
			COMMAND flowToHSV
		)
		LIST(APPEND tests flowToHSV)
		LIST(APPEND testTargets flowToHSV)

		IF(USE_VIGRA)
			INCLUDE_DIRECTORIES(${Vigra_INCLUDE_DIRS})
			ADD_EXECUTABLE(vigraconversion vigraConversion.cpp)
			TARGET_LINK_LIBRARIES(vigraconversion
				${CIMG_LINK_LIBRARIES}
				${VIGRA_LIBRARIES}
				filereader
				cimg2vigramultiarray
				vigramultiarray2cimg
			)
			ADD_TEST(
				NAME VigraConversion
				COMMAND vigraconversion
			)
			LIST(APPEND tests VigraConversion)
			LIST(APPEND testTargets vigraconversion)

			IF(USE_HDF5)
				INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIRS})

				# tests for input/output slot data passing including vigra
				# and cimg arrays
				ADD_EXECUTABLE(datapasstest2
					dataPassing.cpp
					${CHARON_UTILS_INCLUDE_DIR}/DataManagerCImgList.hxx
					${CHARON_UTILS_INCLUDE_DIR}/DataManagerVigraArray.hxx
				)
				TARGET_LINK_LIBRARIES(datapasstest2
					charon-core charon-plugins
					${CIMG_LINK_LIBRARIES}
					${VIGRA_LIBRARIES}
					${HDF5_LIBRARIES}
				)
				IF(USE_MPI)
					TARGET_LINK_LIBRARIES(datapasstest2 ${MPI_LIBRARIES})
				ENDIF()
				ADD_TEST(
						NAME SlotDataPassing_CimgVigra
						COMMAND datapasstest2
				)
				LIST(APPEND tests SlotDataPassing_CimgVigra)
				LIST(APPEND testTargets datapasstest2)
			ENDIF(USE_HDF5)
		ENDIF(USE_VIGRA)

		IF(WIN32)
			# copy needed dll for tests and application run before installing
			GET_TARGET_PROPERTY(CLOC charon-core LOCATION)
			GET_TARGET_PROPERTY(PLOC pluginTest LOCATION)
			GET_FILENAME_COMPONENT(PLOC_DIR ${PLOC} PATH)

			IF(NOT SKIP_LOCAL_DLL_COPY)
				ADD_CUSTOM_COMMAND(TARGET pluginTest POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CLOC}" "${PLOC_DIR}"
				)
			ENDIF(NOT SKIP_LOCAL_DLL_COPY)
			FOREACH(X ${ADDITIONAL_DLL_DEPENDS})
				ADD_CUSTOM_COMMAND(TARGET POST_BUILD pluginTest
					COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${X}" "${PLOC_DIR}"
				)
			ENDFOREACH(X)
		ENDIF(WIN32)

		# temporarily disabled because of group refactoring
		IF(FALSE)
			ADD_EXECUTABLE(groupVirtualSlot2
				groupVirtualSlot.cpp
				${CHARON_UTILS_INCLUDE_DIR}/DataManagerCImgList.hxx

			)
			TARGET_LINK_LIBRARIES(groupVirtualSlot2
				charon-core charon-plugins
				${CIMG_LINK_LIBRARIES}

			)
			ADD_TEST(
					NAME groupVirtualSlot_CimgVigra
					COMMAND datapasstest2
			)
			LIST(APPEND tests groupVirtualSlot_CimgVigra)
			LIST(APPEND testTargets groupVirtualSlot2)
		ENDIF()
	ENDIF(USE_CIMG)

	# charon-xml-helper test
	IF(USE_QT)
		ADD_TEST(
			NAME XmlHelper
			COMMAND charon-xml-helper "${CMAKE_CURRENT_SOURCE_DIR}"
		)
		SET_TESTS_PROPERTIES(XmlHelper PROPERTIES PASS_REGULAR_EXPRESSION
			"Modules.*Inter.*Inter.*Non.*std.*std"
		)
		ADD_TEST(
			NAME XmlHelperXCheck
			COMMAND charon-xml-helper "${CMAKE_CURRENT_SOURCE_DIR}" --xcheck
		)
		SET_TESTS_PROPERTIES(XmlHelperXCheck PROPERTIES PASS_REGULAR_EXPRESSION
			"^I.*ct:[\n\t\r]*InvalidModule[\n\t\r]*P.*p:[\n\t\r ]*Interpolator[\n\t\r ]*$"
		)
		LIST(APPEND tests XmlHelper XmlHelperXCheck)
	ENDIF(USE_QT)

	# add test labels for dashboard submission
	SET_TESTS_PROPERTIES(${tests} PROPERTIES LABELS charon-utils)
	SET_TARGET_PROPERTIES(${testTargets} PROPERTIES FOLDER "charon-utils Tests")
ENDIF(BUILD_TESTING)
