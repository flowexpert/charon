# Test suite of application
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CIMG_INCLUDE_DIRS})
SET(CHARON_UTILS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/charon-utils)

IF(BUILD_TESTING)
	# common definitions and test file configuration
	SET(PENGUINFILE "${CMAKE_CURRENT_SOURCE_DIR}/Penguin.pgm")
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/sample.wrp.in
		${CMAKE_CURRENT_BINARY_DIR}/sample.wrp
		@ONLY
	)
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/Warptest.wrp.in
		${CMAKE_CURRENT_BINARY_DIR}/Warptest.wrp
		@ONLY
	)
	ADD_DEFINITIONS(-DTESTWRPFILE="${CMAKE_CURRENT_BINARY_DIR}/sample.wrp")
	ADD_DEFINITIONS(-DWARPWRPFILE="${CMAKE_CURRENT_BINARY_DIR}/Warptest.wrp")
	ADD_DEFINITIONS(-DWARPTESTWRPFILE="${CMAKE_CURRENT_BINARY_DIR}/sample.wrp")
	ADD_DEFINITIONS(-DPENGUINFILE="${PENGUINFILE}")
	
	ADD_DEFINITIONS(-DTESTDIR="${CMAKE_CURRENT_SOURCE_DIR}")
	IF(WIN32)
		ADD_DEFINITIONS(-DGLOBAL_PLUGIN_DIR="${CHARON_UTILS_ROOT_DIR}/bin")
		ADD_DEFINITIONS(-DLOCAL_PLUGIN_DIR="${PROJECT_BINARY_DIR}/bin")
	ELSE(WIN32)
		ADD_DEFINITIONS(-DGLOBAL_PLUGIN_DIR="${CHARON_UTILS_ROOT_DIR}/lib${LIB_SUFFIX}/charon-plugins")
		ADD_DEFINITIONS(-DLOCAL_PLUGIN_DIR="${PROJECT_BINARY_DIR}/src")
	ENDIF(WIN32)

	SET(tests)

	# tests for imgmanip classes
	ADD_EXECUTABLE(imgManipTest imgmaniptest.cpp)
	TARGET_LINK_LIBRARIES(imgManipTest
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME ImageManip
		COMMAND imgManipTest
	)
	LIST(APPEND tests ImageManip)

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
	
	#tests for plugin creation in various ways
	ADD_EXECUTABLE(pluginTest pluginTest.cpp)
	TARGET_LINK_LIBRARIES(pluginTest
		charon-core
		charon-plugins
		roi
		filereader
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Plugins
		COMMAND pluginTest
	)
	LIST(APPEND tests Plugins)

	ADD_EXECUTABLE(cImgTest cimgtest.cpp)
	TARGET_LINK_LIBRARIES(cImgTest ${CIMG_LINK_LIBRARIES})
	ADD_TEST(
		NAME CImgInfo
		COMMAND cImgTest
	)
	LIST(APPEND tests CImgInfo)
	
	ADD_EXECUTABLE(warptest Warptest.cpp)
	TARGET_LINK_LIBRARIES(warptest
		${CIMG_LINK_LIBRARIES}
		filereader
		warp
		interpolatorlinear
		charon-plugins
	)
	ADD_TEST(
		NAME Warp
		COMMAND warptest
	)
	LIST(APPEND tests Warp)

	IF(USE_VIGRA)
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
	ENDIF(USE_VIGRA)

	IF(WIN32)
		# copy needed dll for tests and application run before installing
		GET_TARGET_PROPERTY(CLOC charon-core LOCATION)
		GET_TARGET_PROPERTY(PLOC imgManipTest LOCATION)
		GET_FILENAME_COMPONENT(PLOC_DIR ${PLOC} PATH)

		ADD_CUSTOM_COMMAND(TARGET imgToolTest POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CLOC}" "${PLOC_DIR}"
		)
	ENDIF(WIN32)

	# add test labels for dashboard submission
	SET_TESTS_PROPERTIES(${tests}
		PROPERTIES LABELS charon-utils
	)
ENDIF(BUILD_TESTING)
