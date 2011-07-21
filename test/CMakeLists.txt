# Test suite of application
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

IF(BUILD_TESTING)
	SET(tests)
	SET(testTargets)

	# tests for SplitStream
	ADD_EXECUTABLE(streamtest splitstreams.cpp)
	TARGET_LINK_LIBRARIES(streamtest charon-core)
	ADD_TEST(
		NAME SplitStream
		COMMAND streamtest
	)
	LIST(APPEND tests SplitStream)
	LIST(APPEND testTargets streamtest)

	# tests for ParameterFile
	ADD_EXECUTABLE(paramtest parameterfiletest.cpp)
	TARGET_LINK_LIBRARIES(paramtest charon-core)
	ADD_TEST(
		NAME ParameterFile
		COMMAND paramtest
	)
	LIST(APPEND tests ParameterFile)
	LIST(APPEND testTargets paramtest)

	# tests for ParameteredObject etc.
	ADD_EXECUTABLE(parametertest parametertest.cpp)
	TARGET_LINK_LIBRARIES(parametertest charon-core charon-plugins)
	ADD_TEST(
		NAME ParameterTest
		COMMAND parametertest
	)
	LIST(APPEND tests ParameterTest)
	LIST(APPEND testTargets parametertest)

	# add test labels for dashboard submission
	SET_TESTS_PROPERTIES(${tests} PROPERTIES LABELS charon-core)
	SET_TARGET_PROPERTIES(${testTargets} PROPERTIES FOLDER "charon-core Tests")
ENDIF(BUILD_TESTING)
