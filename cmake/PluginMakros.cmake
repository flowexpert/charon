# The ADD_CHARON_PLUGIN Makro can be used in the following way:
# ADD_CHARON_PLUGIN(<pluginname>
#	<source files>
#	[CUDA]
#	[SKIP_INSTALL]
#	[COMPANY <company name>]
#	[LINK_LIBRARIES <libraries>]
#	[PLUGIN_LISTS <lists>]
# )
#
# The CUDA argument may be used on plugins depending on CUDA.
# This will replace the regular ADD_LIBRARY call by CUDA_ADD_LIBRARY.
#
# The SKIP_INSTALL argument excludes the generated plugin from install targets.
# This is e.g. useful on plugins only used in the test suite.
#
# The COMPANY argument is only used in VisualStudio.
# The given string will be compiled into the dll.
#
# Using the LINK_LIBRARIES argument, additional libraries for plugin linking
# may be specified. This has the same effect and syntax as calling
# TARGET_LINK_LIBRARIES on the plugin.
#
# The PLUGIN_LISTS argument causes the plugin name to be appended to
# the given CMake list variables.
# This is useful if you wan't to apply additional settings to multiple plugins.

INCLUDE(CMakeParseArguments)
MACRO(ADD_CHARON_PLUGIN)
	CMAKE_PARSE_ARGUMENTS(PLUGIN
		"CUDA;SKIP_INSTALL"
		"COMPANY"
		"LINK_LIBRARIES;PLUGIN_LISTS"
		${ARGN}
	)

	# get plugin name and sources from the first (unparsed) arguments
	SET(PLUGIN_SOURCES ${PLUGIN_UNPARSED_ARGUMENTS})
	LIST(GET PLUGIN_SOURCES 0 PLUGIN_NAME)
	LIST(REMOVE_AT PLUGIN_SOURCES 0)

	#MESSAGE(STATUS "PLUGIN_NAME : ${PLUGIN_NAME}")
	#MESSAGE(STATUS "PLUGIN_SOURCES : ${PLUGIN_SOURCES}")

	IF(MSVC)
		SET(DllRcCompanyName ${PLUGIN_COMPANY})
		SET(DllRcFileDescription "${PLUGIN_NAME} Charon Plugin")
		SET(DllOriginalFilenameDebug "${PLUGIN_NAME}${CMAKE_DEBUG_POSTFIX}.dll")
		SET(DllOriginalFilenameRelease "${PLUGIN_NAME}.dll")
		SET(DllRcInternalName "${PLUGIN_NAME}")

		CONFIGURE_FILE(
			"${charon-core_VERSION_RC_FILE}"
			"${PLUGIN_NAME}.version.rc"
			@ONLY)
		LIST(APPEND PLUGIN_SOURCES "${PLUGIN_NAME}.version.rc")
	ENDIF(MSVC)
	IF(UNIX)
		CONFIGURE_FILE(
			"${charon-core_VERSION_CPP_FILE}"
			"${PLUGIN_NAME}.version.cpp"
			@ONLY)
		LIST(APPEND PLUGIN_SOURCES "${PLUGIN_NAME}.version.cpp")
	ENDIF(UNIX)
	IF(PLUGIN_CUDA)
		CUDA_ADD_LIBRARY(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES})
	ELSE(PLUGIN_CUDA)
		ADD_LIBRARY(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES})
	ENDIF(PLUGIN_CUDA)
	TARGET_LINK_LIBRARIES(${PLUGIN_NAME} charon-core ${PLUGIN_LINK_LIBRARIES} )
	FOREACH(X ${PLUGIN_PLUGIN_LISTS})
		LIST(APPEND ${X} ${PLUGIN_NAME})
	ENDFOREACH(X)

	IF(NOT PLUGIN_SKIP_INSTALL)
		SET_TARGET_PROPERTIES(${PLUGIN_NAME} PROPERTIES INSTALL_RPATH
			"${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins"
		)

		INSTALL(
			TARGETS         ${PLUGIN_NAME}
			EXPORT          ${EXPORT_NAME}
			RUNTIME         DESTINATION bin
			LIBRARY         DESTINATION lib${LIB_SUFFIX}/charon-plugins
			ARCHIVE         DESTINATION lib${LIB_SUFFIX}/charon-plugins
			COMPONENT       libraries
		)
	ENDIF(NOT PLUGIN_SKIP_INSTALL)
ENDMACRO(ADD_CHARON_PLUGIN)
