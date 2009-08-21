# Test suite of application
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${tuchulcha_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${GRAPHVIZ_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(ENABLE_TEST)
    ADD_EXECUTABLE(graphtest graphtest.cpp)
    TARGET_LINK_LIBRARIES(graphtest ${GRAPHVIZ_LIBRARIES})
    ADD_TEST(GraphTest graphtest)

	ADD_DEFINITIONS(-DDEFAULTCLASSFILE="${CMAKE_CURRENT_SOURCE_DIR}/sampleclasses.wrp")
    ADD_EXECUTABLE(modeltest modeltest.cpp)
    TARGET_LINK_LIBRARIES(modeltest editparam)
    ADD_TEST(ModelTest modeltest)

	ADD_DEFINITIONS(-DTESTCLASSFILE="${CMAKE_CURRENT_SOURCE_DIR}/testClasses.wrp")
	QT4_AUTOMOC(graphTemplateTest.cpp)
	ADD_EXECUTABLE(graphtemptest graphTemplateTest.cpp)
	TARGET_LINK_LIBRARIES(graphtemptest editgraph)
	ADD_TEST(GraphTempTest graphtemptest)
ENDIF(ENABLE_TEST)
