# create config files for package finding
SET(PROJECT_INSTALL_DOC ${${PROJECT_NAME}_INSTALL_DOC})
SET(PROJECT_VERSION ${${PROJECT_NAME}_VERSION})
IF(UNIX)
	SET(CONFIG_INSTALL share/cmake/${PROJECT_NAME})
ELSE()
	SET(CONFIG_INSTALL cmake)
ENDIF()
CONFIGURE_FILE(
	${CMAKE_MODULE_PATH}/ProjectConfig.cmake.in
	cmake/${PROJECT_NAME}-config.cmake
	@ONLY
)
CONFIGURE_FILE(
	${CMAKE_MODULE_PATH}/ProjectVersion.cmake.in
	cmake/${PROJECT_NAME}-config-version.cmake
	@ONLY
)
# create config files for usage of build tree
CONFIGURE_FILE(
	${CMAKE_MODULE_PATH}/ProjectConfigLocal.cmake.in
	${PROJECT_NAME}-config.cmake
	@ONLY
)
CONFIGURE_FILE(
	${CMAKE_MODULE_PATH}/ProjectVersion.cmake.in
	${PROJECT_NAME}-config-version.cmake
	@ONLY
)
INSTALL(FILES
	${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config-version.cmake
	DESTINATION ${CONFIG_INSTALL}
	COMPONENT libraries
)

UNSET(PROJECT_INSTALL_DOC)
UNSET(PROJECT_VERSION)
