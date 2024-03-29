INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
SET(ICON_DIR ${PROJECT_SOURCE_DIR}/icons)

QT4_ADD_RESOURCES(QRC_RES ${PROJECT_SOURCE_DIR}/resources.qrc)
ADD_DEFINITIONS(-DTUCHULCHA_VERSION="${tuchulcha_VERSION}")

# create icon files (if possible)
IF(WIN32)
	EMBED_ICON(TC_ICO ${ICON_DIR}/tc.ico tcicon)
	EMBED_ICON(INSP_ICO ${ICON_DIR}/tc2.ico inspicon)
ENDIF(WIN32)

# translation location
IF(${PROJECT_NAME}_TRANSLATIONS_DIR)
	ADD_DEFINITIONS(
		-DTUCHULCHA_QM_DIR="${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_TRANSLATIONS_DIR}")
ELSE()
	# install to bin if not location specified
	SET(${PROJECT_NAME}_TRANSLATIONS_DIR bin)
ENDIF()

# standalone inspector application
ADD_EXECUTABLE(paraminspector WIN32 paraminspector ${QRC_RES} ${INSP_ICO})
TARGET_LINK_LIBRARIES(paraminspector
	tuchulcha-widgets ${QT_QTMAIN_LIBRARY})

ADD_EXECUTABLE(tuchulcha-run tuchulcha-run)
TARGET_LINK_LIBRARIES(tuchulcha-run tc-run)
SET_TARGET_PROPERTIES(tuchulcha-run PROPERTIES
	OUTPUT_NAME_DEBUG tuchulcha-run_d
)

ADD_EXECUTABLE(tuchulcha WIN32 tuchulcha ${QRC_RES} ${TC_ICO})
TARGET_LINK_LIBRARIES(tuchulcha tuchulcha-widgets ${QT_QTMAIN_LIBRARY})

SET_TARGET_PROPERTIES(paraminspector tuchulcha-run tuchulcha
	PROPERTIES FOLDER "Tuchulcha")

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
		PERMISSIONS WORLD_READ
	)
	INSTALL(
		FILES       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MacOSInfo.plist
		DESTINATION /Applications/Tuchulcha.app/Contents
		COMPONENT   applications
		RENAME      Info.plist
		PERMISSIONS WORLD_READ
	)
	INSTALL(
		TARGETS     tuchulcha tuchulcha-run paraminspector
		RUNTIME     DESTINATION    /Applications/Tuchulcha.app/Contents/MacOS
		COMPONENT   applications
		PERMISSIONS WORLD_READ
	)
ELSE(APPLE)
	INSTALL(
		TARGETS     tuchulcha tuchulcha-run paraminspector
		RUNTIME     DESTINATION    bin${BIN_SUFFIX}
		COMPONENT   applications
	)
	IF(UNIX)
		# application icons
		INSTALL(FILES
			${ICON_DIR}/tc48.png
			DESTINATION share/icons/hicolor/48x48/apps
			RENAME      tuchulcha.png
			COMPONENT   applications
		)
		INSTALL(FILES
			tuchulcha.desktop
			DESTINATION share/applications
			COMPONENT   applications
		)
		INSTALL(FILES
			${ICON_DIR}/tc2.png
			DESTINATION share/icons/hicolor/48x48/apps
			RENAME      paraminspector.png
			COMPONENT   applications
		)
		INSTALL(FILES
			paraminspector.desktop
			DESTINATION share/applications
			COMPONENT   applications
		)
	ENDIF()
ENDIF(APPLE)

# generate translations for all ts-files in the i18n dir,
# put them beside the tuchulcha executable in the build tree
# and install them to tuchulcha_TRANSLATIONS_DIR which defaults to
# the bin directory where tuchulcha will be installed (see above)
FILE(GLOB TS_FILES ${PROJECT_SOURCE_DIR}/i18n/*.ts)
GET_TARGET_PROPERTY(TLOC tuchulcha LOCATION)
GET_FILENAME_COMPONENT(TLOC "${TLOC}" PATH)
#MESSAGE("TLOC: ${TLOC}")
QT4_ADD_TRANSLATION(QM_FILES ${TS_FILES})
#MESSAGE("${QM_FILES}")
ADD_CUSTOM_TARGET(i18n DEPENDS ${QM_FILES})
INSTALL(FILES "${QM_FILES}"
	DESTINATION "${${PROJECT_NAME}_TRANSLATIONS_DIR}"
	OPTIONAL
)
FOREACH(X ${QM_FILES})
	# copy to build tree
	ADD_CUSTOM_COMMAND(TARGET i18n POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${X}" "${TLOC}/"
	)
ENDFOREACH(X)
UNSET(QM_FILE)
UNSET(TLOC)
