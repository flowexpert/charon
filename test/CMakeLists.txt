# Test suite of application
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

IF(BUILD_TESTING)
	GET_TARGET_PROPERTY(Diff2d_LOC diff2d LOCATION)
	GET_FILENAME_COMPONENT(CHARON_TEST_PLUGIN_DIR ${Diff2d_LOC} PATH)
	ADD_DEFINITIONS(-DCHARON_TEST_PLUGIN_DIR="${CHARON_TEST_PLUGIN_DIR}")
	MESSAGE("CHARON_TEST_PLUGIN_DIR="${CHARON_TEST_PLUGIN_DIR})
	SET(CHARON_PLUGINS ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins)
	ADD_DEFINITIONS(-DCHARON_PLUGINS="${CHARON_PLUGINS}")
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

	# tests for name casing
	ADD_EXECUTABLE(casetest namecasing.cpp)
	TARGET_LINK_LIBRARIES(casetest charon-core charon-plugins)
	ADD_TEST(
		NAME NameCasing
		COMMAND casetest
	)
	LIST(APPEND tests NameCasing)
	LIST(APPEND testTargets casetest)

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
	ADD_LIBRARY(caseddummy SHARED CasedDummy.cpp)
	TARGET_LINK_LIBRARIES(caseddummy charon-core)
	ADD_LIBRARY(whilegrouptestreader SHARED WhileGroupTestReader.cpp)
	TARGET_LINK_LIBRARIES(whilegrouptestreader charon-core)
	ADD_LIBRARY(whilegrouptestwriter SHARED WhileGroupTestWriter.cpp)
	TARGET_LINK_LIBRARIES(whilegrouptestwriter charon-core)
	ADD_LIBRARY(whilegroupteststatement SHARED WhileGroupTestStatement.cpp)
	TARGET_LINK_LIBRARIES(whilegrouptestwriter charon-core)
	ADD_LIBRARY(virtualslottestwriter SHARED VirtualSlotTestWriter.cpp)
	TARGET_LINK_LIBRARIES(virtualslottestwriter charon-core)
	ADD_LIBRARY(virtualslottestreader SHARED VirtualSlotTestReader.cpp)
	TARGET_LINK_LIBRARIES(virtualslottestreader charon-core)
	LIST(APPEND testTargets dummymodule dynamicdummymodule caseddummy whilegrouptestreader whilegrouptestwriter whilegroupteststatement
		virtualslottestwriter virtualslottestreader)
	
	# locate module directory
	GET_TARGET_PROPERTY(DUMMY_LOC dummymodule LOCATION)
	GET_FILENAME_COMPONENT(MODULE_DIR "${DUMMY_LOC}" PATH)
	ADD_DEFINITIONS(-DMODULE_DIR="${MODULE_DIR}")
	ADD_DEFINITIONS(-DLIB_SUFFIX="${LIB_SUFFIX}")
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



	# tests for virtual slot data passing
	ADD_EXECUTABLE(groupVirtualSlot groupVirtualSlot.cpp)
	TARGET_LINK_LIBRARIES(groupVirtualSlot charon-core charon-plugins)
	ADD_TEST(
		NAME groupVirtualSlot
		COMMAND groupVirtualSlot
	)
	LIST(APPEND tests groupVirtualSlot)
	LIST(APPEND testTargets groupVirtualSlot)





	ADD_EXECUTABLE(whilegrouptest testWhileGroup.cpp)
	TARGET_LINK_LIBRARIES(whilegrouptest charon-core charon-plugins whilegroup)
	ADD_TEST(
		NAME WhileGroupTest
		COMMAND whilegrouptest
	)
	LIST(APPEND tests WhileGroupTest)
	LIST(APPEND testTargets whilegrouptest)


	ADD_EXECUTABLE(testparametergroupobject testParameterGroupObject.cpp)

	TARGET_LINK_LIBRARIES(testparametergroupobject charon-core )
	ADD_TEST(
		NAME ParameterGroupObjectTest
		COMMAND testparametergroupobject
	)
	LIST(APPEND tests ParameterGroupObjectTest)
	LIST(APPEND testTargets testparametergroupobject)

	# add test labels for dashboard submission
	SET_TESTS_PROPERTIES(${tests} PROPERTIES LABELS charon-core)
	SET_TARGET_PROPERTIES(${testTargets} PROPERTIES FOLDER "charon-core Tests")
MESSAGE("Builddir" ${PROJECT_SOURCE_DIR})
ENDIF(BUILD_TESTING)
