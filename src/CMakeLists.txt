INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(MOC_SRC
	QDirEdit.h
	ParamSlotModel.h
	SideWidget.h
	Wizard.h
	WizardPageStart.h
	WizardPageMetadata.h
	WizardPageSlots.h
	WizardPageParameters.h
	WizardPageSummary.h
	Delegates.h
)
SET(CPP_SRC
	QDirEdit.cpp
	ParamSlotModel.cpp
	SideWidget.cpp
	Wizard.cpp
	WizardPageStart.cpp
	WizardPageMetadata.cpp
	WizardPageSlots.cpp
	WizardPageParameters.cpp
	WizardPageSummary.cpp
	Delegates.cpp
	main.cpp
)
SET(UI_SRC
	WizardPageStart.ui
	WizardPageMetadata.ui
	WizardPageSlots.ui
	WizardPageParameters.ui
	WizardPageSummary.ui
)
SET(QRC_SRC
	resource.qrc
)

QT4_WRAP_CPP(MOC_OUT ${MOC_SRC})
QT4_WRAP_UI(UI_OUT ${UI_SRC})
QT4_ADD_RESOURCES(QRC_OUT ${QRC_SRC})

ADD_EXECUTABLE(templategenerator WIN32
	${CPP_SRC}
	${MOC_SRC}
	${QRC_OUT}
	${MOC_OUT}
	${UI_OUT}
)
TARGET_LINK_LIBRARIES(templategenerator ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
SET_TARGET_PROPERTIES(templategenerator PROPERTIES FOLDER charon-templates)

SOURCE_GROUP("Moc Files" FILES ${MOC_OUT})
SOURCE_GROUP("Ui Files" FILES ${UI_SRC} ${UI_OUT})
SOURCE_GROUP("qrc Files" FILES ${QRC_SRC} ${QRC_OUT})

INSTALL(TARGETS templategenerator DESTINATION bin)
