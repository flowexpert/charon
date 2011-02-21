# Test suite of application
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

IF(ENABLE_TEST)
	SET(tests)

	# tests for SplitStream
	ADD_EXECUTABLE(streamtest splitstreams.cpp)
	TARGET_LINK_LIBRARIES(streamtest charon-core)
	ADD_TEST(
		NAME SplitStream
		COMMAND streamtest
	)
	LIST(APPEND tests SplitStream)

	# tests for ParameterFile
	ADD_EXECUTABLE(paramtest parameterfiletest.cpp)
	TARGET_LINK_LIBRARIES(paramtest charon-core)
	ADD_TEST(
		NAME ParameterFile
		COMMAND paramtest
	)
	LIST(APPEND tests ParameterFile)

	# tests for ParameteredObject etc.
	ADD_EXECUTABLE(parametertest parametertest.cpp)
	TARGET_LINK_LIBRARIES(parametertest charon-core charon-plugins)
	ADD_TEST(
		NAME ParameterTest
		COMMAND parametertest
	)
	LIST(APPEND tests ParameterTest)

	# tests for compile and load routines.
	IF(WIN32)
		ADD_DEFINITIONS(-DLIBDIR="${PROJECT_BINARY_DIR}/src")
		ADD_DEFINITIONS(-DPLUGIN_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
	ELSE(WIN32)
		ADD_DEFINITIONS(-DPLUGIN_DIR="${PROJECT_BINARY_DIR}/src")
	ENDIF(WIN32)
	ADD_DEFINITIONS(-DTESTPLUGIN_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
	ADD_EXECUTABLE(compileandloadtest compileandloadtest.cpp)
	SET(CHARON_CORE_ROOT_DIR "${PROJECT_SOURCE_DIR}")
	CONFIGURE_FILE(
		${PROJECT_SOURCE_DIR}/src/Paths.config.in
		${CMAKE_CURRENT_BINARY_DIR}/Paths.config
		@ONLY
	)
	TARGET_LINK_LIBRARIES(compileandloadtest charon-plugins)
	ADD_TEST(
		NAME CompileAndLoad
		COMMAND compileandloadtest
	)
	LIST(APPEND tests CompileAndLoad)

	# add test labels for dashboard submission
	SET_TESTS_PROPERTIES(${tests}
		PROPERTIES LABELS charon-core
	)
ENDIF(ENABLE_TEST)
