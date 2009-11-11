# Test suite of application
INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

IF(ENABLE_TEST)
	# common definitions and test file configuration
	SET(PENGUINFILE "${CMAKE_CURRENT_SOURCE_DIR}/Penguin.pgm")
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/sample.wrp.in ${CMAKE_CURRENT_BINARY_DIR}/sample.wrp @ONLY)
	ADD_DEFINITIONS(-DTESTWRPFILE="${CMAKE_CURRENT_BINARY_DIR}/sample.wrp")
	ADD_DEFINITIONS(-DPENGUINFILE="${PENGUINFILE}")

	# tests for imgmanip classes
	ADD_EXECUTABLE(imgManipTest imgmaniptest.cpp)
	TARGET_LINK_LIBRARIES(imgManipTest charon-plugins ${CIMG_LINK_LIBRARIES})
	ADD_TEST(ImgManipTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imgManipTest)

	# tests for ImgTool routines
	ADD_EXECUTABLE(imgToolTest imgToolTest.cpp ../src/ImgTool.h ../src/ImgTool.hxx)
	TARGET_LINK_LIBRARIES(imgToolTest charon-core ${CIMG_LINK_LIBRARIES})
	ADD_TEST(ImgToolTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imgToolTest)
	
	#tests for plugin creation in various ways
	ADD_EXECUTABLE(pluginTest pluginTest.cpp)
	TARGET_LINK_LIBRARIES(pluginTest charon-core charon-plugins roi filereader ${CIMG_LINK_LIBRARIES})
	ADD_TEST(PluginTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pluginTest)

	ADD_EXECUTABLE(cImgTest cimgtest.cpp)
	TARGET_LINK_LIBRARIES(cImgTest ${CIMG_LINK_LIBRARIES})
	ADD_TEST(CImgTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cImgTest)

	IF(WIN32)
		# copy needed dll for tests and application run before installing
		GET_TARGET_PROPERTY(CLOC charon-core LOCATION)
		GET_TARGET_PROPERTY(PLOC imgManipTest LOCATION)
		GET_FILENAME_COMPONENT(PLOC_DIR ${PLOC} PATH)

		ADD_CUSTOM_COMMAND(TARGET imgToolTest POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CLOC}" "${PLOC_DIR}"
		)
	ENDIF(WIN32)
ENDIF(ENABLE_TEST)
