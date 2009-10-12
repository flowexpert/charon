#files for Charon library
SET(CHARON_FILES
	SplitStream
	StringTool
	FileTool
	AbstractData
	Parameter
	Slots
	ParameterFile
	ParameteredObject
	TypeDetector
)
SET(CHARON_GROUP)
FOREACH(CURFILE ${CHARON_FILES})
	FILE(GLOB TEMP
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		${CURFILE}.cpp ${CURFILE}.h ${CURFILE}.hxx
	)
	LIST(APPEND CHARON_GROUP ${TEMP})
ENDFOREACH(CURFILE ${CHARON_FILES})

SET(PLUGIN_FILES
	AbstractPluginLoader.cpp
	AbstractPluginLoader.h
	PluginManager.cpp
	PluginManager.h
	PluginManagerInterface.h
)
IF(WIN32)
	LIST(APPEND PLUGIN_FILES
		WindowsPluginLoader.h
		WindowsPluginLoader.cpp
	)
ENDIF(WIN32)
IF(UNIX)
	LIST(APPEND PLUGIN_FILES
		UnixPluginLoader.h
		UnixPluginLoader.cpp
	)
ENDIF(UNIX)

# create libraries
ADD_LIBRARY(${PROJECT_NAME} SHARED ${CHARON_GROUP})

SET_PROPERTY(TARGET ${PROJECT_NAME}
	PROPERTY COMPILE_DEFINITIONS CREATE_SHARED
)
IF(UNIX)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} dl)
ENDIF(UNIX)
ADD_LIBRARY(charon-plugins STATIC ${PLUGIN_FILES})
IF(MSVC)
	SET_PROPERTY(TARGET charon-plugins
		PROPERTY COMPILE_DEFINITIONS DLL_EXPORT
	)
ENDIF(MSVC)
TARGET_LINK_LIBRARIES(charon-plugins ${PROJECT_NAME})

# Configure Paths.config file
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/Paths.config.in
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Paths.config
	@ONLY
)
INSTALL(
    FILES	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Paths.config
    DESTINATION share/${PROJECT_NAME}
    COMPONENT   libraries
)

# install library
INSTALL(
    TARGETS      ${PROJECT_NAME} charon-plugins
    EXPORT       ${PROJECT_NAME}
    RUNTIME      DESTINATION bin
    LIBRARY      DESTINATION lib${LIB_SUFFIX}
    ARCHIVE      DESTINATION lib${LIB_SUFFIX}
    COMPONENT    libraries
)

INSTALL(
    EXPORT       ${PROJECT_NAME}
    DESTINATION  include/${PROJECT_NAME}
    COMPONENT    libraries
)

# install header files to include directory
FILE(GLOB HEADER_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.h *.hxx
)
INSTALL(
    FILES       ${HEADER_FILES}
    DESTINATION include/${PROJECT_NAME}
    COMPONENT   headers
)
