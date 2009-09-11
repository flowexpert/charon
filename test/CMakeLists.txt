# Test suite of application
INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

IF(ENABLE_TEST)
	# tests for imgmanip classes
	SET(PENGUINFILE "${CMAKE_CURRENT_SOURCE_DIR}/Penguin.pgm")
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cropsample.wrp.in ${CMAKE_CURRENT_BINARY_DIR}/cropsample.wrp @ONLY)
	ADD_DEFINITIONS(-DTESTWRPFILE="${CMAKE_CURRENT_BINARY_DIR}/cropsample.wrp")
	ADD_EXECUTABLE(imgmaniptest imgmaniptest.cpp)
	TARGET_LINK_LIBRARIES(imgmaniptest charon-plugins)
	IF(UNIX)
		TARGET_LINK_LIBRARIES(imgmaniptest pthread ${X11_LIBRARIES})
	ENDIF(UNIX)

	# tests for ImgTool routines
	ADD_DEFINITIONS(-DPENGUINFILE="${CMAKE_CURRENT_SOURCE_DIR}/Penguin.pgm")
	ADD_EXECUTABLE(imgtooltest imgToolTest.cpp)
	TARGET_LINK_LIBRARIES(imgtooltest charon-core)
	IF(UNIX)
		TARGET_LINK_LIBRARIES(imgtooltest pthread ${X11_LIBRARIES})
	ENDIF(UNIX)
ENDIF(ENABLE_TEST)
