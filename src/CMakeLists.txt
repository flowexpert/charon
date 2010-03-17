INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CIMG_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
SET(CHARON_UTILS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/charon-utils)

IF(MSVC)
	ADD_DEFINITIONS(/DHANDLE_DLL)
ENDIF(MSVC)

#Create shared libraries
ADD_LIBRARY(channelconverter SHARED
	ChannelConverter.cpp
	${CHARON_UTILS_INCLUDE_DIR}/ChannelConverter.h
	${CHARON_UTILS_INCLUDE_DIR}/ChannelConverter.hxx
)
ADD_LIBRARY(roi SHARED
	Roi.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Roi.h
	${CHARON_UTILS_INCLUDE_DIR}/Roi.hxx
)
ADD_LIBRARY(crop SHARED
	Crop.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Crop.h
	${CHARON_UTILS_INCLUDE_DIR}/Crop.hxx
)
TARGET_LINK_LIBRARIES(crop roi)
ADD_LIBRARY(filereader SHARED
	FileReader.cpp
	${CHARON_UTILS_INCLUDE_DIR}/FileReader.h
	${CHARON_UTILS_INCLUDE_DIR}/FileReader.hxx
)
ADD_LIBRARY(filewriter SHARED FileWriter.cpp
	${CHARON_UTILS_INCLUDE_DIR}/FileWriter.h
	${CHARON_UTILS_INCLUDE_DIR}/FileWriter.hxx
)
ADD_LIBRARY(imageblur SHARED
	ImageBlur.cpp
	${CHARON_UTILS_INCLUDE_DIR}/ImageBlur.h
	${CHARON_UTILS_INCLUDE_DIR}/ImageBlur.hxx
)
TARGET_LINK_LIBRARIES(imageblur roi)
ADD_LIBRARY(imagedisplay SHARED
	ImageDisplay.cpp
	${CHARON_UTILS_INCLUDE_DIR}/ImageDisplay.h
	${CHARON_UTILS_INCLUDE_DIR}/ImageDisplay.hxx
)
ADD_LIBRARY(interpolatorcubic SHARED
	InterpolatorCubic.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Interpolator.h
	${CHARON_UTILS_INCLUDE_DIR}/Interpolator.hxx
	${CHARON_UTILS_INCLUDE_DIR}/InterpolatorCubic.h
	${CHARON_UTILS_INCLUDE_DIR}/InterpolatorCubic.hxx
)
ADD_LIBRARY(interpolatorlinear SHARED
	InterpolatorLinear.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Interpolator.h
	${CHARON_UTILS_INCLUDE_DIR}/Interpolator.hxx
	${CHARON_UTILS_INCLUDE_DIR}/InterpolatorLinear.h
	${CHARON_UTILS_INCLUDE_DIR}/InterpolatorLinear.hxx
)
ADD_LIBRARY(normalize SHARED
	Normalize.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Normalize.h
	${CHARON_UTILS_INCLUDE_DIR}/Normalize.hxx
)
ADD_LIBRARY(threshold SHARED
	Threshold.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Threshold.h
	${CHARON_UTILS_INCLUDE_DIR}/Threshold.hxx
)
ADD_LIBRARY(linearfilter SHARED
	LinearFilter.cpp
	${CHARON_UTILS_INCLUDE_DIR}/LinearFilter.h
	${CHARON_UTILS_INCLUDE_DIR}/LinearFilter.hxx
)
TARGET_LINK_LIBRARIES(linearfilter interpolatorlinear)
ADD_LIBRARY(mask1d SHARED
	Mask1D.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Mask1D.h
	${CHARON_UTILS_INCLUDE_DIR}/Mask1D.hxx
)
ADD_LIBRARY(structuretensor SHARED
	StructureTensor.cpp
	${CHARON_UTILS_INCLUDE_DIR}/StructureTensor.h
	${CHARON_UTILS_INCLUDE_DIR}/StructureTensor.hxx
)
ADD_LIBRARY(steeredderivative SHARED
	SteeredDerivative.cpp
	${CHARON_UTILS_INCLUDE_DIR}/SteeredDerivative.h
	${CHARON_UTILS_INCLUDE_DIR}/SteeredDerivative.hxx
)
ADD_LIBRARY(images2sequence SHARED
	Images2Sequence.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Images2Sequence.h
	${CHARON_UTILS_INCLUDE_DIR}/Images2Sequence.hxx
)
ADD_LIBRARY(warp SHARED
	Warp.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Warp.h
	${CHARON_UTILS_INCLUDE_DIR}/Warp.hxx
)

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
	images2sequence
	warp
)
SET(Plugins_CImg
	${Plugins}
)

IF(WITH_VIGRA)
	# additional vigra modules
	INCLUDE_DIRECTORIES(${VIGRA_INCLUDE_DIRS})
	ADD_LIBRARY(cimg2vigramultiarray SHARED
		CImg2VigraMultiArray.cpp
		${CHARON_UTILS_INCLUDE_DIR}/CImg2VigraMultiArray.h
		${CHARON_UTILS_INCLUDE_DIR}/CImg2VigraMultiArray.hxx
	)
	ADD_LIBRARY(vigramultiarray2cimg SHARED
		VigraMultiArray2CImg.cpp
		${CHARON_UTILS_INCLUDE_DIR}/VigraMultiArray2CImg.h
		${CHARON_UTILS_INCLUDE_DIR}/VigraMultiArray2CImg.hxx
	)
	ADD_LIBRARY(splineresize SHARED
		SplineResize.cpp
		${CHARON_UTILS_INCLUDE_DIR}/SplineResize.h
		${CHARON_UTILS_INCLUDE_DIR}/SplineResize.hxx
	)
	SET(Plugins_Vigra
		cimg2vigramultiarray
		vigramultiarray2cimg
		splineresize
	)
	LIST(APPEND Plugins_CImg
		cimg2vigramultiarray
		vigramultiarray2cimg
	)
	LIST(APPEND Plugins
		${Plugins_Vigra}
	)
	FOREACH(X ${Plugins_Vigra})
		TARGET_LINK_LIBRARIES(${X} ${VIGRA_LIBRARIES})
	ENDFOREACH(X)
ENDIF(WITH_VIGRA)

FOREACH(X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core)
ENDFOREACH(X)
FOREACH(X ${Plugins_CImg})
	TARGET_LINK_LIBRARIES(${X} ${CIMG_LINK_LIBRARIES})
ENDFOREACH(X)

# install library
SET_TARGET_PROPERTIES(${Plugins}
	PROPERTIES INSTALL_RPATH
	${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins
)
IF(NOT EXPORT_NAME)
	SET(EXPORT_NAME ${PROJECT_NAME})
ENDIF()
INSTALL(
    TARGETS      	${Plugins}
    EXPORT      	${EXPORT_NAME}
    RUNTIME      	DESTINATION bin
    LIBRARY      	DESTINATION lib${LIB_SUFFIX}/charon-plugins
    ARCHIVE      	DESTINATION lib${LIB_SUFFIX}/charon-plugins
    COMPONENT    	libraries
)

# install header files to include directory
INSTALL(
    DIRECTORY   ${CHARON_UTILS_INCLUDE_DIR}
    DESTINATION include
    COMPONENT   headers
	PATTERN ".svn" EXCLUDE
)

IF(NOT SKIP_EXPORT_INSTALL)
	INSTALL(
		EXPORT       ${EXPORT_NAME}
		DESTINATION  include/${PROJECT_NAME}
		COMPONENT    libraries
	)
ENDIF()
