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
	ADD_CHARON_PLUGIN(dummymodule DummyModule.cpp
		SKIP_INSTALL PLUGIN_LISTS testTargets)
	ADD_LIBRARY(dummymodule2 SHARED DummyModule.cpp)
	SET_TARGET_PROPERTIES(dummymodule2 PROPERTIES COMPILE_DEFINITIONS "dummymodule_EXPORTS")
	TARGET_LINK_LIBRARIES(dummymodule2 charon-core)
	LIST(APPEND testTargets dummymodule2)
	ADD_LIBRARY(dummymodule3 SHARED DummyModule.cpp versionInvalid1.cpp)
	SET_TARGET_PROPERTIES(dummymodule3 PROPERTIES COMPILE_DEFINITIONS "dummymodule_EXPORTS")
	TARGET_LINK_LIBRARIES(dummymodule3 charon-core)
	LIST(APPEND testTargets dummymodule3)
	ADD_LIBRARY(dummymodule4 SHARED DummyModule.cpp versionInvalid2.cpp)
	SET_TARGET_PROPERTIES(dummymodule4 PROPERTIES COMPILE_DEFINITIONS "dummymodule_EXPORTS")
	TARGET_LINK_LIBRARIES(dummymodule4 charon-core)
	LIST(APPEND testTargets dummymodule4)
	ADD_CHARON_PLUGIN(dynamicdummymodule DynamicDummyModule.cpp
		SKIP_INSTALL PLUGIN_LISTS testTargets)
	ADD_CHARON_PLUGIN(caseddummy CasedDummy.cpp
		SKIP_INSTALL PLUGIN_LISTS testTargets)
	ADD_CHARON_PLUGIN(whilegrouptestreader WhileGroupTestReader.cpp
		SKIP_INSTALL PLUGIN_LISTS testTargets)
	ADD_CHARON_PLUGIN(whilegrouptestwriter WhileGroupTestWriter.cpp
		SKIP_INSTALL PLUGIN_LISTS testTargets)
	ADD_CHARON_PLUGIN(whilegroupteststatement WhileGroupTestStatement.cpp
		SKIP_INSTALL PLUGIN_LISTS testTargets)
	ADD_CHARON_PLUGIN(virtualslottestwriter VirtualSlotTestWriter.cpp
		SKIP_INSTALL PLUGIN_LISTS testTargets)
	ADD_CHARON_PLUGIN(virtualslottestreader VirtualSlotTestReader.cpp
		SKIP_INSTALL PLUGIN_LISTS testTargets)
	
	# locate module directories:
	# MODULE_DIR: test module directory
	# CHARON_PLUGINS: (global) directory of charon-core-modules
	GET_TARGET_PROPERTY(DUMMY_LOC dummymodule LOCATION)
	GET_FILENAME_COMPONENT(MODULE_DIR "${DUMMY_LOC}" PATH)
	ADD_DEFINITIONS(-DMODULE_DIR="${MODULE_DIR}")
	GET_TARGET_PROPERTY(GOBJ_LOC groupobject LOCATION)
	GET_FILENAME_COMPONENT(CHARON_PLUGINS "${GOBJ_LOC}" PATH)
	ADD_DEFINITIONS(-DCHARON_PLUGINS="${CHARON_PLUGINS}")

	# tests for ParameterReferences
	ADD_EXECUTABLE(toposorttest topoSortTest.cpp)
	TARGET_LINK_LIBRARIES(toposorttest charon-core charon-plugins)
	ADD_TEST(
		NAME ExecutionOrder_TopoSort
		COMMAND toposorttest
	)
	LIST(APPEND tests ExecutionOrder_TopoSort)
	LIST(APPEND testTargets toposorttest)

	IF(USE_LIBELF)
		# test for Plugin Version in ELF files
		ADD_EXECUTABLE(versionInfoElf versionInfoElf.cpp)
		TARGET_LINK_LIBRARIES(versionInfoElf charon-core charon-plugins)
		ADD_TEST(
			NAME Plugin_VersionELF
			COMMAND versionInfoElf
		)
		LIST(APPEND tests Plugin_VersionELF)
		LIST(APPEND testTargets versionInfoElf)
	ENDIF()

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

	TARGET_LINK_LIBRARIES(testparametergroupobject charon-core charon-plugins)
	ADD_TEST(
		NAME ParameterGroupObjectTest
		COMMAND testparametergroupobject
	)
	LIST(APPEND tests ParameterGroupObjectTest)
	LIST(APPEND testTargets testparametergroupobject)

	# add test labels for dashboard submission
	SET_TESTS_PROPERTIES(${tests} PROPERTIES LABELS charon-core)
	SET_TARGET_PROPERTIES(${testTargets} PROPERTIES FOLDER "charon-core Tests")
ENDIF(BUILD_TESTING)
