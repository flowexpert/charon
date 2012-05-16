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
	NodeTreeView.ui
	ObjectInspector.ui
	PriorityDialog.ui
)

SET(TUCHULCHA_SOURCES
	Qtiostream.cpp
	Qtiostream.h
	FileManager.cpp
	FileManager.h
	RecentFileHandler.cpp
	RecentFileHandler.h
	ParameterFileModel.cpp
	ParameterFileModel.h
	PrefixValidator.cpp
	PrefixValidator.h
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
	TuchulchaWindow.cpp
	TuchulchaWindow.h
	DocGenerator.cpp
	DocGenerator.h
	LogDialog.cpp
	LogDialog.h
	OptionsDialog.cpp
	OptionsDialog.h
	NodeTreeView.cpp
	NodeTreeView.h
	WorkflowComments.cpp
	WorkflowComments.h
	CentralMdiArea.cpp
	CentralMdiArea.h
	ObjectInspector.cpp
	ObjectInspector.h
	ParamInspectorWindow.cpp
	ParamInspectorWindow.h
	PriorityDialog.cpp
	PriorityDialog.h
	PriorityBox.cpp
	PriorityBox.h
)

SET(WF_RUNNER_SOURCES
	CommunicationHandler.h
	CommunicationHandler.cpp
	CharonRun.h
	CharonRun.cpp
)

# =============  generate moc files ====================

# create moc files
QT4_AUTOMOC(${TUCHULCHA_SOURCES})
QT4_AUTOMOC(${WF_RUNNER_SOURCES})

# create libraries
ADD_LIBRARY(tuchulcha-widgets ${TUCHULCHA_SOURCES} ${UI_SRC})
ADD_LIBRARY(tc-run ${WF_RUNNER_SOURCES})

# add dependencies
TARGET_LINK_LIBRARIES(tuchulcha-widgets ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(tc-run tuchulcha-widgets charon-plugins ${QT_LIBRARIES})
SET_TARGET_PROPERTIES(
	tuchulcha-widgets tc-run
	PROPERTIES FOLDER "Tuchulcha")

# add install target for library
INSTALL(TARGETS tuchulcha-widgets
	EXPORT tuchulcha
	DESTINATION lib${LIB_SUFFIX}
	COMPONENT   libraries
)
INSTALL(DIRECTORY .
	DESTINATION include/tuchulcha
	COMPONENT   libraries
	FILES_MATCHING PATTERN "*.h"
)
INSTALL(EXPORT tuchulcha
	DESTINATION ${CONFIG_INSTALL}
	COMPONENT   libraries
)
