INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}) # for including ui headers
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}) # for ui header includes

# info definitions
IF(tuchulcha_VCS_INFO)
	ADD_DEFINITIONS(-DVCSINFO="${tuchulcha_VCS_INFO}")
ENDIF()
ADD_DEFINITIONS(-DTUCHULCHA_VERSION="${tuchulcha_VERSION}")
IF(tuchulcha_BUILD_INFO)
	ADD_DEFINITIONS(-DBUILD_INFO="${tuchulcha_BUILD_INFO}")
ENDIF()

IF(USE_ASSISTANT)
	IF(NOT QT_QTHELP_FOUND)
		MESSAGE(FATAL_ERROR "USE_ASSISTANT needs QtHelp, but QtHelp not found.")
	ENDIF()
	ADD_DEFINITIONS(-DUSE_ASSISTANT)
	IF(${PROJECT_NAME}_QHC_DIR)
		ADD_DEFINITIONS(
			-DTUCHULCHA_QHC_DIR="${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_QHC_DIR}")
	ENDIF()
ENDIF()

# source files containing QObject derived classes
QT4_WRAP_UI(UI_SRC
	LogDialog.ui
	OptionsDialog.ui
	NodeTreeView.ui
	ObjectInspector.ui
	PriorityDialog.ui
)

QT4_WRAP_CPP(MOC_SRC
	LogViewProxyModel.h
	FileManager.h
	PrefixValidator.h
	InspectorDelegate.h
	RecentFileHandler.h
	ModelToolBar.h
	ParameterFileModel.h
	GraphModel.h
	FlowWidget.h
	NodeHandler.h
	OptionsDialog.h
	DocGenerator.h
	LogDialog.h
	LogDecorators.h
	NodeTreeView.h
	TuchulchaWindow.h
	CentralMdiArea.h
	WorkflowComments.h
	PriorityDialog.h
	PriorityBox.h
	ParamInspectorWindow.h
	ObjectInspector.h
	QCopyListView.h
	QDirEdit.h
	QFilterWidget.h
)

SET(TUCHULCHA_SOURCES
	VarTypeMap.h
	VarTypeMap.cpp
	MetaData.h
	MetaData.cpp
	Node.h
	Node.cpp
	NodeProperty.h
	NodeProperty.cpp
	FileManager.cpp
	RecentFileHandler.cpp
	ParameterFileModel.cpp
	PrefixValidator.cpp
	InspectorDelegate.cpp
	ModelToolBar.cpp
	GraphModel.cpp
	FlowWidget.cpp
	ConnectionLine.h
	ConnectionLine.cpp
	NodeHandler.cpp
	TuchulchaWindow.cpp
	DocGenerator.cpp
	LogDialog.cpp
	LogDecorators.cpp
	LogViewProxyModel.cpp
	OptionsDialog.cpp
	NodeTreeView.cpp
	WorkflowComments.cpp
	CentralMdiArea.cpp
	ObjectInspector.cpp
	ParamInspectorWindow.cpp
	PriorityDialog.cpp
	PriorityBox.cpp
	QDirEdit.cpp
	QCopyListView.cpp
	QFilterWidget.cpp
	Qtiostream.h
	Qtiostream.cpp
	QParameterFile.h
	QParameterFile.cpp
)

QT4_WRAP_CPP(WF_RUNNER_MOC
	CharonRun.h
	CommunicationHandler.h
)
SET(WF_RUNNER_SOURCES
	CommunicationHandler.cpp
	CharonRun.cpp
)

# create libraries
ADD_LIBRARY(tuchulcha-widgets ${TUCHULCHA_SOURCES} ${UI_SRC} ${MOC_SRC})
ADD_LIBRARY(tc-run ${WF_RUNNER_SOURCES} ${WF_RUNNER_MOC})

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
