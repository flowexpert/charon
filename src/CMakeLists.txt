INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

IF(MSVC)
	ADD_DEFINITIONS(/DHANDLE_DLL)
ENDIF(MSVC)

#Create shared libraries
ADD_LIBRARY(roi SHARED Roi.cpp)
ADD_LIBRARY(crop SHARED Crop.cpp)
TARGET_LINK_LIBRARIES(crop roi)
ADD_LIBRARY(filereader SHARED FileReader.cpp)
ADD_LIBRARY(filewriter SHARED FileWriter.cpp)
ADD_LIBRARY(imageblur SHARED ImageBlur.cpp)
TARGET_LINK_LIBRARIES(imageblur roi)
ADD_LIBRARY(imagedisplay SHARED ImageDisplay.cpp)
ADD_LIBRARY(interpolatorcubic SHARED InterpolatorCubic.cpp)
ADD_LIBRARY(interpolatorlinear SHARED InterpolatorLinear.cpp)
ADD_LIBRARY(normalize SHARED Normalize.cpp)
ADD_LIBRARY(threshold SHARED Threshold.cpp)
ADD_LIBRARY(linfilter SHARED LinFilter.cpp)

SET(Plugins
	roi
	crop
	filereader
	filewriter
	imageblur
	imagedisplay
	interpolatorcubic
	interpolatorlinear
	normalize
	threshold
	linfilter
)

FOREACH(X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core)
	IF(UNIX)
		TARGET_LINK_LIBRARIES(${X} pthread ${X11_LIBRARIES})
	ENDIF(UNIX)
	IF(WIN32)
		TARGET_LINK_LIBRARIES(${X} gdi32)
	ENDIF(WIN32)
ENDFOREACH(X)

# install library
SET_TARGET_PROPERTIES(${Plugins}
	PROPERTIES INSTALL_RPATH
	${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins
)
INSTALL(
    TARGETS      	${Plugins}
    EXPORT      	${PROJECT_NAME}
    RUNTIME      	DESTINATION bin
    LIBRARY      	DESTINATION lib${LIB_SUFFIX}/charon-plugins
    ARCHIVE      	DESTINATION lib${LIB_SUFFIX}/charon-plugins
    COMPONENT    	libraries
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


INSTALL(
    EXPORT       ${PROJECT_NAME}
    DESTINATION  include/${PROJECT_NAME}
    COMPONENT    libraries
)
