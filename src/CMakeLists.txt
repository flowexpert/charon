INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

IF(MSVC)
	ADD_DEFINITIONS(/DHANDLE_DLL)
ENDIF(MSVC)

#Create shared libraries
ADD_LIBRARY(channelconverter SHARED ChannelConverter.cpp ChannelConverter.h ChannelConverter.hxx)
ADD_LIBRARY(roi SHARED Roi.cpp Roi.h Roi.hxx)
ADD_LIBRARY(crop SHARED Crop.cpp Crop.h Crop.hxx)
TARGET_LINK_LIBRARIES(crop roi)
ADD_LIBRARY(filereader SHARED FileReader.cpp FileReader.h FileReader.hxx)
ADD_LIBRARY(filewriter SHARED FileWriter.cpp FileWriter.h FileWriter.hxx)
ADD_LIBRARY(imageblur SHARED ImageBlur.cpp ImageBlur.h ImageBlur.hxx)
TARGET_LINK_LIBRARIES(imageblur roi)
ADD_LIBRARY(imagedisplay SHARED ImageDisplay.cpp ImageDisplay.h ImageDisplay.hxx)
ADD_LIBRARY(interpolatorcubic SHARED InterpolatorCubic.cpp Interpolator.h Interpolator.hxx InterpolatorCubic.h InterpolatorCubic.hxx)
ADD_LIBRARY(interpolatorlinear SHARED InterpolatorLinear.cpp Interpolator.h Interpolator.hxx InterpolatorLinear.h InterpolatorLinear.hxx)
ADD_LIBRARY(normalize SHARED Normalize.cpp Normalize.h Normalize.hxx)
ADD_LIBRARY(threshold SHARED Threshold.cpp Threshold.h Threshold.hxx)
ADD_LIBRARY(linearfilter SHARED LinearFilter.cpp LinearFilter.h LinearFilter.hxx)
TARGET_LINK_LIBRARIES(linearfilter interpolatorlinear)
ADD_LIBRARY(mask1d SHARED Mask1D.cpp Mask1D.h Mask1D.hxx)
ADD_LIBRARY(structuretensor SHARED StructureTensor.cpp StructureTensor.h StructureTensor.hxx)
ADD_LIBRARY(steeredderivative SHARED SteeredDerivative.cpp SteeredDerivative.h SteeredDerivative.hxx)

SET(Plugins
	channelconverter
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
	linearfilter
	mask1d
	structuretensor
	steeredderivative
)

FOREACH(X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core ${CIMG_LINK_LIBRARIES})
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
