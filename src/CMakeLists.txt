#files for Charon library
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
SET(CHARON_CORE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})
SET(CHARON_FILES
	${CHARON_CORE_INCLUDE_DIR}/AbstractData.hxx
	${CHARON_CORE_INCLUDE_DIR}/DllEx.h
	${CHARON_CORE_INCLUDE_DIR}/FileTool.h
	${CHARON_CORE_INCLUDE_DIR}/Parameter.h
	${CHARON_CORE_INCLUDE_DIR}/Parameter.hxx
	${CHARON_CORE_INCLUDE_DIR}/ParameteredObject.h
	${CHARON_CORE_INCLUDE_DIR}/ParameteredObject.hxx
	${CHARON_CORE_INCLUDE_DIR}/ParameterFile.h
	${CHARON_CORE_INCLUDE_DIR}/ParameterFile.hxx
	${CHARON_CORE_INCLUDE_DIR}/Slots.h
	${CHARON_CORE_INCLUDE_DIR}/Slots.hxx
	${CHARON_CORE_INCLUDE_DIR}/DataManagerParameterFile.hxx
	${CHARON_CORE_INCLUDE_DIR}/SplitStream.h
	${CHARON_CORE_INCLUDE_DIR}/StringTool.h
	${CHARON_CORE_INCLUDE_DIR}/TypeDetector.h
	${CHARON_CORE_INCLUDE_DIR}/ParameteredGroupObject.h
	ParameteredGroupObject.cpp

	FileTool.cpp
	Parameter.cpp
	ParameteredObject.cpp
	ParameterFile.cpp
	SplitStream.cpp
	StringTool.cpp
	TypeDetector.cpp
	Slots.cpp
)

SET(PLUGIN_FILES
	${CHARON_CORE_INCLUDE_DIR}/AbstractPluginLoader.h
	${CHARON_CORE_INCLUDE_DIR}/ExceptionHandler.h
	${CHARON_CORE_INCLUDE_DIR}/PluginManager.h
	${CHARON_CORE_INCLUDE_DIR}/PluginManagerInterface.h
	AbstractPluginLoader.cpp
	ExceptionHandler.cpp
	PluginManager.cpp
)

IF(WIN32)
	LIST(APPEND PLUGIN_FILES
		${CHARON_CORE_INCLUDE_DIR}/WindowsPluginLoader.h
		WindowsPluginLoader.cpp
	)
ENDIF(WIN32)
IF(UNIX)
	LIST(APPEND PLUGIN_FILES
		${CHARON_CORE_INCLUDE_DIR}/UnixPluginLoader.h
		UnixPluginLoader.cpp
	)
ENDIF(UNIX)

LIST(APPEND CHARON_FILES ${PLUGIN_FILES})

# create libraries
ADD_LIBRARY(charon-core SHARED ${CHARON_FILES})

ADD_LIBRARY(charon-plugins STATIC ${PLUGIN_FILES})
TARGET_LINK_LIBRARIES(charon-plugins charon-core)
IF(UNIX)
	TARGET_LINK_LIBRARIES(charon-plugins dl)
	TARGET_LINK_LIBRARIES(charon-core dl)
ENDIF(UNIX)

# create plugins

# determine plugin rpath
SET(LIB_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}")
SET(PLUGIN_RPATH "${LIB_INSTALL_PATH}/charon-plugins")
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${LIB_INSTALL_PATH}" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
	SET(PLUGIN_RPATH "${PLUGIN_RPATH}:${LIB_INSTALL_PATH}")
ENDIF("${isSystemDir}" STREQUAL "-1")
UNSET(LIB_INSTALL_PATH)
UNSET(isSystemDir)

ADD_LIBRARY(ifgroup SHARED
	IfGroup.cpp
	${CHARON_CORE_INCLUDE_DIR}/IfGroup.h
)
TARGET_LINK_LIBRARIES(ifgroup
	charon-core
)
SET_TARGET_PROPERTIES(ifgroup PROPERTIES
	INSTALL_RPATH "${PLUGIN_RPATH}"
	FOLDER "charon-core-modules"
)

ADD_LIBRARY(whilegroup SHARED
	WhileGroup.cpp
	${CHARON_CORE_INCLUDE_DIR}/WhileGroup.h
	${CHARON_CORE_INCLUDE_DIR}/StatementIntf.h
)
TARGET_LINK_LIBRARIES(whilegroup
	charon-core charon-plugins
)
SET_TARGET_PROPERTIES(whilegroup PROPERTIES
	INSTALL_RPATH "${PLUGIN_RPATH}"
	FOLDER "charon-core-modules"
)

