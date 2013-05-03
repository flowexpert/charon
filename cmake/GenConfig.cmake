# create config files for package finding
SET(PROJECT_INSTALL_DOC ${${PROJECT_NAME}_INSTALL_DOC})
SET(USE_IMPORT ON)
INCLUDE(CMakePackageConfigHelpers)

IF(UNIX)
	SET(PROJECT_CONFIG_INSTALL share/cmake/${PROJECT_NAME})
ELSE()
	SET(PROJECT_CONFIG_INSTALL cmake)
ENDIF()
IF(NOT DEFINED CONFIG_INSTALL)
	SET(CONFIG_INSTALL ${PROJECT_CONFIG_INSTALL})
ENDIF()
CONFIGURE_PACKAGE_CONFIG_FILE(
	${CMAKE_MODULE_PATH}/ProjectConfig.cmake.in
	cmake/${PROJECT_NAME}-config.cmake
	INSTALL_DESTINATION ${PROJECT_CONFIG_INSTALL}
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
WRITE_BASIC_PACKAGE_VERSION_FILE(
	cmake/${PROJECT_NAME}-config-version.cmake
	VERSION ${${PROJECT_NAME}_VERSION}
	COMPATIBILITY AnyNewerVersion
)
# create config files for usage of build tree
CONFIGURE_FILE(
	${CMAKE_MODULE_PATH}/ProjectConfigLocal.cmake.in
	${PROJECT_NAME}-config.cmake
	@ONLY
)
WRITE_BASIC_PACKAGE_VERSION_FILE(
	${PROJECT_NAME}-config-version.cmake
	VERSION ${${PROJECT_NAME}_VERSION}
	COMPATIBILITY AnyNewerVersion
)
CONFIGURE_FILE(
	${CMAKE_MODULE_PATH}/PluginMakros.cmake
	cmake/${PROJECT_NAME}-use.cmake
	@ONLY
)
CONFIGURE_FILE(
	${CMAKE_MODULE_PATH}/version.rc.in
	cmake/version.rc.in
	COPYONLY
)

INSTALL(FILES
	${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config-version.cmake
	${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-use.cmake
	${CMAKE_CURRENT_BINARY_DIR}/cmake/version.rc.in
	DESTINATION ${PROJECT_CONFIG_INSTALL}
	COMPONENT libraries
)

# handle export installation
IF(NOT EXPORT_NAME)
	SET(EXPORT_NAME ${PROJECT_NAME}-export)
ENDIF()
IF(NOT SKIP_EXPORT_INSTALL)
	INSTALL(
		EXPORT          ${EXPORT_NAME}
		DESTINATION     ${PROJECT_CONFIG_INSTALL}
		COMPONENT       libraries
	)
ENDIF()

UNSET(PROJECT_INSTALL_DOC)
UNSET(IMPORT_PATH)
UNSET(USE_IMPORT)
