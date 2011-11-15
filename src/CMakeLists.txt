INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# info definitions
IF(tuchulcha_VCS_INFO)
	ADD_DEFINITIONS(-DVCSINFO="${tuchulcha_VCS_INFO}")
ENDIF()
ADD_DEFINITIONS(-DTUCHULCHA_VERSION="${tuchulcha_VERSION}")
IF(tuchulcha_BUILD_INFO)
	ADD_DEFINITIONS(-DBUILD_INFO="${tuchulcha_BUILD_INFO}")
ENDIF()
ADD_DEFINITIONS(-DCHARON_CORE_VERSION="${charon-core_VERSION}")

# source files containing QObject derived classes
QT4_WRAP_UI(UI_SRC
	LogDialog.ui
	OptionsDialog.ui
)

SET(TUCHULCHA_COMMON_SOURCES
	Qtiostream.cpp
	Qtiostream.h
	imageManip.cpp
	imageManip.h
	FileManager.cpp
	FileManager.h
)

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
	QParameterFile.h
	QParameterFile.cpp
	VarTypeMap.cpp
	VarTypeMap.h
	MetaData.cpp
	MetaData.h
	QDirEdit.cpp
	QDirEdit.h
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
)

SET(INSPECTOR_SOURCES
	AdvancedInspector.cpp
	AdvancedInspector.h
	ParamInspectorWindow.cpp
	ParamInspectorWindow.h
)

SET(WF_RUNNER_SOURCES
	CommunicationHandler.h
	CommunicationHandler.cpp
	CharonRun.h
	CharonRun.cpp
)

SET(TUCHULCHA_SOURCES
	TuchulchaWindow.cpp
	TuchulchaWindow.h
	DocGenerator.cpp
	DocGenerator.h
	LogDialog.cpp
	LogDialog.h
	OptionsDialog.cpp
	OptionsDialog.h
)

# =============  generate moc files ====================

# create moc files
QT4_AUTOMOC(${TUCHULCHA_COMMON_SOURCES})
QT4_AUTOMOC(${EDITPARAM_SOURCES})
QT4_AUTOMOC(${EDITGRAPH_SOURCES})
QT4_AUTOMOC(${INSPECTOR_SOURCES})
QT4_AUTOMOC(${WF_RUNNER_SOURCES})
QT4_AUTOMOC(${TUCHULCHA_SOURCES})

# create libraries
ADD_LIBRARY(tc-common ${TUCHULCHA_COMMON_SOURCES})
ADD_LIBRARY(editparam ${EDITPARAM_SOURCES})
ADD_LIBRARY(editgraph ${EDITGRAPH_SOURCES})
ADD_LIBRARY(paraminspector-widgets ${INSPECTOR_SOURCES})
ADD_LIBRARY(tc-run ${WF_RUNNER_SOURCES})
ADD_LIBRARY(tuchulcha-widgets ${TUCHULCHA_SOURCES} ${UI_SRC})

# add dependencies
TARGET_LINK_LIBRARIES(editparam
	tc-common charon-plugins charon-core ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(editgraph editparam)
TARGET_LINK_LIBRARIES(paraminspector-widgets editparam)
TARGET_LINK_LIBRARIES(tc-run tc-common charon-plugins ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(tuchulcha-widgets editgraph)
SET_TARGET_PROPERTIES(
	tc-common editparam editgraph paraminspector-widgets
	tc-run tuchulcha-widgets
	PROPERTIES FOLDER "Tuchulcha")
