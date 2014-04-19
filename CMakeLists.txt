PROJECT(charon-meta)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# set common export name
SET(EXPORT_NAME ${PROJECT_NAME}-export)
SET(SKIP_EXPORT_INSTALL TRUE)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(BUILD_ROOT_PREFIX "${charon-meta_BINARY_DIR}/")
SET(SKIP_LOCAL_DLL_COPY ON)
IF(UNIX)
	SET(CONFIG_INSTALL share/cmake/${PROJECT_NAME})
ELSE()
	SET(CONFIG_INSTALL cmake)
ENDIF()

# set directories for FIND_PACKAGE commands to find
# already built subprojects
SET(charon-core_DIR "${CMAKE_CURRENT_BINARY_DIR}/charon-core"
	CACHE PATH "Directory containing charon-core config files"
	FORCE)
SET(charon-utils_DIR "${CMAKE_CURRENT_BINARY_DIR}/charon-utils"
	CACHE PATH "Directory containing charon-utils config files"
	FORCE)
SET(charon_DIR "${CMAKE_CURRENT_BINARY_DIR}/charon"
	CACHE PATH "Directory containing charon config files"
	FORCE)

MESSAGE(STATUS "Configuring selected charon subprojects")

# tests
INCLUDE(CTest)

OPTION(BUILD_CHARON_CORE "enable build of charon-core" ON)
IF(BUILD_CHARON_CORE)
	MESSAGE(STATUS)
	MESSAGE(STATUS "Configuring charon-core:")
	ADD_SUBDIRECTORY(charon-core)
ENDIF()

OPTION(BUILD_CHARON_UTILS "enable build of charon-utils" ON)
IF(BUILD_CHARON_UTILS)
	IF(NOT BUILD_CHARON_CORE)
		MESSAGE(FATAL_ERROR "charon-core is required by charon-utils!")
	ENDIF()
	MESSAGE(STATUS)
	MESSAGE(STATUS "Configuring charon-utils:")
	ADD_SUBDIRECTORY(charon-utils)
	IF(TARGET charon-utils_doc_html)
		ADD_DEPENDENCIES(charon-utils_doc_html charon-core_doc_html)
	ENDIF()
	IF(TARGET charon-utils_doc_pdf)
		ADD_DEPENDENCIES(charon-utils_doc_pdf charon-core_doc_html)
	ENDIF()
ENDIF()

OPTION(BUILD_CHARON "enable build of charon" ON)
IF(BUILD_CHARON)
	IF(NOT BUILD_CHARON_UTILS)
		MESSAGE(FATAL_ERROR "charon-utils is required by charon!")
	ENDIF()
	MESSAGE(STATUS)
	MESSAGE(STATUS "Configuring charon:")
	ADD_SUBDIRECTORY(charon)
	IF(TARGET charon_doc_html)
		ADD_DEPENDENCIES(charon_doc_html charon-utils_doc_html)
	ENDIF()
	IF(TARGET charon_doc_pdf)
		ADD_DEPENDENCIES(charon_doc_pdf charon-utils_doc_html)
	ENDIF()
ENDIF()

OPTION(BUILD_TUCHULCHA "enable build of tuchulcha" ON)
IF(BUILD_TUCHULCHA)
	IF(NOT BUILD_CHARON_CORE)
		MESSAGE(FATAL_ERROR "charon-core is required by tuchulcha!")
	ENDIF()
	MESSAGE(STATUS)
	MESSAGE(STATUS "Configuring tuchulcha:")
	ADD_SUBDIRECTORY(tuchulcha)
	IF(TARGET tuchulcha_doc_html)
		ADD_DEPENDENCIES(tuchulcha_doc_html charon-core_doc_html)
	ENDIF()
	IF(TARGET tuchulcha_doc_pdf)
		ADD_DEPENDENCIES(tuchulcha_doc_pdf charon-core_doc_html)
	ENDIF()
ENDIF()

OPTION(BUILD_TEMPLATEGENERATOR "enable build of Template Generator" ON)
IF(BUILD_TEMPLATEGENERATOR)
	MESSAGE(STATUS)
	MESSAGE(STATUS "Configuring templategenerator:")
	ADD_SUBDIRECTORY(templategenerator)
ENDIF()

OPTION(BUILD_HEKATE "enable build of Hekate" OFF)
IF(BUILD_HEKATE)
	MESSAGE(STATUS)
	MESSAGE(STATUS "Configuring hekate:")
	ADD_SUBDIRECTORY(hekate)
ENDIF()

# install export
INSTALL(
	EXPORT       ${EXPORT_NAME}
	DESTINATION  ${CONFIG_INSTALL}
	COMPONENT    libraries
)
