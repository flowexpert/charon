# Test suite of application
INCLUDE_DIRECTORIES(${tuchulcha_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Graphviz_INCLUDE_DIRS})

IF(BUILD_TESTING)
	SET(tests)
	SET(testTargets)

	# common definitions
	ADD_DEFINITIONS(-DDEFAULTCLASSFILE="${CMAKE_CURRENT_SOURCE_DIR}/sampleclasses.wrp")
	ADD_DEFINITIONS(-DTESTCLASSFILE="${CMAKE_CURRENT_SOURCE_DIR}/testClasses.wrp")

	# test usage of graphviz library
    ADD_EXECUTABLE(graphtest graphtest.cpp)
    TARGET_LINK_LIBRARIES(graphtest ${Graphviz_LIBRARIES})
    ADD_TEST(
		NAME Graphviz
		COMMAND graphtest
	)
	LIST(APPEND tests Graphviz)
	LIST(APPEND testTargets graphtest)

	# test ParameterFileModel
    ADD_EXECUTABLE(modeltest modeltest.cpp)
    TARGET_LINK_LIBRARIES(modeltest editparam)
    ADD_TEST(
		NAME ParameterFileModel
		COMMAND modeltest
	)
	LIST(APPEND tests ParameterFileModel)
	LIST(APPEND testTargets modeltest)

	# check for graphviz bug
	QT4_AUTOMOC(graphTemplateTest.cpp)
	ADD_EXECUTABLE(graphtemptest graphTemplateTest.cpp)
	TARGET_LINK_LIBRARIES(graphtemptest editgraph)
	ADD_TEST(
		NAME TemplateWidget
		COMMAND graphtemptest
	)
	LIST(APPEND tests TemplateWidget)
	LIST(APPEND testTargets graphtemptest)

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

	# add test labels for dashboard submission
	SET_TESTS_PROPERTIES(${tests} PROPERTIES LABELS tuchulcha)
	SET_TARGET_PROPERTIES(${testTargets} PROPERTIES FOLDER "Tuchulcha Tests")
ENDIF(BUILD_TESTING)
