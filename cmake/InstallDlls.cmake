IF(INSTALL_DLLS)
	# Qt dlls
	FIND_FILE(QT_CORE_DLL
		QtCore4.dll
		PATHS $ENV{PATH}
	)
	FIND_FILE(QT_GUI_DLL
		QtGui4.dll
		PATHS $ENV{PATH}
	)
	FIND_FILE(QT_SVG_DLL
		QtSvg4.dll
		PATHS $ENV{PATH}
	)
	FIND_FILE(QT_WEBKIT_DLL
		QtWebKit4.dll
		PATHS $ENV{PATH}
	)
	FIND_FILE(QT_NETWORK_DLL
		QtNetwork4.dll
		PATHS $ENV{PATH}
	)
	INSTALL(FILES
		${QT_CORE_DLL}
		${QT_GUI_DLL}
		${QT_SVG_DLL}
		${QT_WEBKIT_DLL}
		${QT_NETWORK_DLL}
		DESTINATION    bin
		COMPONENT      dlls
	)
	MARK_AS_ADVANCED(
		QT_CORE_DLL
		QT_GUI_DLL
		QT_SVG_DLL
		QT_WEBKIT_DLL
		QT_NETWORK_DLL
	)

	# Debug Qt libraries
	FIND_FILE(QT_CORE_DLL_DEBUG
		QtCored4.dll
		PATHS $ENV{PATH}
	)
	FIND_FILE(QT_GUI_DLL_DEBUG
		QtGuid4.dll
		PATHS $ENV{PATH}
	)
	FIND_FILE(QT_SVG_DLL_DEBUG
		QtSvgd4.dll
		PATHS $ENV{PATH}
	)
	FIND_FILE(QT_WEBKIT_DLL_DEBUG
		QtWebKitd4.dll
		PATHS $ENV{PATH}
	)
	FIND_FILE(QT_NETWORK_DLL_DEBUG
		QtNetworkd4.dll
		PATHS $ENV{PATH}
	)
	MARK_AS_ADVANCED(
		QT_CORE_DLL_DEBUG
		QT_GUI_DLL_DEBUG
		QT_SVG_DLL_DEBUG
		QT_WEBKIT_DLL_DEBUG
		QT_NETWORK_DLL_DEBUG
	)

	INSTALL(FILES
		${QT_CORE_DLL_DEBUG}
		${QT_GUI_DLL_DEBUG}
		${QT_SVG_DLL_DEBUG}
		${QT_WEBKIT_DLL_DEBUG}
		${QT_NETWORK_DLL_DEBUG}
		DESTINATION    bin
		CONFIGURATIONS Debug
		COMPONENT      dlls
		OPTIONAL
	)
	
	IF(MSVC)
		# MSVC specific libraries (if needed)
		SET(CMAKE_INSTALL_MFC_LIBRARIES OFF)
		SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)
		
		# Release libraries
		SET(CMAKE_INSTALL_DEBUG_LIBRARIES OFF)
		INCLUDE(InstallRequiredSystemLibraries)
		INSTALL(FILES
			${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
			DESTINATION    bin
			CONFIGURATIONS Release
			COMPONENT      dlls
		)
		
		# Debug libraries
		SET(CMAKE_INSTALL_DEBUG_LIBRARIES ON)
		INCLUDE(InstallRequiredSystemLibraries)
		INSTALL(FILES
			${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
			DESTINATION    bin
			CONFIGURATIONS Debug
			COMPONENT      dlls	
		)
	ENDIF(MSVC)
	IF(MINGW)
		FIND_FILE(QT_MINGWM10
			mingwm10.dll
			PATHS $ENV{PATH}
		)
		MARK_AS_ADVANCED(
			QT_MINGWM10
		)
		INSTALL(FILES
			${QT_MINGWM10}
			DESTINATION    bin
			COMPONENT      dlls	
		)
	ENDIF(MINGW)
ENDIF(INSTALL_DLLS)
