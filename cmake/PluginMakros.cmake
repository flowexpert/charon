# The ADD_CHARON_PLUGIN Makro can be used in the following way:
# ADD_CHARON_PLUGIN(<pluginname>
#	<source files>
#	[CUDA] # add this when the plugin depends on CUDA, CUDA_ADD_LIBRARY instead of the regular ADD_LIBRARY will be used
#	[COMPANY <company name>] #only used in VisualStudio, this string will be compiled into the dll.
#	[LINK_LIBRARIES <libraries>] #additional libraries to link the plugin against. Same syntax as TARGET_LINK_LIBRARIES
#	[PLUGIN_LISTS] <lists>] #names of existing cmake variables. The created library target will be added to each list. 
#							#Usefull if you wan't to make additional settings to multiple plugins.
# )
INCLUDE(CMakeParseArguments)
MACRO(ADD_CHARON_PLUGIN)
	CMAKE_PARSE_ARGUMENTS(PLUGIN
	"CUDA"
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
		
		CONFIGURE_FILE(${charon-core_ROOT_DIR}/cmake/version.rc.in
			${PLUGIN_NAME}.version.rc
		@ONLY)
		LIST(APPEND PLUGIN_SOURCES "${PLUGIN_NAME}.version.rc")
	ENDIF(MSVC)
	IF(UNIX)
		CONFIGURE_FILE(${charon-core_ROOT_DIR}/cmake/version.cpp.in
			${PLUGIN_NAME}.version.cpp
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
  
	SET_TARGET_PROPERTIES(${PLUGIN_NAME} PROPERTIES
		INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins
	)

	INSTALL(
		TARGETS         ${PLUGIN_NAME}
		EXPORT          ${EXPORT_NAME}
		RUNTIME         DESTINATION bin
		LIBRARY         DESTINATION lib${LIB_SUFFIX}/charon-plugins
		ARCHIVE         DESTINATION lib${LIB_SUFFIX}/charon-plugins
		COMPONENT       libraries
	) 
ENDMACRO(ADD_CHARON_PLUGIN)
