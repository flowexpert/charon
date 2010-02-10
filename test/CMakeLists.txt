# Test suite of application
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

IF(ENABLE_TEST)
	# tests for SplitStream
	ADD_EXECUTABLE(streamtest splitstreams.cpp)
	TARGET_LINK_LIBRARIES(streamtest charon-core)
	ADD_TEST(StreamTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/streamtest)

	# tests for ParameterFile
	ADD_EXECUTABLE(paramtest parameterfiletest.cpp)
	TARGET_LINK_LIBRARIES(paramtest charon-core)
	ADD_TEST(ParamTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/paramtest)

	# tests for ParameteredObject etc.
	ADD_EXECUTABLE(parametertest parametertest.cpp)
	TARGET_LINK_LIBRARIES(parametertest charon-core)
	ADD_TEST(ParameterTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/parametertest)

	# tests for compile and load routines.
	IF(WIN32)
		ADD_DEFINITIONS(-DPLUGIN_DIR=".")
	ELSE(WIN32)
		ADD_DEFINITIONS(-DPLUGIN_DIR="${PROJECT_BINARY_DIR}/src")
	ENDIF(WIN32)
	ADD_DEFINITIONS(-DTESTPLUGIN_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
	ADD_EXECUTABLE(compileandloadtest compileandloadtest.cpp)
	ADD_CUSTOM_COMMAND(TARGET compileandloadtest POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Paths.config
			${CMAKE_CURRENT_BINARY_DIR}/Paths.config)
	TARGET_LINK_LIBRARIES(compileandloadtest charon-plugins)
	ADD_TEST(CompileAndLoadTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/compileandloadtest)
ENDIF(ENABLE_TEST)
