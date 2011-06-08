INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_DEFINITIONS(-DCHARON_INSTALL="${charon-core_ROOT_DIR}")

# source files containing QObject derived classes
QT4_WRAP_UI(UI_SRC LogDialog.ui OptionsDialog.ui)
SET(EDITPARAM_SOURCES
	ParameterFileModel.cpp
	ParameterFileModel.h
	PrefixValidator.cpp
	PrefixValidator.h
	ObjectInspector.cpp
	ObjectInspector.h
	InspectorDelegate.cpp
	InspectorDelegate.h
	ModelToolBar.cpp
	ModelToolBar.h
	Qtiostream.cpp
	Qtiostream.h
	QParameterFile.h
	QParameterFile.cpp
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
	DocGenerator.cpp
	DocGenerator.h
)

SET(EDITGRAPH_SOURCES
	GraphModel.cpp
	GraphModel.h
	FlowWidget.cpp
	FlowWidget.h
	ConnectionLine.cpp
	ConnectionLine.h
	Node.cpp
	Node.h
	NodeProperty.cpp
	NodeProperty.h
	NodeHandler.cpp
	NodeHandler.h
	NodeTreeView.cpp
	NodeTreeView.h
	WorkflowExecutor.h
	WorkflowExecutor.cpp
)

# =============  generate moc files ====================

# create moc files
QT4_AUTOMOC(${EDITPARAM_SOURCES})
QT4_AUTOMOC(${EDITGRAPH_SOURCES})

# create libraries
ADD_LIBRARY(editparam ${EDITPARAM_SOURCES} ${UI_SRC})
ADD_LIBRARY(editgraph ${EDITGRAPH_SOURCES})

# add dependencies
TARGET_LINK_LIBRARIES(editparam charon-plugins charon-core ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(editgraph editparam)
SET_TARGET_PROPERTIES(editparam editgraph PROPERTIES FOLDER "Tuchulcha")