ADD_LIBRARY(statement SHARED
	Statement.cpp
	${CHARON_CORE_INCLUDE_DIR}/Statement.h
	${CHARON_CORE_INCLUDE_DIR}/StatementIntf.h
)
TARGET_LINK_LIBRARIES(statement
	charon-core
)
SET_TARGET_PROPERTIES(statement PROPERTIES
	INSTALL_RPATH "${PLUGIN_RPATH}"
	FOLDER "charon-core-modules"
)

ADD_LIBRARY(forstatement SHARED
	ForStatement.cpp
	${CHARON_CORE_INCLUDE_DIR}/ForStatement.h
	${CHARON_CORE_INCLUDE_DIR}/StatementIntf.h
)
TARGET_LINK_LIBRARIES(forstatement
	charon-core
)
SET_TARGET_PROPERTIES(forstatement PROPERTIES
	INSTALL_RPATH "${PLUGIN_RPATH}"
	FOLDER "charon-core-modules"
)

ADD_LIBRARY(groupobject SHARED
	GroupObject.cpp
	${CHARON_CORE_INCLUDE_DIR}/GroupObject.h
)
TARGET_LINK_LIBRARIES(groupobject
	charon-core
)
SET_TARGET_PROPERTIES(groupobject PROPERTIES
	INSTALL_RPATH "${PLUGIN_RPATH}"
	FOLDER "charon-core-modules"
)

ADD_LIBRARY(inputslotbundle SHARED
	InputSlotBundle.cpp
	SlotBundle.cpp
	${CHARON_CORE_INCLUDE_DIR}/InputSlotBundle.h
	${CHARON_CORE_INCLUDE_DIR}/SlotBundle.h
	${CHARON_CORE_INCLUDE_DIR}/SlotBundleInterfaces.h
)
SET_TARGET_PROPERTIES(inputslotbundle PROPERTIES
	COMPILE_DEFINITIONS slotbundle_EXPORTS
	INSTALL_RPATH "${PLUGIN_RPATH}"
	FOLDER "charon-core-modules"
)
TARGET_LINK_LIBRARIES(inputslotbundle charon-core charon-plugins)

ADD_LIBRARY(outputslotbundle SHARED
	OutputSlotBundle.cpp
	SlotBundle.cpp
	${CHARON_CORE_INCLUDE_DIR}/OutputSlotBundle.h
	${CHARON_CORE_INCLUDE_DIR}/SlotBundle.h
	${CHARON_CORE_INCLUDE_DIR}/SlotBundleInterfaces.h
)
SET_TARGET_PROPERTIES(outputslotbundle PROPERTIES
	COMPILE_DEFINITIONS slotbundle_EXPORTS
	INSTALL_RPATH "${PLUGIN_RPATH}"
	FOLDER "charon-core-modules"
)
TARGET_LINK_LIBRARIES(outputslotbundle charon-core charon-plugins)

# install library
SET_TARGET_PROPERTIES(charon-core charon-plugins PROPERTIES
	VERSION ${${PROJECT_NAME}_VERSION}
	FOLDER "charon-core"
)
INSTALL(
	TARGETS      charon-core charon-plugins
	EXPORT       ${EXPORT_NAME}
	RUNTIME      DESTINATION bin${BIN_SUFFIX}
	LIBRARY      DESTINATION lib${LIB_SUFFIX}
	ARCHIVE      DESTINATION lib${LIB_SUFFIX}
	COMPONENT    libraries
)

# install charon-core modules
INSTALL(
	TARGETS          inputslotbundle outputslotbundle
	                 ifgroup whilegroup groupobject
	                 statement forstatement
	EXPORT           ${EXPORT_NAME}
	RUNTIME          DESTINATION bin${BIN_SUFFIX}
	LIBRARY          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	ARCHIVE          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	COMPONENT        libraries
)

IF(NOT SKIP_EXPORT_INSTALL)
	INSTALL(
		EXPORT       ${EXPORT_NAME}
		DESTINATION  ${CONFIG_INSTALL}
		COMPONENT    libraries
	)
ENDIF()

# install header files to include directory
INSTALL(
	DIRECTORY   ${CHARON_CORE_INCLUDE_DIR}
	DESTINATION include
	COMPONENT   headers
)
