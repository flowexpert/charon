# Test suite of application
INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

IF(ENABLE_TEST)
	# common definitions
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/sample.wrp.in ${CMAKE_CURRENT_BINARY_DIR}/sample.wrp @ONLY)
	ADD_DEFINITIONS(-DTESTWRPFILE="${CMAKE_CURRENT_BINARY_DIR}/sample.wrp")
	ADD_DEFINITIONS(-DPENGUINFILE="${CMAKE_CURRENT_SOURCE_DIR}/Penguin.pgm")

	# tests for imgmanip classes
	ADD_EXECUTABLE(imgManipTest imgmaniptest.cpp)
	TARGET_LINK_LIBRARIES(imgManipTest charon-plugins)
	IF(UNIX)
		TARGET_LINK_LIBRARIES(imgManipTest pthread ${X11_LIBRARIES})
	ENDIF(UNIX)

	# tests for ImgTool routines
	ADD_EXECUTABLE(imgToolTest imgToolTest.cpp)
	TARGET_LINK_LIBRARIES(imgToolTest charon-core)
	IF(UNIX)
		TARGET_LINK_LIBRARIES(imgToolTest pthread ${X11_LIBRARIES})
	ENDIF(UNIX)
	
	#tests for plugin creatin in various ways
	ADD_EXECUTABLE(pluginTest pluginTest.cpp)
	TARGET_LINK_LIBRARIES(pluginTest charon-core charon-plugins roi filereader)
	IF(UNIX)
		TARGET_LINK_LIBRARIES(pluginTest pthread ${X11_LIBRARIES})
	ENDIF(UNIX)


	IF(WIN32)
		# copy needed dll for tests and application run before installing
		GET_TARGET_PROPERTY(CLOC charon-core LOCATION)
		GET_TARGET_PROPERTY(PLOC imgmaniptest LOCATION)
		GET_FILENAME_COMPONENT(PLOC_DIR ${PLOC} PATH)

		ADD_CUSTOM_COMMAND(TARGET imgmaniptest POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${CLOC} ${PLOC_DIR}
		)
	ENDIF(WIN32)
ENDIF(ENABLE_TEST)
