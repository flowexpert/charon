# Test suite of application
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

IF(ENABLE_TEST)
	# tests for SplitStream
	ADD_EXECUTABLE(streamtest splitstreams.cpp)
	TARGET_LINK_LIBRARIES(streamtest charon-core)
	GET_TARGET_PROPERTY(streamtest_LOC streamtest LOCATION)
	ADD_TEST(StreamTest ${streamtest_LOC})

	# tests for ParameterFile
	ADD_EXECUTABLE(paramtest parameterfiletest.cpp)
	TARGET_LINK_LIBRARIES(paramtest charon-core)
	GET_TARGET_PROPERTY(paramtest_LOC paramtest LOCATION)
	ADD_TEST(ParamTest ${paramtest_LOC})

	# tests for ParameteredObject etc.
	ADD_EXECUTABLE(parametertest parametertest.cpp)
	TARGET_LINK_LIBRARIES(parametertest charon-core)
	GET_TARGET_PROPERTY(parametertest_LOC parametertest LOCATION)
	ADD_TEST(ParameterTest ${parametertest_LOC})

	# tests for compile and load routines.
	IF(WIN32)
		ADD_DEFINITIONS(-DPLUGIN_DIR=".")
	ELSE(WIN32)
		ADD_DEFINITIONS(-DPLUGIN_DIR="${PROJECT_BINARY_DIR}/src")
	ENDIF(WIN32)
	ADD_DEFINITIONS(-DTESTPLUGIN_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
	ADD_EXECUTABLE(compileandloadtest compileandloadtest.cpp)
	TARGET_LINK_LIBRARIES(compileandloadtest charon-plugins)
	GET_TARGET_PROPERTY(compileandloadtest_LOC compileandloadtest LOCATION)
	ADD_TEST(CompileAndLoadTest ${compileandloadtest_LOC})
ENDIF(ENABLE_TEST)
