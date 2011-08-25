INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

QT4_ADD_RESOURCES(QRC_RES ${PROJECT_SOURCE_DIR}/resources.qrc)

# create icon files (if possible)
IF(WIN32)
	EMBED_ICON(TC_ICO ${PROJECT_SOURCE_DIR}/icons/tc.ico tcicon)
	EMBED_ICON(INSP_ICO ${PROJECT_SOURCE_DIR}/icons/tc2.ico inspicon)
ENDIF(WIN32)

# standalone inspector application
SET(INSPECTOR_SOURCES
	AdvancedInspector.cpp
	AdvancedInspector.h
	ParamInspectorWindow.cpp
	ParamInspectorWindow.h
	paramInspectorApp.cpp
)

QT4_AUTOMOC(${INSPECTOR_SOURCES})
ADD_EXECUTABLE(paraminspector WIN32 ${INSPECTOR_SOURCES} ${QRC_RES} ${INSP_ICO})
TARGET_LINK_LIBRARIES(paraminspector editparam ${QT_QTMAIN_LIBRARY})
SET_TARGET_PROPERTIES(paraminspector PROPERTIES
	FOLDER "Tuchulcha"
)

# main application
SET(TUCHULCHA_SOURCES
	MainWindow.cpp
	MainWindow.h
	main.cpp
)

QT4_AUTOMOC(${TUCHULCHA_SOURCES})
ADD_EXECUTABLE(tuchulcha WIN32 ${TUCHULCHA_SOURCES} ${QRC_RES} ${TC_ICO})
IF(tuchulcha_VCS_INFO)
	ADD_DEFINITIONS(-DVCSINFO="${tuchulcha_VCS_INFO}")
ENDIF()
ADD_DEFINITIONS(-DTUCHULCHA_VERSION="${tuchulcha_VERSION}")
IF(tuchulcha_BUILD_INFO)
	ADD_DEFINITIONS(-DBUILD_INFO="${tuchulcha_BUILD_INFO}")
ENDIF()
ADD_DEFINITIONS(-DCHARON_CORE_VERSION="${charon-core_VERSION}")

TARGET_LINK_LIBRARIES(tuchulcha editgraph ${QT_QTMAIN_LIBRARY} charon-plugins)
SET_TARGET_PROPERTIES(tuchulcha PROPERTIES
	FOLDER "Tuchulcha"
	OUTPUT_NAME_DEBUG tuchulcha_d
)

IF(WIN32)
	# copy needed dll for tests and application run before installing
	GET_TARGET_PROPERTY(CLOC charon-core LOCATION)
	GET_TARGET_PROPERTY(CLOCD charon-core LOCATION_DEBUG)
	IF(INSTALL_DLLS AND NOT DEFINED charon-meta_SOURCE_DIR)
		INSTALL(FILES ${CLOC}
			DESTINATION bin
			COMPONENT dlls
			CONFIGURATION Release
		)
		INSTALL(FILES ${CLOCD}
			DESTINATION bin
			COMPONENT dlls
			CONFIGURATION Debug
		)
	ENDIF()
ENDIF(WIN32)

# install into bin subdirectory
INSTALL(
	TARGETS     paraminspector
	RUNTIME     DESTINATION    bin${BIN_SUFFIX}
	COMPONENT   applications
)

IF(APPLE)
	# Create apple application package
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/MacOSInfo.plist.in
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MacOSInfo.plist
		@ONLY
	)
	INSTALL(
		FILES       ${PROJECT_SOURCE_DIR}/icons/MacOSIcon.icns
		DESTINATION /Applications/Tuchulcha.app/Contents/Resources
		COMPONENT   applications
		PERMISSIONS WORLD_WRITE
	)
	INSTALL(
		FILES       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MacOSInfo.plist
		DESTINATION /Applications/Tuchulcha.app/Contents
		COMPONENT   applications
		RENAME      Info.plist
		PERMISSIONS WORLD_WRITE
	)
	INSTALL(
		TARGETS     tuchulcha
		RUNTIME     DESTINATION    /Applications/Tuchulcha.app/Contents/MacOS
		COMPONENT   applications
		PERMISSIONS WORLD_WRITE
	)
ELSE(APPLE)
	INSTALL(
		TARGETS     tuchulcha
		RUNTIME     DESTINATION    bin${BIN_SUFFIX}
		COMPONENT   applications
	)
	IF(UNIX)
		# application icons
		INSTALL(FILES
			${PROJECT_SOURCE_DIR}/icons/tc.png
			DESTINATION share/icons/hicolor/48x48/apps
			RENAME tuchulcha.png
			COMPONENT   applications
		)
		INSTALL(FILES
			tuchulcha.desktop
			DESTINATION share/applications
			COMPONENT   applications
		)
		INSTALL(FILES
			${PROJECT_SOURCE_DIR}/icons/tc2.png
			DESTINATION share/icons/hicolor/48x48/apps
			RENAME paraminspector.png
			COMPONENT   applications
		)
		INSTALL(FILES
			paraminspector.desktop
			DESTINATION share/applications
			COMPONENT   applications
		)
	ENDIF()
ENDIF(APPLE)
