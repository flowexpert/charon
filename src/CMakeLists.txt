#files for Charon library
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
SET(CHARON_CORE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})
SET(CHARON_FILES
	${CHARON_CORE_INCLUDE_DIR}/configVersion.h
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
	${CHARON_CORE_INCLUDE_DIR}/PluginManagerInterface.h
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
	${CHARON_CORE_INCLUDE_DIR}/UnixPluginLoader.h
	${CHARON_CORE_INCLUDE_DIR}/WindowsPluginLoader.h
	AbstractPluginLoader.cpp
	ExceptionHandler.cpp
	PluginManager.cpp
)

SET(GROUP_FILES
	${CHARON_CORE_INCLUDE_DIR}/ParameteredGroupObject.h
	${CHARON_CORE_INCLUDE_DIR}/SlotBundle.h
	${CHARON_CORE_INCLUDE_DIR}/SlotBundleInterfaces.h
	ParameteredGroupObject.cpp
	SlotBundle.cpp
)

IF(WIN32)
	LIST(APPEND PLUGIN_FILES
		WindowsPluginLoader.cpp
	)
ENDIF(WIN32)
IF(UNIX)
	LIST(APPEND PLUGIN_FILES
		UnixPluginLoader.cpp
	)
ENDIF(UNIX)

# create libraries
ADD_LIBRARY(charon-core SHARED ${CHARON_FILES})
ADD_LIBRARY(charon-plugins SHARED ${PLUGIN_FILES})
ADD_LIBRARY(charon-groups SHARED ${GROUP_FILES})
TARGET_LINK_LIBRARIES(charon-plugins charon-core)
TARGET_LINK_LIBRARIES(charon-groups charon-plugins)

IF(UNIX)
	TARGET_LINK_LIBRARIES(charon-plugins dl)
	IF(USE_LIBELF)
		SET_TARGET_PROPERTIES(charon-plugins PROPERTIES
			COMPILE_DEFINITIONS USE_LIBELF)
		TARGET_LINK_LIBRARIES(charon-plugins elf)
	ENDIF(USE_LIBELF)
ENDIF(UNIX)
IF(MSVC)
	TARGET_LINK_LIBRARIES(charon-plugins version)
ENDIF(MSVC)

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

SET(charon-core-modules)

ADD_CHARON_PLUGIN(ifgroup
	IfGroup.cpp
	${CHARON_CORE_INCLUDE_DIR}/IfGroup.h
	LINK_LIBRARIES charon-groups
	PLUGIN_LISTS charon-core-modules
)

ADD_CHARON_PLUGIN(whilegroup
	WhileGroup.cpp
	${CHARON_CORE_INCLUDE_DIR}/WhileGroup.h
	${CHARON_CORE_INCLUDE_DIR}/StatementIntf.h
	LINK_LIBRARIES charon-groups
	PLUGIN_LISTS charon-core-modules
)

ADD_CHARON_PLUGIN(statement
	Statement.cpp
	${CHARON_CORE_INCLUDE_DIR}/Statement.h
	${CHARON_CORE_INCLUDE_DIR}/StatementIntf.h
	PLUGIN_LISTS charon-core-modules
)

ADD_CHARON_PLUGIN(forstatement
	ForStatement.cpp
	${CHARON_CORE_INCLUDE_DIR}/ForStatement.h
	${CHARON_CORE_INCLUDE_DIR}/StatementIntf.h
	PLUGIN_LISTS charon-core-modules
)

ADD_CHARON_PLUGIN(groupobject
	GroupObject.cpp
	${CHARON_CORE_INCLUDE_DIR}/GroupObject.h
	LINK_LIBRARIES charon-groups
	PLUGIN_LISTS charon-core-modules
)

ADD_CHARON_PLUGIN(inputslotbundle
	${CHARON_CORE_INCLUDE_DIR}/InputSlotBundle.h
	InputSlotBundle.cpp
	LINK_LIBRARIES charon-groups
	PLUGIN_LISTS charon-core-modules
)

ADD_CHARON_PLUGIN(outputslotbundle
	${CHARON_CORE_INCLUDE_DIR}/OutputSlotBundle.h
	OutputSlotBundle.cpp
	LINK_LIBRARIES charon-groups
	PLUGIN_LISTS charon-core-modules
)

SET_TARGET_PROPERTIES(${charon-core-modules} PROPERTIES
	FOLDER "charon-core-modules"
)

# install library
SET_TARGET_PROPERTIES(charon-core charon-plugins charon-groups PROPERTIES
	VERSION ${${PROJECT_NAME}_VERSION}
	SOVERSION ${${PROJECT_NAME}_VERSION_SHORT}
	FOLDER "charon-core"
)
INSTALL(
	TARGETS      charon-core charon-plugins charon-groups
	EXPORT       ${EXPORT_NAME}
	RUNTIME      DESTINATION bin${BIN_SUFFIX}
	LIBRARY      DESTINATION lib${LIB_SUFFIX}
	ARCHIVE      DESTINATION lib${LIB_SUFFIX}
	COMPONENT    libraries
)

# install header files to include directory
INSTALL(
	DIRECTORY   ${CHARON_CORE_INCLUDE_DIR}
	DESTINATION include
	COMPONENT   headers
)
