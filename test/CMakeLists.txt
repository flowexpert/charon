# Test suite of application
INCLUDE_DIRECTORIES(${tuchulcha_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Graphviz_INCLUDE_DIRS})

IF(ENABLE_TEST)
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

	# test ParameterFileModel
    ADD_EXECUTABLE(modeltest modeltest.cpp)
    TARGET_LINK_LIBRARIES(modeltest editparam)
    ADD_TEST(
		NAME ParameterFileModel
		COMMAND modeltest
	)

	# check for graphviz bug
	QT4_AUTOMOC(graphTemplateTest.cpp)
	ADD_EXECUTABLE(graphtemptest graphTemplateTest.cpp)
	TARGET_LINK_LIBRARIES(graphtemptest editgraph)
	ADD_TEST(
		NAME TemplateWidget
		COMMAND graphtemptest
	)
ENDIF(ENABLE_TEST)
