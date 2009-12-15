INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${GRAPHVIZ_INCLUDE_DIRS})

# source files containing QObject derived classes
QT4_WRAP_UI(UI_SRC LogDialog.ui)
SET(EDITPARAM_SOURCES
	${UI_SRC}
    ParameterFileModel.cpp
    ParameterFileModel.h
    PrefixValidator.cpp
    PrefixValidator.h
    ObjectInspector.cpp
    ObjectInspector.h
	InspectorDelegate.cpp
	InspectorDelegate.h
    DocGenerator.cpp
    DocGenerator.h
    ModelToolBar.cpp
    ModelToolBar.h
    Qtiostream.cpp
    Qtiostream.h
    imageManip.cpp
    imageManip.h
    VarTypeMap.cpp
    VarTypeMap.h
    MetaData.cpp
    MetaData.h
    FileManager.cpp
    FileManager.h
)

SET(EDITGRAPH_SOURCES
    GraphModel.cpp
    GraphModel.h
    GraphBuilder.cpp
    GraphBuilder.h
    HierarchyGraphBuilder.cpp
    HierarchyGraphBuilder.h
    TemplateGraphBuilder.cpp
    TemplateGraphBuilder.h
    FlowWidget.cpp
    FlowWidget.h
    SelectorWidget.cpp
    SelectorWidget.h
    GraphView.cpp
    GraphView.h
    HierarchyGraphView.cpp
    HierarchyGraphView.h
    TemplateGraphView.cpp
    TemplateGraphView.h
)

# Configure Paths.config file
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/Paths.config.in
	${CMAKE_CURRENT_BINARY_DIR}/Paths.config
	@ONLY
)

# =============  generate moc files ====================

# create moc files
QT4_AUTOMOC(${EDITPARAM_SOURCES})
QT4_AUTOMOC(${EDITGRAPH_SOURCES})

# create libraries
ADD_LIBRARY(editparam ${EDITPARAM_SOURCES})
ADD_LIBRARY(editgraph ${EDITGRAPH_SOURCES})

# add dependencies
TARGET_LINK_LIBRARIES(editparam charon-plugins charon-core ${QT_LIBRARIES})
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

