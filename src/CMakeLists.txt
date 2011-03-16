INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${Graphviz_INCLUDE_DIRS})

ADD_DEFINITIONS(-DCHARON_INSTALL="${charon-core_ROOT_DIR}")

# source files containing QObject derived classes
QT4_WRAP_UI(UI_SRC LogDialog.ui OptionsDialog.ui)
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
    QDirEdit.cpp
    QDirEdit.h
)

SET(EDITGRAPH_SOURCES
    GraphModel.cpp
    GraphModel.h
    GraphBuilder.cpp
    GraphBuilder.h
    FlowWidget.cpp
    FlowWidget.h
    ConnectionSocket.cpp
    ConnectionSocket.h
    ConnectionLine.cpp
    ConnectionLine.h
    Node.cpp
    Node.h
    NodeProperty.cpp
    NodeProperty.h
    TypeHandler.cpp
    TypeHandler.h
    ParameterType.cpp
    ParameterType.h
    NodeHandler.cpp
    NodeHandler.h
    TreeViewItem.cpp
    TreeViewItem.h
    NodeTreeView.cpp
    NodeTreeView.h
    NodeView.cpp
    NodeView.h
    WorkflowExecutor.h
    WorkflowExecutor.cpp
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
TARGET_LINK_LIBRARIES(editgraph editparam ${Graphviz_LIBRARIES})
SET_TARGET_PROPERTIES(editparam editgraph PROPERTIES FOLDER "Tuchulcha")
