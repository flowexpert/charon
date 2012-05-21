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
	ADD_DEFINITIONS(
		-DTEST_WRP_FILE="${CMAKE_CURRENT_SOURCE_DIR}/parameterfiletest.wrp")
	ADD_EXECUTABLE(parameterfiletest parameterfiletest.cpp)
	TARGET_LINK_LIBRARIES(parameterfiletest charon-core charon-plugins)
	ADD_TEST(
		NAME ParameterFile
		COMMAND parameterfiletest
	)
	LIST(APPEND tests ParameterFile)
	LIST(APPEND testTargets parameterfiletest)

	# tests for ParameteredObject etc.
	ADD_EXECUTABLE(parametertest parametertest.cpp)
	TARGET_LINK_LIBRARIES(parametertest charon-core charon-plugins)
	ADD_TEST(
		NAME ParameterTest
		COMMAND parametertest
	)
	LIST(APPEND tests ParameterTest)
	LIST(APPEND testTargets parametertest)

	# tests for input/output slot data passing
	ADD_EXECUTABLE(datapasstest dataPassing.cpp)
	TARGET_LINK_LIBRARIES(datapasstest charon-core charon-plugins)
	ADD_TEST(
		NAME SlotDataPassing
		COMMAND datapasstest
	)
	LIST(APPEND tests SlotDataPassing)
	LIST(APPEND testTargets datapasstest)

	# tests for ParameterReferences
	ADD_EXECUTABLE(paramreftest parameterRefTest.cpp)
	TARGET_LINK_LIBRARIES(paramreftest charon-core)
	ADD_TEST(
		NAME ParameterRef
		COMMAND paramreftest
	)
	LIST(APPEND tests ParameterRef)
	LIST(APPEND testTargets paramreftest)

	# dummy test modules for PluginManager tests
	ADD_LIBRARY(dummymodule SHARED DummyModule.cpp)
	TARGET_LINK_LIBRARIES(dummymodule charon-core)
	ADD_LIBRARY(dynamicdummymodule SHARED DynamicDummyModule.cpp)
	TARGET_LINK_LIBRARIES(dynamicdummymodule charon-core)
	LIST(APPEND testTargets dummymodule dynamicdummymodule)
	
	# locate module directory
	GET_TARGET_PROPERTY(DUMMY_LOC dummymodule LOCATION)
	GET_FILENAME_COMPONENT(MODULE_DIR "${DUMMY_LOC}" PATH)
	ADD_DEFINITIONS(-DMODULE_DIR="${MODULE_DIR}")

	# tests for ParameterReferences
	ADD_EXECUTABLE(toposorttest topoSortTest.cpp)
	TARGET_LINK_LIBRARIES(toposorttest charon-core charon-plugins)
	ADD_TEST(
		NAME ExecutionOrder_TopoSort
		COMMAND toposorttest
	)
	LIST(APPEND tests ExecutionOrder_TopoSort)
	LIST(APPEND testTargets toposorttest)

	# tests for DynamicModules
	ADD_EXECUTABLE(dynamictest dynamictest.cpp)
	TARGET_LINK_LIBRARIES(dynamictest charon-core charon-plugins)
	ADD_TEST(
		NAME DynamicModuleTest
		COMMAND dynamictest
	)
	LIST(APPEND tests DynamicModuleTest)
	LIST(APPEND testTargets dynamictest)

	# add test labels for dashboard submission
	SET_TESTS_PROPERTIES(${tests} PROPERTIES LABELS charon-core)
	SET_TARGET_PROPERTIES(${testTargets} PROPERTIES FOLDER "charon-core Tests")
ENDIF(BUILD_TESTING)
