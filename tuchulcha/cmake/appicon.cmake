# macro for application icon embedding

# usage:
# 	EMBED_ICON(outvar iconfile iconname)
#		outvar:		variable you have to add to the application sources
#		iconfile:	path to the application icon file (*.ico)
#		iconname:	some unique (in the current dir) icon file name
#					to name the resource files that are created
#	Example:
# 		EMBED_ICON(ICON1 somefile.ico icon1)
#		ADD_EXECUTABLE(bla main.cpp ${ICON1})

IF(WIN32)
	# use rc compiler system on MSVC
	IF(MSVC)
		ENABLE_LANGUAGE(RC)
	ENDIF(MSVC)

	# macro for application icon embedding
	MACRO(EMBED_ICON outvar iconfile iconname)
		SET(ICON_FILE ${iconfile})
		CONFIGURE_FILE(
			${PROJECT_SOURCE_DIR}/cmake/appicon.rc.in
			${CMAKE_CURRENT_BINARY_DIR}/${iconname}.rc
			@ONLY
		)
		IF(MINGW)
			ADD_CUSTOM_COMMAND(
				OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${iconname}_rc.o
				COMMAND windres.exe
					-I ${CMAKE_CURRENT_SOURCE_DIR}
					-i ${CMAKE_CURRENT_BINARY_DIR}/${iconname}.rc
					-o ${CMAKE_CURRENT_BINARY_DIR}/${iconname}_rc.o
				DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${iconname}.rc
				DEPENDS ${iconfile}
			)
			SET(${outvar} ${CMAKE_CURRENT_BINARY_DIR}/${iconname}_rc.o)
		ELSE(MINGW)
			SET(${outvar} ${CMAKE_CURRENT_BINARY_DIR}/${iconname}.rc)
		ENDIF(MINGW)
	ENDMACRO(EMBED_ICON)
ELSE(WIN32)
	MESSAGE(SEND_ERROR "WARNING: appicon not implemented for non-windows systems")
ENDIF(WIN32)
