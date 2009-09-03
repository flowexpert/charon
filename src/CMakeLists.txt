INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${GRAPHVIZ_INCLUDE_DIRS})

# source files containing QObject derived classes
SET(EDITPARAM_SOURCES
    ParameterFileModel.cpp
    PrefixValidator.cpp
    ObjectInspector.cpp
    DocGenerator.cpp
    ModelToolBar.cpp
    messageHandler.cpp
    Qtiostream.cpp
    imageManip.cpp
    VarTypeMap.cpp
    MetaData.cpp
    FileManager.cpp
)

SET(EDITGRAPH_SOURCES
    GraphModel.cpp
    GraphBuilder.cpp
    HierarchyGraphBuilder.cpp
    TemplateGraphBuilder.cpp
    FlowWidget.cpp
    SelectorWidget.cpp
    GraphView.cpp
    HierarchyGraphView.cpp
    TemplateGraphView.cpp
)

# Configure Paths.config file
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/Paths.config.in
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Paths.config
	@ONLY
)
INSTALL(
    FILES	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Paths.config
    DESTINATION share/${PROJECT_NAME}
    COMPONENT   libraries
)

# =============  generate moc files ====================

# create moc files
QT4_AUTOMOC(${EDITPARAM_SOURCES})
QT4_AUTOMOC(${EDITGRAPH_SOURCES})

# create libraries
ADD_LIBRARY(editparam ${EDITPARAM_SOURCES})
ADD_LIBRARY(editgraph ${EDITGRAPH_SOURCES})

# add dependencies
TARGET_LINK_LIBRARIES(editparam charon-plugins charon-utils ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(editgraph editparam ${GRAPHVIZ_LIBRARIES})

# install library to lib directory
INSTALL(
    TARGETS      editparam
    EXPORT       ${PROJECT_NAME}
    RUNTIME      DESTINATION bin
    LIBRARY      DESTINATION lib${LIB_SUFFIX}
    ARCHIVE      DESTINATION lib${LIB_SUFFIX}
    COMPONENT    libraries
)
INSTALL(
    TARGETS      editgraph
    EXPORT       ${PROJECT_NAME}
    RUNTIME      DESTINATION bin
    LIBRARY      DESTINATION lib${LIB_SUFFIX}
    ARCHIVE      DESTINATION lib${LIB_SUFFIX}
    COMPONENT    libraries
)
INSTALL(
    EXPORT       ${PROJECT_NAME}
    DESTINATION  include/${PROJECT_NAME}
    COMPONENT    libraries
)

# install header files to include directory
FILE(GLOB HEADER_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.h *.hxx
)
INSTALL(
    FILES       ${HEADER_FILES}
    DESTINATION include/${PROJECT_NAME}
    COMPONENT   headers
)

