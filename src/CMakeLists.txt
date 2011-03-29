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
)
SET(TG_SRC
	QDirEdit.cpp
	ParamSlotModel.cpp
	SideWidget.cpp
	Wizard.cpp
	WizardPageStart.cpp
	WizardPageMetadata.cpp
	WizardPageSlots.cpp
	WizardPageParameters.cpp
	WizardPageSummary.cpp
	main.cpp
)
SET(UI_SRC
	SideWidget.ui
	WizardPageStart.ui
	WizardPageMetadata.ui
	WizardPageSlots.ui
	WizardPageParameters.ui
	WizardPageSummary.ui
)

QT4_WRAP_CPP(MOC_OUT ${MOC_SRC})
SOURCE_GROUP(MOCS FILES ${MOC_OUT})
QT4_WRAP_UI(UI_OUT ${UI_SRC})
QT4_ADD_RESOURCES(QRC_OUT resource.qrc)

ADD_EXECUTABLE(templategenerator
	${TG_SRC}
	${MOC_SRC}
	${QRC_OUT}
	${MOC_OUT}
	${UI_OUT}
)
TARGET_LINK_LIBRARIES(templategenerator ${QT_LIBRARIES})
INSTALL(TARGETS templategenerator DESTINATION bin)
