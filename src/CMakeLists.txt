INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(MOC_SRC
	QDirEdit.h
	Wizard.h
	WizardPageStart.h
	MainWindow.h
	MyTabWidget.h
)
SET(TG_SRC
	QDirEdit.cpp
	Wizard.cpp
	WizardPageStart.cpp
	MyTabWidget.cpp
	main.cpp
	MainWindow.cpp
)

QT4_WRAP_CPP(MOC_OUT ${MOC_SRC})
SOURCE_GROUP(MOCS FILES ${MOC_OUT})
QT4_WRAP_UI(UI_OUT Wizard.ui)
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
