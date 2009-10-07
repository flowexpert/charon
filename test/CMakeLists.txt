# Test suite of application
INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${tuchulcha_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${GRAPHVIZ_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(ENABLE_TEST)
	# common definitions
	ADD_DEFINITIONS(-DDEFAULTCLASSFILE="${CMAKE_CURRENT_SOURCE_DIR}/sampleclasses.wrp")
	ADD_DEFINITIONS(-DTESTCLASSFILE="${CMAKE_CURRENT_SOURCE_DIR}/testClasses.wrp")

	# test usage of graphviz library
    ADD_EXECUTABLE(graphtest graphtest.cpp)
    TARGET_LINK_LIBRARIES(graphtest ${GRAPHVIZ_LIBRARIES})
    ADD_TEST(GraphTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/graphtest)

	# test ParameterFileModel
    ADD_EXECUTABLE(modeltest modeltest.cpp)
    TARGET_LINK_LIBRARIES(modeltest editparam)
    ADD_TEST(ModelTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modeltest)

	# check for graphviz bug
	QT4_AUTOMOC(graphTemplateTest.cpp)
	ADD_EXECUTABLE(graphtemptest graphTemplateTest.cpp)
	TARGET_LINK_LIBRARIES(graphtemptest editgraph)
	ADD_TEST(GraphTempTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/graphtemptest)
ENDIF(ENABLE_TEST)
