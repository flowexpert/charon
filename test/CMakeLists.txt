# Test suite of application
INCLUDE_DIRECTORIES(${tuchulcha_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})

IF(BUILD_TESTING)
	IF(NOT QT_QTTEST_FOUND)
		MESSAGE(FATAL_ERROR "BUILD_TESTING needs QtTest, but QtTest not found.")
	ENDIF()
	SET(tests)
	SET(testTargets)

	# common definitions
	ADD_DEFINITIONS(-DTEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

	# test ParameterFileModel
	QT4_WRAP_CPP(MTest_MOC QParameterFileModelTest.h)
	ADD_EXECUTABLE(modeltest QParameterFileModelTest.cpp ${MTest_MOC})
	TARGET_LINK_LIBRARIES(modeltest tuchulcha-widgets)
	ADD_TEST(
		NAME ParameterFileModel
		COMMAND modeltest
	)
	LIST(APPEND tests ParameterFileModel)
	LIST(APPEND testTargets modeltest)

	IF(WIN32)
		GET_TARGET_PROPERTY(test_loc modeltest LOCATION)
		GET_FILENAME_COMPONENT(LOCAL_PLUGIN_DIR ${test_loc} PATH)
		FOREACH(X ${ADDITIONAL_DLL_DEPENDS})
			ADD_CUSTOM_COMMAND(TARGET POST_BUILD modeltest
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${X}" "${LOCAL_PLUGIN_DIR}"
			)
		ENDFOREACH(X)
		FOREACH(X ${tuchulcha_QT_DLLS})
			ADD_CUSTOM_COMMAND(TARGET POST_BUILD modeltest
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${X}" "${LOCAL_PLUGIN_DIR}"
			)
		ENDFOREACH(X)
	ENDIF(WIN32)

	# test QParameterFile
	QT4_WRAP_CPP(QPFileTest_MOC QParameterFileTest.h)
	ADD_EXECUTABLE(qparameterfiletest QParameterFileTest.cpp ${QPFileTest_MOC})
	TARGET_LINK_LIBRARIES(qparameterfiletest tuchulcha-widgets charon-core)
	ADD_TEST(
		NAME QParameterFile
		COMMAND qparameterfiletest
	)
	LIST(APPEND tests QParameterFile)
	LIST(APPEND testTargets qparameterfiletest)

	# test mapping of QVariant types
	QT4_WRAP_CPP(QVarMapTest_MOC QVariantMappingTest.h)
	ADD_EXECUTABLE(qvariantmappingtest QVariantMappingTest.cpp ${QVarMapTest_MOC})
	TARGET_LINK_LIBRARIES(qvariantmappingtest ${QT_LIBRARIES})
	ADD_TEST(NAME QVariant_Mapping COMMAND qvariantmappingtest)
	LIST(APPEND tests QVariant_Mapping)
	LIST(APPEND testTargets qvariantmappingtest)

	# test for log view widget
	QT4_WRAP_CPP(LogViewTest_MOC LogViewTest.h)
	ADD_EXECUTABLE(logviewtest LogViewTest.cpp ${LogViewTest_MOC})
	TARGET_LINK_LIBRARIES(logviewtest tuchulcha-widgets ${QT_LIBRARIES})
	ADD_TEST(NAME LogViewWidget COMMAND logviewtest)
	LIST(APPEND tests LogViewWidget)
	LIST(APPEND testTargets logviewtest)

	# link tests against QtTest
	FOREACH(X ${testTargets})
		TARGET_LINK_LIBRARIES(${X} ${QT_QTTEST_LIBRARY})
	ENDFOREACH(X)

	IF(WIN32)
		GET_TARGET_PROPERTY(test_loc qparameterfiletest LOCATION)
		GET_FILENAME_COMPONENT(LOCAL_PLUGIN_DIR ${test_loc} PATH)
		FOREACH(X ${ADDITIONAL_DLL_DEPENDS})
			ADD_CUSTOM_COMMAND(TARGET POST_BUILD qparameterfiletest
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${X}" "${LOCAL_PLUGIN_DIR}"
			)
		ENDFOREACH(X)
		FOREACH(X ${tuchulcha_QT_DLLS})
			ADD_CUSTOM_COMMAND(TARGET POST_BUILD qparameterfiletest
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${X}" "${LOCAL_PLUGIN_DIR}"
			)
		ENDFOREACH(X)
	ENDIF(WIN32)

	# add test labels for dashboard submission
	SET_TESTS_PROPERTIES(${tests} PROPERTIES LABELS tuchulcha)
	SET_TARGET_PROPERTIES(${testTargets} PROPERTIES FOLDER "Tuchulcha Tests")
ENDIF(BUILD_TESTING)
