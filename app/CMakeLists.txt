INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
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
TARGET_LINK_LIBRARIES(paramedit editgraph ${QT_QTMAIN_LIBRARY} charon-plugins)

IF(WIN32)
	# copy needed dll for tests and application run before installing
	GET_TARGET_PROPERTY(CLOC charon-core LOCATION)
	GET_TARGET_PROPERTY(PLOC paramedit LOCATION)
	GET_FILENAME_COMPONENT(PLOC_DIR ${PLOC} PATH)

	ADD_CUSTOM_COMMAND(TARGET paramedit POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CLOC} ${PLOC_DIR}
	)
	INSTALL(FILES ${CLOC}
		DESTINATION bin
		COMPONENT dlls
	)
ENDIF(WIN32)

# install into bin subdirectory
INSTALL(
    TARGETS     paraminspector
    RUNTIME     DESTINATION    bin
    COMPONENT   applications
)

#Create application package
IF(APPLE)
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/MacOSInfo.plist.in
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MacOSInfo.plist
		@ONLY
	)
	INSTALL(
	    FILES       ${PROJECT_SOURCE_DIR}/icons/MacOSIcon.icns
	    DESTINATION /Applications/Tuchulcha.app/Contents/Resources
	    COMPONENT   applications
	)
	INSTALL(
	    FILES       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MacOSInfo.plist
	    DESTINATION /Applications/Tuchulcha.app/Contents
	    COMPONENT   applications
	    RENAME	Info.plist
	)
	INSTALL(
	    TARGETS     paramedit
	    RUNTIME     DESTINATION    /Applications/Tuchulcha.app/Contents/MacOS
	    COMPONENT   applications
	)
ELSE(APPLE)
	INSTALL(
	    TARGETS     paramedit
	    RUNTIME     DESTINATION    bin
	    COMPONENT   applications
	)

ENDIF(APPLE)
