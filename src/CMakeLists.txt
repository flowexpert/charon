#files for Charon library
SET(CHARON_FILES
    SplitStream.cpp
    StringTool.cpp
    FileTool.cpp
    Parameter.cpp
    ParameterFile.cpp
    ParameteredObject.cpp
    TypeDetector.cpp
)

SET(PLUGIN_FILES
	AbstractPluginLoader.cpp
	PluginManager.cpp
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
ADD_LIBRARY(${PROJECT_NAME} SHARED ${CHARON_FILES})

SET_PROPERTY(TARGET ${PROJECT_NAME}
	PROPERTY COMPILE_DEFINITIONS CREATE_SHARED
)
ADD_LIBRARY(charon-plugins STATIC ${PLUGIN_FILES})
SET_PROPERTY(TARGET charon-plugins
	PROPERTY COMPILE_DEFINITIONS DLL_EXPORT
)
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
