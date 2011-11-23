INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
SET(CHARON_UTILS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/charon-utils)

IF(MSVC)
	ADD_DEFINITIONS(/DHANDLE_DLL)
ENDIF(MSVC)

# Create shared libraries
ADD_LIBRARY(roi SHARED
	Roi.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Roi.h
	${CHARON_UTILS_INCLUDE_DIR}/Roi.hxx
)
SET(Plugins
	roi
)

IF(USE_CIMG)
	INCLUDE_DIRECTORIES(${CIMG_INCLUDE_DIRS})
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
	ADD_LIBRARY(channelconverter SHARED
		ChannelConverter.cpp
		${CHARON_UTILS_INCLUDE_DIR}/ChannelConverter.h
		${CHARON_UTILS_INCLUDE_DIR}/ChannelConverter.hxx
	)
	ADD_LIBRARY(crop SHARED
		Crop.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Crop.h
		${CHARON_UTILS_INCLUDE_DIR}/Crop.hxx
	)
	TARGET_LINK_LIBRARIES(crop roi)
	ADD_LIBRARY(resize SHARED
		Resize.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Resize.h
		${CHARON_UTILS_INCLUDE_DIR}/Resize.hxx
	)
	TARGET_LINK_LIBRARIES(resize roi)
	ADD_LIBRARY(filereader SHARED
		FileReader.cpp
		${CHARON_UTILS_INCLUDE_DIR}/FileReader.h
		${CHARON_UTILS_INCLUDE_DIR}/FileReader.hxx
	)
	ADD_LIBRARY(floreader SHARED
		FloReader.cpp
		${CHARON_UTILS_INCLUDE_DIR}/FloReader.h
		${CHARON_UTILS_INCLUDE_DIR}/FloReader.hxx
	)
	ADD_LIBRARY(flowriter SHARED
		FloWriter.cpp
		${CHARON_UTILS_INCLUDE_DIR}/FloWriter.h
		${CHARON_UTILS_INCLUDE_DIR}/FloWriter.hxx
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
	ADD_LIBRARY(normalize SHARED
		Normalize.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Normalize.h
		${CHARON_UTILS_INCLUDE_DIR}/Normalize.hxx
	)
	ADD_LIBRARY(medianfilter SHARED
		MedianFilter.cpp
		${CHARON_UTILS_INCLUDE_DIR}/MedianFilter.h
		${CHARON_UTILS_INCLUDE_DIR}/MedianFilter.hxx
	)
	ADD_LIBRARY(rgb2hsv SHARED
		Rgb2Hsv.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Rgb2Hsv.h
		${CHARON_UTILS_INCLUDE_DIR}/Rgb2Hsv.hxx
	)
	ADD_LIBRARY(flow2hsv SHARED
		Flow2HSV.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Flow2HSV.h
		${CHARON_UTILS_INCLUDE_DIR}/Flow2HSV.hxx
	)
	ADD_LIBRARY(colormask SHARED
		ColorMask.cpp
		${CHARON_UTILS_INCLUDE_DIR}/ColorMask.h
		${CHARON_UTILS_INCLUDE_DIR}/ColorMask.hxx
	)
	ADD_LIBRARY(flowquiver SHARED
		FlowQuiver.cpp
		${CHARON_UTILS_INCLUDE_DIR}/FlowQuiver.h
		${CHARON_UTILS_INCLUDE_DIR}/FlowQuiver.hxx
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
	ADD_LIBRARY(warpsymmetric SHARED
		WarpSymmetric.cpp
		${CHARON_UTILS_INCLUDE_DIR}/WarpSymmetric.h
		${CHARON_UTILS_INCLUDE_DIR}/WarpSymmetric.hxx
	)
	SET(Plugins_CImg
		channelconverter
		crop
		resize
		filereader
		filewriter
		floreader
		flowriter
		imageblur
		imagedisplay
		interpolatorcubic
		interpolatorlinear
		normalize
		medianfilter
		rgb2hsv
		flow2hsv
		colormask
		flowquiver
		linearfilter
		mask1d
		structuretensor
		steeredderivative
		images2sequence
		warp
		warpsymmetric
	)

	LIST(APPEND Plugins
		${Plugins_CImg}
	)
ENDIF(USE_CIMG)

IF(USE_VIGRA)
	# additional vigra modules
	INCLUDE_DIRECTORIES(${Vigra_INCLUDE_DIRS})
	ADD_LIBRARY(splineresize SHARED
		SplineResize.cpp
		${CHARON_UTILS_INCLUDE_DIR}/SplineResize.h
		${CHARON_UTILS_INCLUDE_DIR}/SplineResize.hxx
	)
	ADD_LIBRARY(threshold SHARED
		Threshold.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Threshold.h
		${CHARON_UTILS_INCLUDE_DIR}/Threshold.hxx
	)
	ADD_LIBRARY(binoperator SHARED
		BinOperator.cpp
		${CHARON_UTILS_INCLUDE_DIR}/BinOperator.h
		${CHARON_UTILS_INCLUDE_DIR}/BinOperator.hxx
	)
	ADD_LIBRARY(dilationerosion SHARED
		DilationErosion.cpp
		${CHARON_UTILS_INCLUDE_DIR}/DilationErosion.h
		${CHARON_UTILS_INCLUDE_DIR}/DilationErosion.hxx
	)
	ADD_LIBRARY(subarray SHARED
		Subarray.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Subarray.h
		${CHARON_UTILS_INCLUDE_DIR}/Subarray.hxx
	)
	TARGET_LINK_LIBRARIES(subarray roi)
	ADD_LIBRARY(mirror SHARED
		Mirror.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Mirror.h
		${CHARON_UTILS_INCLUDE_DIR}/Mirror.hxx
	)

	SET(Plugins_Vigra
		splineresize
		threshold
		binoperator
		dilationerosion
		subarray
		mirror
	)
	IF(USE_CIMG)
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
		LIST(APPEND Plugins_Vigra
			cimg2vigramultiarray
			vigramultiarray2cimg
		)
		LIST(APPEND Plugins_CImg
			cimg2vigramultiarray
			vigramultiarray2cimg
		)
	ENDIF(USE_CIMG)

	IF(USE_HDF5)
		INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIRS})
		IF(USE_MPI)
			INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})
		ENDIF()
		ADD_LIBRARY(filewriterhdf5 SHARED
			FileWriterHDF5.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FileWriterHDF5.h
			${CHARON_UTILS_INCLUDE_DIR}/FileWriterHDF5.hxx
		)
		ADD_LIBRARY(filereaderhdf5 SHARED
			FileReaderHDF5.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FileReaderHDF5.h
			${CHARON_UTILS_INCLUDE_DIR}/FileReaderHDF5.hxx
		)
		TARGET_LINK_LIBRARIES(filereaderhdf5 roi)
		SET(Plugins_HDF5
			filewriterhdf5
			filereaderhdf5
		)
		# workaround to avoid linking against debug libraries in release mode,
		# setup of variable HDF5_LIBRARIES is buggy in old cmake versions.
		# fixed at least in cmake 2.8.5
		#SET(HDF5_LIBRARIES ${HDF5_hdf5_LIBRARY} ${HDF5_hdf5_hl_LIBRARY})
		#MESSAGE("HDF5_LIBRARIES: ${HDF5_LIBRARIES}")
		FOREACH(X ${Plugins_HDF5})
			TARGET_LINK_LIBRARIES(${X} ${HDF5_LIBRARIES})
			IF(USE_MPI)
				TARGET_LINK_LIBRARIES(${X} ${MPI_LIBRARIES})
			ENDIF()
		ENDFOREACH(X)
		LIST(APPEND Plugins_Vigra ${Plugins_HDF5})
	ENDIF(USE_HDF5)

	IF(USE_QT)
		SET(QT_USE_QTOPENGL 1)
		INCLUDE(${QT_USE_FILE})
		INCLUDE_DIRECTORIES(qt)
		ADD_SUBDIRECTORY(qt)
		SET(Plugins_QT)

		ADD_LIBRARY(argosdisplay SHARED
			ArgosDisplay.cpp
			${CHARON_UTILS_INCLUDE_DIR}/ArgosDisplay.h
			${CHARON_UTILS_INCLUDE_DIR}/ArgosDisplay.hxx
		)
		ADD_LIBRARY(frameselect SHARED
			FrameSelect.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FrameSelect.h
			${CHARON_UTILS_INCLUDE_DIR}/FrameSelect.hxx
		)
		TARGET_LINK_LIBRARIES(frameselect roi)
		LIST(APPEND Plugins_Qt
			argosdisplay
			frameselect
		)
		LIST(APPEND Plugins_CImg
			argosdisplay
		)

		IF(USE_Boost)
			INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
			ADD_LIBRARY(statisticsdisplay SHARED
				StatisticsDisplay.cpp
				${CHARON_UTILS_INCLUDE_DIR}/StatisticsDisplay.h
				${CHARON_UTILS_INCLUDE_DIR}/StatisticsDisplay.hxx
			)
			LIST(APPEND Plugins_Qt
				statisticsdisplay
			)
			LIST(APPEND Plugins_CImg
				statisticsdisplay
			)
		ENDIF(USE_Boost)

		LIST(APPEND Plugins
			${Plugins_Qt}
		)
		FOREACH(X ${Plugins_Qt})
			TARGET_LINK_LIBRARIES(${X} charonwidgets ${QT_LIBRARIES})
		ENDFOREACH(X)
	ENDIF(USE_QT)

	FOREACH(X ${Plugins_Vigra})
		TARGET_LINK_LIBRARIES(${X} ${VIGRA_LIBRARIES})
		IF(USE_MPI)
			TARGET_LINK_LIBRARIES(${X} ${MPI_LIBRARIES})
		ENDIF()
	ENDFOREACH(X)
	LIST(APPEND Plugins ${Plugins_Vigra})
