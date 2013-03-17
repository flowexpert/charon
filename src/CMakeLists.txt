INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
SET(CHARON_UTILS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/charon-utils)

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIRECTORIES})
IF(Qwt5_Qt4_FOUND)
	INCLUDE_DIRECTORIES(${Qwt5_INCLUDE_DIR})
ENDIF(Qwt5_Qt4_FOUND)

IF(USE_MathGL)
	INCLUDE_DIRECTORIES(${MathGL_INCLUDE_DIR})
ENDIF(USE_MathGL)

IF(MSVC)
	ADD_DEFINITIONS(/DHANDLE_DLL)
ENDIF(MSVC)

# Create shared libraries
ADD_LIBRARY(roi SHARED
	Roi.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Roi.h
	${CHARON_UTILS_INCLUDE_DIR}/Roi.hxx
)
ADD_LIBRARY(statementgenerator SHARED
	StatementGenerator.cpp
	${CHARON_UTILS_INCLUDE_DIR}/StatementGenerator.h
)
ADD_LIBRARY(parametertoslot SHARED
	ParameterToSlot.cpp
	${CHARON_UTILS_INCLUDE_DIR}/ParameterToSlot.h
)
ADD_LIBRARY(greatersmallerthenpar SHARED
	GreaterSmallerThenPar.cpp
	${CHARON_UTILS_INCLUDE_DIR}/GreaterSmallerThenPar.h
	${CHARON_UTILS_INCLUDE_DIR}/GreaterSmallerThenPar.hxx
)
ADD_LIBRARY(valuebuffer SHARED
	ValueBuffer.cpp
	${CHARON_UTILS_INCLUDE_DIR}/ValueBuffer.h
	${CHARON_UTILS_INCLUDE_DIR}/ValueBuffer.hxx
)
ADD_LIBRARY(binoperatorvalue SHARED
	BinOperatorValue.cpp
	${CHARON_UTILS_INCLUDE_DIR}/BinOperatorValue.h
	${CHARON_UTILS_INCLUDE_DIR}/BinOperatorValue.hxx
)
ADD_LIBRARY(printvalue SHARED
	PrintValue.cpp
	${CHARON_UTILS_INCLUDE_DIR}/PrintValue.h
)
SET(Plugins
	roi
	statementgenerator
	greatersmallerthenpar
	parametertoslot
	printvalue
	valuebuffer
	binoperatorvalue
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
	ADD_LIBRARY(discretizebythreshold SHARED
		DiscretizeByThreshold.cpp
		${CHARON_UTILS_INCLUDE_DIR}/DiscretizeByThreshold.h
		${CHARON_UTILS_INCLUDE_DIR}/DiscretizeByThreshold.hxx
	)
	ADD_LIBRARY(resize SHARED
		Resize.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Resize.h
		${CHARON_UTILS_INCLUDE_DIR}/Resize.hxx
	)
	ADD_LIBRARY(min SHARED
		Min.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Min.h
		${CHARON_UTILS_INCLUDE_DIR}/Min.hxx
	)
	IF(USE_QT)
		SET(filereaderwatcher_HEADERS_TOMOC ${CHARON_UTILS_INCLUDE_DIR}/FileReaderWatcher.h)
		QT4_WRAP_CPP(filereaderwatcher_HEADERS_MOCKED ${filereaderwatcher_HEADERS_TOMOC})
		ADD_LIBRARY(filereader SHARED
			FileReader.cpp
			FileReaderWatcher.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FileReader.h
			${CHARON_UTILS_INCLUDE_DIR}/FileReader.hxx
			${filereaderwatcher_HEADERS_MOCKED}
		)
		TARGET_LINK_LIBRARIES(filereader ${QT_LIBRARIES})
	ELSE(USE_QT)
		ADD_LIBRARY(filereader SHARED
			FileReader.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FileReader.h
			${CHARON_UTILS_INCLUDE_DIR}/FileReader.hxx
		)
	ENDIF(USE_QT)
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
	ADD_LIBRARY(layeredmedianfilter SHARED
		LayeredMedianFilter.cpp
		${CHARON_UTILS_INCLUDE_DIR}/LayeredMedianFilter.h
		${CHARON_UTILS_INCLUDE_DIR}/LayeredMedianFilter.hxx
	)
	ADD_LIBRARY(quantize SHARED
		Quantize.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Quantize.h
		${CHARON_UTILS_INCLUDE_DIR}/Quantize.hxx
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
	ADD_LIBRARY(endpointerror SHARED
		EndpointError.cpp
		${CHARON_UTILS_INCLUDE_DIR}/EndpointError.h
		${CHARON_UTILS_INCLUDE_DIR}/EndpointError.hxx
	)
	ADD_LIBRARY(angularerror SHARED
		AngularError.cpp
		${CHARON_UTILS_INCLUDE_DIR}/AngularError.h
		${CHARON_UTILS_INCLUDE_DIR}/AngularError.hxx
	)
	ADD_LIBRARY(lengtherror SHARED
		LengthError.cpp
		${CHARON_UTILS_INCLUDE_DIR}/LengthError.h
		${CHARON_UTILS_INCLUDE_DIR}/LengthError.hxx
	)
	ADD_LIBRARY(colormask SHARED
		ColorMask.cpp
		${CHARON_UTILS_INCLUDE_DIR}/ColorMask.h
		${CHARON_UTILS_INCLUDE_DIR}/ColorMask.hxx
	)
	ADD_LIBRARY(overlay SHARED
		Overlay.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Overlay.h
		${CHARON_UTILS_INCLUDE_DIR}/Overlay.hxx
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
	ADD_LIBRARY(mux2 SHARED
		Mux2.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Mux2.h
		${CHARON_UTILS_INCLUDE_DIR}/Mux2.hxx
	)
	ADD_LIBRARY(demux2 SHARED
		Demux2.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Demux2.h
		${CHARON_UTILS_INCLUDE_DIR}/Demux2.hxx
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
	ADD_LIBRARY(emptycimg SHARED
		EmptyCImg.cpp
		${CHARON_UTILS_INCLUDE_DIR}/EmptyCImg.h
		${CHARON_UTILS_INCLUDE_DIR}/EmptyCImg.hxx
	)
	ADD_LIBRARY(drawcimg SHARED
		DrawCImg.cpp
		${CHARON_UTILS_INCLUDE_DIR}/DrawCImg.h
		${CHARON_UTILS_INCLUDE_DIR}/DrawCImg.hxx
	)
	ADD_LIBRARY(gammacorrection SHARED
		GammaCorrection.cpp
		${CHARON_UTILS_INCLUDE_DIR}/GammaCorrection.h
		${CHARON_UTILS_INCLUDE_DIR}/GammaCorrection.hxx
	)
	ADD_LIBRARY(amplitudethreshold SHARED
		AmplitudeThreshold.cpp
		${CHARON_UTILS_INCLUDE_DIR}/AmplitudeThreshold.h
		${CHARON_UTILS_INCLUDE_DIR}/AmplitudeThreshold.hxx
	)
	ADD_LIBRARY(binoperatorcimg SHARED
		BinOperatorCImg.cpp
		${CHARON_UTILS_INCLUDE_DIR}/BinOperatorCImg.h
		${CHARON_UTILS_INCLUDE_DIR}/BinOperatorCImg.hxx
	)
	ADD_LIBRARY(maskedblur SHARED
		MaskedBlur.cpp
		${CHARON_UTILS_INCLUDE_DIR}/MaskedBlur.h
		${CHARON_UTILS_INCLUDE_DIR}/MaskedBlur.hxx
	)
	ADD_LIBRARY(erodedilate SHARED
		ErodeDilate.cpp
		${CHARON_UTILS_INCLUDE_DIR}/ErodeDilate.h
		${CHARON_UTILS_INCLUDE_DIR}/ErodeDilate.hxx
	)
	ADD_LIBRARY(strictresizexy SHARED
		StrictResizeXY.cpp
		${CHARON_UTILS_INCLUDE_DIR}/StrictResizeXY.h
		${CHARON_UTILS_INCLUDE_DIR}/StrictResizeXY.hxx
	)
	ADD_LIBRARY(size SHARED
		Size.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Size.h
		${CHARON_UTILS_INCLUDE_DIR}/Size.hxx
	)
	ADD_LIBRARY(fitsizes SHARED
		FitSizes.cpp
		${CHARON_UTILS_INCLUDE_DIR}/FitSizes.h
		${CHARON_UTILS_INCLUDE_DIR}/FitSizes.hxx
	)
	ADD_LIBRARY(cut SHARED
		Cut.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Cut.h
		${CHARON_UTILS_INCLUDE_DIR}/Cut.hxx
	)
	SET(Plugins_CImg
		channelconverter
		crop
		discretizebythreshold
		resize
		min
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
		layeredmedianfilter
		rgb2hsv
		flow2hsv
		endpointerror
		angularerror
		lengtherror
		colormask
		overlay
		flowquiver
		linearfilter
		mask1d
		structuretensor
		steeredderivative
		images2sequence
		mux2
		demux2
		warp
		warpsymmetric
		emptycimg
		drawcimg
		gammacorrection
		amplitudethreshold
		binoperatorcimg
		maskedblur
		quantize
		erodedilate
		strictresizexy
		size
		fitsizes
		cut
	)
	IF(USE_QT)
		SET(toggletimeout_HEADERS_TOMOC ${CHARON_UTILS_INCLUDE_DIR}/ToggleTimeout.h)
		QT4_WRAP_CPP(toggletimeout_HEADERS_MOCKED ${toggletimeout_HEADERS_TOMOC})
		ADD_LIBRARY(toggle SHARED
			${toggletimeout_HEADERS_MOCKED}
			ToggleTimeout.cpp
			${CHARON_UTILS_INCLUDE_DIR}/Toggle.h
			${CHARON_UTILS_INCLUDE_DIR}/Toggle.hxx
			Toggle.cpp
		)
		TARGET_LINK_LIBRARIES(toggle ${QT_LIBRARIES})
		LIST(APPEND Plugins_CImg
			toggle
		)
	ENDIF(USE_QT)
	IF(USE_PNG)
		ADD_LIBRARY(kittireader SHARED
			KittiReader.cpp
			${CHARON_UTILS_INCLUDE_DIR}/KittiReader.h
			${CHARON_UTILS_INCLUDE_DIR}/KittiReader.hxx
			${CHARON_UTILS_INCLUDE_DIR}/kitti-devkit/io_flow.h
			${CHARON_UTILS_INCLUDE_DIR}/kitti-devkit/io_disp.h
		)
		ADD_LIBRARY(kittiwriter SHARED
			KittiWriter.cpp
			${CHARON_UTILS_INCLUDE_DIR}/KittiWriter.h
			${CHARON_UTILS_INCLUDE_DIR}/KittiWriter.hxx
			${CHARON_UTILS_INCLUDE_DIR}/kitti-devkit/io_flow.h
			${CHARON_UTILS_INCLUDE_DIR}/kitti-devkit/io_disp.h
		)
		LIST(APPEND Plugins_CImg
			kittireader
			kittiwriter
		)
	ENDIF(USE_PNG)
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
		LIST(APPEND Plugins_Vigra
			filewriterhdf5
			filereaderhdf5
		)
		FOREACH(X ${Plugins_Vigra})
			TARGET_LINK_LIBRARIES(${X} ${HDF5_LIBRARIES})
			IF(USE_MPI)
				TARGET_LINK_LIBRARIES(${X} ${MPI_LIBRARIES})
			ENDIF()
		ENDFOREACH(X)
	ENDIF(USE_HDF5)

	IF(USE_QT)
		SET(QT_USE_QTOPENGL 1)
		INCLUDE(${QT_USE_FILE})
		INCLUDE_DIRECTORIES(qt)
		ADD_SUBDIRECTORY(qt)
		SET(Plugins_QT)

		QT4_WRAP_CPP(argosdisplayreloader_MOC
			${CHARON_UTILS_INCLUDE_DIR}/ArgosDisplayReloader.h)
		ADD_LIBRARY(argosdisplay SHARED
			ArgosDisplay.cpp
			${CHARON_UTILS_INCLUDE_DIR}/ArgosDisplay.h
			${CHARON_UTILS_INCLUDE_DIR}/ArgosDisplay.hxx
			${CHARON_UTILS_INCLUDE_DIR}/ArgosDisplayReloader.h
			ArgosDisplayReloader.cpp
			${argosdisplayreloader_MOC}
		)
		TARGET_LINK_LIBRARIES(argosdisplay ${QT_LIBRARIES})
		ADD_LIBRARY(frameselect SHARED
			FrameSelect.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FrameSelect.h
			${CHARON_UTILS_INCLUDE_DIR}/FrameSelect.hxx
		)
		ADD_LIBRARY(frameselectcimg SHARED
			FrameSelectCImg.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FrameSelectCImg.h
			${CHARON_UTILS_INCLUDE_DIR}/FrameSelectCImg.hxx
		)

		QT4_WRAP_CPP(Scriptor_MOC
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorExecuter.h)
		ADD_LIBRARY(scriptorcimg SHARED
			${Scriptor_MOC}
			ScriptorCimgPlugin.cpp
			ScriptorExecuter.cpp
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorBase.h
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorBase.hxx
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorCimg.h
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorCimg.hxx
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorCimgPlugin.h
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorCimgPlugin.hxx
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorExecuter.h
		)
		ADD_LIBRARY(scriptorvigra SHARED
			${Scriptor_MOC}
			ScriptorVigraPlugin.cpp
			ScriptorExecuter.cpp
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorBase.h
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorBase.hxx
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorVigra.h
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorVigra.hxx
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorVigraPlugin.h
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorVigraPlugin.hxx
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorExecuter.h
		)
		TARGET_LINK_LIBRARIES(scriptorvigra filereaderhdf5 filewriterhdf5)

		LIST(APPEND Plugins_Qt
			argosdisplay
			frameselect
			scriptorcimg
			scriptorvigra
			frameselectcimg
		)
		LIST(APPEND Plugins_CImg
			argosdisplay
			scriptorcimg
			frameselectcimg
		)
		LIST(APPEND Plugins_Vigra
			argosdisplay
			scriptorvigra
			frameselect
		)

		IF(USE_MathGL)
			QT4_WRAP_CPP(quiverplot_MOC
				${CHARON_UTILS_INCLUDE_DIR}/QuiverPlot.h)
			ADD_LIBRARY(quiver SHARED
				Quiver.cpp
				${CHARON_UTILS_INCLUDE_DIR}/Quiver.h
				${CHARON_UTILS_INCLUDE_DIR}/Quiver.hxx
				${CHARON_UTILS_INCLUDE_DIR}/QuiverPlot.h
				QuiverPlot.cpp
				${quiverplot_MOC}
			)
			TARGET_LINK_LIBRARIES(quiver ${QT_LIBRARIES} ${MathGL_LIBS})
			LIST(APPEND Plugins_Qt quiver)
			LIST(APPEND Plugins_CImg quiver)
		ENDIF(USE_MathGL)

		IF(USE_Qwt5)
			ADD_LIBRARY(residualplot SHARED
				ResidualPlot.cpp
				${CHARON_UTILS_INCLUDE_DIR}/ResidualPlot.h
				${CHARON_UTILS_INCLUDE_DIR}/ResidualPlot.hxx
			)
			LIST(APPEND Plugins_Qt
				residualplot
			)
			LIST(APPEND Plugins_CImg
				residualplot
			)
		ENDIF(USE_Qwt5)

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
			LIST(APPEND Plugins_Vigra
				statisticsdisplay
			)

			ADD_LIBRARY(customcolormask SHARED
				CustomColorMask.cpp
				${CHARON_UTILS_INCLUDE_DIR}/CustomColorMask.hxx
				${CHARON_UTILS_INCLUDE_DIR}/CustomColorMask.h
			)
			LIST(APPEND Plugins_Qt
				customcolormask
			)
			LIST(APPEND Plugins_CImg
				customcolormask
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
		TARGET_LINK_LIBRARIES(${X} vigraimpex)
		IF(USE_MPI)
			TARGET_LINK_LIBRARIES(${X} ${MPI_LIBRARIES})
		ENDIF()
		IF(USE_HDF5)
			TARGET_LINK_LIBRARIES(${X} ${HDF5_LIBRARIES})
		ENDIF()
	ENDFOREACH(X)
	LIST(APPEND Plugins ${Plugins_Vigra})
ENDIF(USE_VIGRA)

IF(USE_CIMG)
	FOREACH(X ${Plugins_CImg})
		TARGET_LINK_LIBRARIES(${X} ${CIMG_LINK_LIBRARIES})
	ENDFOREACH(X)
ENDIF(USE_CIMG)

LIST(REMOVE_DUPLICATES Plugins)
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
