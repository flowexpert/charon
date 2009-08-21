INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# create resource files
QT4_ADD_RESOURCES(QRC_RES ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc)

# create icon files (if possible)
IF(WIN32)
	EMBED_ICON(PE_ICO ${PROJECT_SOURCE_DIR}/icons/tc.ico paramediticon)
	EMBED_ICON(INSP_ICO ${PROJECT_SOURCE_DIR}/icons/tc2.ico inspicon)
ENDIF(WIN32)

# standalone inspector application
SET(INSPECTOR_SOURCES
	AdvancedInspector.cpp
	ParamInspectorWindow.cpp
	paramInspectorApp.cpp
)

QT4_AUTOMOC(${INSPECTOR_SOURCES})
ADD_EXECUTABLE(paraminspector WIN32 ${INSPECTOR_SOURCES} ${INSP_ICO})
TARGET_LINK_LIBRARIES(paraminspector editparam ${QT_QTMAIN_LIBRARY})

# main application
SET(PARAMEDIT_SOURCES
	MainWindow.cpp
	main.cpp
)

QT4_AUTOMOC(${PARAMEDIT_SOURCES})
ADD_EXECUTABLE(paramedit WIN32 ${PARAMEDIT_SOURCES} ${QRC_RES} ${PE_ICO})
TARGET_LINK_LIBRARIES(paramedit editgraph ${QT_QTMAIN_LIBRARY})

# install into bin subdirectory
INSTALL(
    TARGETS     paramedit paraminspector
    RUNTIME     DESTINATION    bin
    COMPONENT   applications
)