ENDIF(USE_VIGRA)

IF(USE_CIMG)
	FOREACH(X ${Plugins_CImg})
		TARGET_LINK_LIBRARIES(${X} ${CIMG_LINK_LIBRARIES})
	ENDFOREACH(X)
ENDIF(USE_CIMG)

FOREACH(X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core)
	SET_TARGET_PROPERTIES(${X} PROPERTIES
		FOLDER "charon-utils Modules"
	)
ENDFOREACH(X)

# install library
SET_TARGET_PROPERTIES(${Plugins} PROPERTIES
	INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins
)
INSTALL(
	TARGETS          ${Plugins}
	EXPORT           ${EXPORT_NAME}
	RUNTIME          DESTINATION bin${BIN_SUFFIX}
	LIBRARY          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	ARCHIVE          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	COMPONENT        libraries
)

# install header files to include directory
INSTALL(
	DIRECTORY        ${CHARON_UTILS_INCLUDE_DIR}
	DESTINATION      include
	COMPONENT        headers
	PATTERN          ".svn" EXCLUDE
)

IF(NOT SKIP_EXPORT_INSTALL)
	INSTALL(
		EXPORT       ${EXPORT_NAME}
		DESTINATION  ${CONFIG_INSTALL}
		COMPONENT    libraries
	)
ENDIF()
