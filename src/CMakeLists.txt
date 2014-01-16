INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
SET(CHARON_UTILS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/charon-utils)

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIRECTORIES})
#IF(Qwt5_Qt4_FOUND)
#	INCLUDE_DIRECTORIES(${Qwt5_INCLUDE_DIR})
#ENDIF(Qwt5_Qt4_FOUND)

IF(USE_MathGL)
	INCLUDE_DIRECTORIES(${MathGL_INCLUDE_DIR})
ENDIF(USE_MathGL)

IF(MSVC)
	ADD_DEFINITIONS(/DHANDLE_DLL)
ENDIF(MSVC)

SET(Plugins) #main plugin list

# Create libraries

# ---------------------- Plugins without dependencies -------------------------

ADD_CHARON_PLUGIN(roi
	Roi.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Roi.h
	${CHARON_UTILS_INCLUDE_DIR}/Roi.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(roioffset
	RoiOffset.cpp
	${CHARON_UTILS_INCLUDE_DIR}/RoiOffset.h
	${CHARON_UTILS_INCLUDE_DIR}/RoiOffset.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(statementgenerator
	StatementGenerator.cpp
	${CHARON_UTILS_INCLUDE_DIR}/StatementGenerator.h
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(parametertoslot
	ParameterToSlot.cpp
	${CHARON_UTILS_INCLUDE_DIR}/ParameterToSlot.h
	${CHARON_UTILS_INCLUDE_DIR}/ParameterToSlot.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(int2uint
	Int2Uint.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Int2Uint.h
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(uint2int
	Uint2Int.cpp
	${CHARON_UTILS_INCLUDE_DIR}/Uint2Int.h
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(greatersmallerthenpar
	GreaterSmallerThenPar.cpp
	${CHARON_UTILS_INCLUDE_DIR}/GreaterSmallerThenPar.h
	${CHARON_UTILS_INCLUDE_DIR}/GreaterSmallerThenPar.hxx
	PLUGIN_LISTS Plugins
)

ADD_CHARON_PLUGIN(valuebuffer
	ValueBuffer.cpp
	${CHARON_UTILS_INCLUDE_DIR}/ValueBuffer.h
	${CHARON_UTILS_INCLUDE_DIR}/ValueBuffer.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(binoperatorvalue
	BinOperatorValue.cpp
	${CHARON_UTILS_INCLUDE_DIR}/BinOperatorValue.h
	${CHARON_UTILS_INCLUDE_DIR}/BinOperatorValue.hxx
	PLUGIN_LISTS Plugins
)
ADD_CHARON_PLUGIN(printvalue
	PrintValue.cpp
	${CHARON_UTILS_INCLUDE_DIR}/PrintValue.h
	PLUGIN_LISTS Plugins
)

# ----------------------- Plugins which use only CImg -------------------------


IF(USE_CIMG)
	SET(Plugins_CImg)
	INCLUDE_DIRECTORIES(${CIMG_INCLUDE_DIRS})
	ADD_CHARON_PLUGIN(interpolatordiscrete
		InterpolatorDiscrete.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Interpolator.h
		${CHARON_UTILS_INCLUDE_DIR}/Interpolator.hxx
		${CHARON_UTILS_INCLUDE_DIR}/InterpolatorDiscrete.h
		${CHARON_UTILS_INCLUDE_DIR}/InterpolatorDiscrete.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(interpolatorcubic
		InterpolatorCubic.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Interpolator.h
		${CHARON_UTILS_INCLUDE_DIR}/Interpolator.hxx
		${CHARON_UTILS_INCLUDE_DIR}/InterpolatorCubic.h
		${CHARON_UTILS_INCLUDE_DIR}/InterpolatorCubic.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(interpolatorlinear
		InterpolatorLinear.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Interpolator.h
		${CHARON_UTILS_INCLUDE_DIR}/Interpolator.hxx
		${CHARON_UTILS_INCLUDE_DIR}/InterpolatorLinear.h
		${CHARON_UTILS_INCLUDE_DIR}/InterpolatorLinear.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(channelconverter
		ChannelConverter.cpp
		${CHARON_UTILS_INCLUDE_DIR}/ChannelConverter.h
		${CHARON_UTILS_INCLUDE_DIR}/ChannelConverter.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(crop
		Crop.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Crop.h
		${CHARON_UTILS_INCLUDE_DIR}/Crop.hxx
		LINK_LIBRARIES debug roi
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(discretizebythreshold
		DiscretizeByThreshold.cpp
		${CHARON_UTILS_INCLUDE_DIR}/DiscretizeByThreshold.h
		${CHARON_UTILS_INCLUDE_DIR}/DiscretizeByThreshold.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(resize
		Resize.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Resize.h
		${CHARON_UTILS_INCLUDE_DIR}/Resize.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(min
		Min.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Min.h
		${CHARON_UTILS_INCLUDE_DIR}/Min.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)

	ADD_CHARON_PLUGIN(floreader
		FloReader.cpp
		${CHARON_UTILS_INCLUDE_DIR}/FloReader.h
		${CHARON_UTILS_INCLUDE_DIR}/FloReader.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(flowriter
		FloWriter.cpp
		${CHARON_UTILS_INCLUDE_DIR}/FloWriter.h
		${CHARON_UTILS_INCLUDE_DIR}/FloWriter.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(filewriter FileWriter.cpp
		${CHARON_UTILS_INCLUDE_DIR}/FileWriter.h
		${CHARON_UTILS_INCLUDE_DIR}/FileWriter.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(imageblur
		ImageBlur.cpp
		${CHARON_UTILS_INCLUDE_DIR}/ImageBlur.h
		${CHARON_UTILS_INCLUDE_DIR}/ImageBlur.hxx
		LINK_LIBRARIES debug roi
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(imagedisplay
		ImageDisplay.cpp
		${CHARON_UTILS_INCLUDE_DIR}/ImageDisplay.h
		${CHARON_UTILS_INCLUDE_DIR}/ImageDisplay.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(normalize
		Normalize.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Normalize.h
		${CHARON_UTILS_INCLUDE_DIR}/Normalize.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(medianfilter
		MedianFilter.cpp
		${CHARON_UTILS_INCLUDE_DIR}/MedianFilter.h
		${CHARON_UTILS_INCLUDE_DIR}/MedianFilter.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(layeredmedianfilter
		LayeredMedianFilter.cpp
		${CHARON_UTILS_INCLUDE_DIR}/LayeredMedianFilter.h
		${CHARON_UTILS_INCLUDE_DIR}/LayeredMedianFilter.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(quantize
		Quantize.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Quantize.h
		${CHARON_UTILS_INCLUDE_DIR}/Quantize.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(rgb2hsv
		Rgb2Hsv.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Rgb2Hsv.h
		${CHARON_UTILS_INCLUDE_DIR}/Rgb2Hsv.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(flow2hsv
		Flow2HSV.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Flow2HSV.h
		${CHARON_UTILS_INCLUDE_DIR}/Flow2HSV.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(endpointerror
		EndpointError.cpp
		${CHARON_UTILS_INCLUDE_DIR}/EndpointError.h
		${CHARON_UTILS_INCLUDE_DIR}/EndpointError.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(angularerror
		AngularError.cpp
		${CHARON_UTILS_INCLUDE_DIR}/AngularError.h
		${CHARON_UTILS_INCLUDE_DIR}/AngularError.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(lengtherror
		LengthError.cpp
		${CHARON_UTILS_INCLUDE_DIR}/LengthError.h
		${CHARON_UTILS_INCLUDE_DIR}/LengthError.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(colormask
		ColorMask.cpp
		${CHARON_UTILS_INCLUDE_DIR}/ColorMask.h
		${CHARON_UTILS_INCLUDE_DIR}/ColorMask.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(overlay
		Overlay.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Overlay.h
		${CHARON_UTILS_INCLUDE_DIR}/Overlay.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(flowquiver
		FlowQuiver.cpp
		${CHARON_UTILS_INCLUDE_DIR}/FlowQuiver.h
		${CHARON_UTILS_INCLUDE_DIR}/FlowQuiver.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(linearfilter
		LinearFilter.cpp
		${CHARON_UTILS_INCLUDE_DIR}/LinearFilter.h
		${CHARON_UTILS_INCLUDE_DIR}/LinearFilter.hxx
		LINK_LIBRARIES interpolatorlinear
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(mask1d
		Mask1D.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Mask1D.h
		${CHARON_UTILS_INCLUDE_DIR}/Mask1D.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(structuretensor
		StructureTensor.cpp
		${CHARON_UTILS_INCLUDE_DIR}/StructureTensor.h
		${CHARON_UTILS_INCLUDE_DIR}/StructureTensor.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(steeredderivative
		SteeredDerivative.cpp
		${CHARON_UTILS_INCLUDE_DIR}/SteeredDerivative.h
		${CHARON_UTILS_INCLUDE_DIR}/SteeredDerivative.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(images2sequence
		Images2Sequence.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Images2Sequence.h
		${CHARON_UTILS_INCLUDE_DIR}/Images2Sequence.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(mux2
		Mux2.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Mux2.h
		${CHARON_UTILS_INCLUDE_DIR}/Mux2.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(demux2
		Demux2.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Demux2.h
		${CHARON_UTILS_INCLUDE_DIR}/Demux2.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(warp
		Warp.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Warp.h
		${CHARON_UTILS_INCLUDE_DIR}/Warp.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(warpsymmetric
		WarpSymmetric.cpp
		${CHARON_UTILS_INCLUDE_DIR}/WarpSymmetric.h
		${CHARON_UTILS_INCLUDE_DIR}/WarpSymmetric.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(emptycimg
		EmptyCImg.cpp
		${CHARON_UTILS_INCLUDE_DIR}/EmptyCImg.h
		${CHARON_UTILS_INCLUDE_DIR}/EmptyCImg.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(drawcimg
		DrawCImg.cpp
		${CHARON_UTILS_INCLUDE_DIR}/DrawCImg.h
		${CHARON_UTILS_INCLUDE_DIR}/DrawCImg.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(gammacorrection
		GammaCorrection.cpp
		${CHARON_UTILS_INCLUDE_DIR}/GammaCorrection.h
		${CHARON_UTILS_INCLUDE_DIR}/GammaCorrection.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(amplitudethreshold
		AmplitudeThreshold.cpp
		${CHARON_UTILS_INCLUDE_DIR}/AmplitudeThreshold.h
		${CHARON_UTILS_INCLUDE_DIR}/AmplitudeThreshold.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(binoperatorcimg
		BinOperatorCImg.cpp
		${CHARON_UTILS_INCLUDE_DIR}/BinOperatorCImg.h
		${CHARON_UTILS_INCLUDE_DIR}/BinOperatorCImg.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(maskedblur
		MaskedBlur.cpp
		${CHARON_UTILS_INCLUDE_DIR}/MaskedBlur.h
		${CHARON_UTILS_INCLUDE_DIR}/MaskedBlur.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(erodedilate
		ErodeDilate.cpp
		${CHARON_UTILS_INCLUDE_DIR}/ErodeDilate.h
		${CHARON_UTILS_INCLUDE_DIR}/ErodeDilate.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(strictresizexy
		StrictResizeXY.cpp
		${CHARON_UTILS_INCLUDE_DIR}/StrictResizeXY.h
		${CHARON_UTILS_INCLUDE_DIR}/StrictResizeXY.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(size
		Size.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Size.h
		${CHARON_UTILS_INCLUDE_DIR}/Size.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(fitsizes
		FitSizes.cpp
		${CHARON_UTILS_INCLUDE_DIR}/FitSizes.h
		${CHARON_UTILS_INCLUDE_DIR}/FitSizes.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)
	ADD_CHARON_PLUGIN(cut
		Cut.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Cut.h
		${CHARON_UTILS_INCLUDE_DIR}/Cut.hxx
		PLUGIN_LISTS Plugins Plugins_CImg
	)

	IF(USE_PNG)
		ADD_CHARON_PLUGIN(kittireader
			KittiReader.cpp
			${CHARON_UTILS_INCLUDE_DIR}/KittiReader.h
			${CHARON_UTILS_INCLUDE_DIR}/KittiReader.hxx
			${CHARON_UTILS_INCLUDE_DIR}/kitti-devkit/io_flow.h
			${CHARON_UTILS_INCLUDE_DIR}/kitti-devkit/io_disp.h
			PLUGIN_LISTS Plugins Plugins_CImg
		)
		ADD_CHARON_PLUGIN(kittiwriter
			KittiWriter.cpp
			${CHARON_UTILS_INCLUDE_DIR}/KittiWriter.h
			${CHARON_UTILS_INCLUDE_DIR}/KittiWriter.hxx
			${CHARON_UTILS_INCLUDE_DIR}/kitti-devkit/io_flow.h
			${CHARON_UTILS_INCLUDE_DIR}/kitti-devkit/io_disp.h
			PLUGIN_LISTS Plugins Plugins_CImg
		)
	ENDIF(USE_PNG)

	IF(USE_QT)
		SET(toggletimeout_HEADERS_TOMOC ${CHARON_UTILS_INCLUDE_DIR}/ToggleTimeout.h)
		QT4_WRAP_CPP(toggletimeout_HEADERS_MOCKED ${toggletimeout_HEADERS_TOMOC})
		ADD_CHARON_PLUGIN(toggle
			Toggle.cpp
			ToggleTimeout.cpp
			${CHARON_UTILS_INCLUDE_DIR}/Toggle.h
			${CHARON_UTILS_INCLUDE_DIR}/Toggle.hxx
			${toggletimeout_HEADERS_MOCKED}
			PLUGIN_LISTS Plugins Plugins_CImg
			LINK_LIBRARIES ${QT_LIBRARIES}
		)

		SET(filereaderwatcher_HEADERS_TOMOC ${CHARON_UTILS_INCLUDE_DIR}/FileReaderWatcher.h)
		QT4_WRAP_CPP(filereaderwatcher_HEADERS_MOCKED ${filereaderwatcher_HEADERS_TOMOC})
		ADD_CHARON_PLUGIN(filereader
			FileReader.cpp
			FileReaderWatcher.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FileReader.h
			${CHARON_UTILS_INCLUDE_DIR}/FileReader.hxx
			${filereaderwatcher_HEADERS_MOCKED}
			PLUGIN_LISTS Plugins Plugins_CImg
			LINK_LIBRARIES ${QT_LIBRARIES}
		)

		#ADD_CHARON_PLUGIN(filereader
		#	FileReader.cpp
		#	${CHARON_UTILS_INCLUDE_DIR}/FileReader.h
		#	${CHARON_UTILS_INCLUDE_DIR}/FileReader.hxx
		#	PLUGIN_LISTS Plugins Plugins_CImg
		#	LINK_LIBRARIES ${QT_LIBRARIES}
		#)

		QT4_WRAP_CPP(Scriptor_MOC
			${CHARON_UTILS_INCLUDE_DIR}/Scriptor/ScriptorExecuter.h)
		ADD_CHARON_PLUGIN(scriptorcimg
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
			PLUGIN_LISTS Plugins Plugins_CImg Plugins_QT
			LINK_LIBRARIES ${QT_LIBRARIES}
		)
	ENDIF(USE_QT)

ENDIF(USE_CIMG)

# ----------------------- Plugins which use Vigra -------------------------

IF(USE_VIGRA)
	#additional vigra modules
	INCLUDE_DIRECTORIES(${Vigra_INCLUDE_DIRS})
	SET(Plugins_Vigra)
	ADD_CHARON_PLUGIN(maskcreator 
		MaskCreator.cpp
		${CHARON_UTILS_INCLUDE_DIR}/MaskCreator.h
		${CHARON_UTILS_INCLUDE_DIR}/MaskCreator.hxx
		PLUGIN_LISTS Plugins Plugins_Vigra
	)
        ADD_CHARON_PLUGIN(structuretexturedecomposition
                StructureTextureDecomposition.cpp
                ${CHARON_UTILS_INCLUDE_DIR}/StructureTextureDecomposition.h
                ${CHARON_UTILS_INCLUDE_DIR}/StructureTextureDecomposition.hxx
                PLUGIN_LISTS Plugins Plugins_Vigra
        )
	ADD_CHARON_PLUGIN(splineresize
		SplineResize.cpp
		${CHARON_UTILS_INCLUDE_DIR}/SplineResize.h
		${CHARON_UTILS_INCLUDE_DIR}/SplineResize.hxx
		PLUGIN_LISTS Plugins Plugins_Vigra
	)
	ADD_CHARON_PLUGIN(threshold
		Threshold.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Threshold.h
		${CHARON_UTILS_INCLUDE_DIR}/Threshold.hxx
		PLUGIN_LISTS Plugins Plugins_Vigra
	)
	ADD_CHARON_PLUGIN(binoperator
		BinOperator.cpp
		${CHARON_UTILS_INCLUDE_DIR}/BinOperator.h
		${CHARON_UTILS_INCLUDE_DIR}/BinOperator.hxx
		PLUGIN_LISTS Plugins Plugins_Vigra
	)
	ADD_CHARON_PLUGIN(dilationerosion
		DilationErosion.cpp
		${CHARON_UTILS_INCLUDE_DIR}/DilationErosion.h
		${CHARON_UTILS_INCLUDE_DIR}/DilationErosion.hxx
		PLUGIN_LISTS Plugins Plugins_Vigra
	)
	ADD_CHARON_PLUGIN(subarray
		Subarray.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Subarray.h
		${CHARON_UTILS_INCLUDE_DIR}/Subarray.hxx
		LINK_LIBRARIES debug roi
		PLUGIN_LISTS Plugins Plugins_Vigra
	)
	ADD_CHARON_PLUGIN(mirror
		Mirror.cpp
		${CHARON_UTILS_INCLUDE_DIR}/Mirror.h
		${CHARON_UTILS_INCLUDE_DIR}/Mirror.hxx
		PLUGIN_LISTS Plugins Plugins_Vigra
	)

	IF(USE_CIMG)
		ADD_CHARON_PLUGIN(cimg2vigramultiarray
			CImg2VigraMultiArray.cpp
			${CHARON_UTILS_INCLUDE_DIR}/CImg2VigraMultiArray.h
			${CHARON_UTILS_INCLUDE_DIR}/CImg2VigraMultiArray.hxx
			PLUGIN_LISTS Plugins Plugins_Vigra Plugins_CImg
		)
		ADD_CHARON_PLUGIN(vigramultiarray2cimg
			VigraMultiArray2CImg.cpp
			${CHARON_UTILS_INCLUDE_DIR}/VigraMultiArray2CImg.h
			${CHARON_UTILS_INCLUDE_DIR}/VigraMultiArray2CImg.hxx
			PLUGIN_LISTS Plugins Plugins_Vigra Plugins_CImg
		)
	ENDIF(USE_CIMG)

	IF(USE_HDF5)
		INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIRS})
		IF(USE_MPI)
			INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})
		ENDIF()
		ADD_CHARON_PLUGIN(filewriterhdf5
			FileWriterHDF5.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FileWriterHDF5.h
			${CHARON_UTILS_INCLUDE_DIR}/FileWriterHDF5.hxx
			PLUGIN_LISTS Plugins Plugins_Vigra
		)
		ADD_CHARON_PLUGIN(filereaderhdf5
			FileReaderHDF5.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FileReaderHDF5.h
			${CHARON_UTILS_INCLUDE_DIR}/FileReaderHDF5.hxx
			LINK_LIBRARIES debug roi
			PLUGIN_LISTS Plugins Plugins_Vigra
		)
	ENDIF(USE_HDF5)

	IF(USE_QT)
		#SET(Plugins_Qt)
		SET(QT_USE_QTOPENGL 1)
		INCLUDE(${QT_USE_FILE})
		INCLUDE_DIRECTORIES(qt)
		ADD_SUBDIRECTORY(qt)

		QT4_WRAP_CPP(argosdisplayreloader_MOC
			${CHARON_UTILS_INCLUDE_DIR}/ArgosDisplayReloader.h)
		ADD_CHARON_PLUGIN(argosdisplay
			ArgosDisplay.cpp
			${CHARON_UTILS_INCLUDE_DIR}/ArgosDisplay.h
			${CHARON_UTILS_INCLUDE_DIR}/ArgosDisplay.hxx
			${CHARON_UTILS_INCLUDE_DIR}/ArgosDisplayReloader.h
			ArgosDisplayReloader.cpp
			${argosdisplayreloader_MOC}
			PLUGIN_LISTS Plugins Plugins_CImg Plugins_Vigra Plugins_Qt
		)

		ADD_CHARON_PLUGIN(frameselect
			FrameSelect.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FrameSelect.h
			${CHARON_UTILS_INCLUDE_DIR}/FrameSelect.hxx
			PLUGIN_LISTS Plugins Plugins_Vigra Plugins_Qt
		)
		ADD_CHARON_PLUGIN(frameselectcimg
			FrameSelectCImg.cpp
			${CHARON_UTILS_INCLUDE_DIR}/FrameSelectCImg.h
			${CHARON_UTILS_INCLUDE_DIR}/FrameSelectCImg.hxx
			PLUGIN_LISTS Plugins Plugins_CImg Plugins_Qt
		)

		ADD_CHARON_PLUGIN(scriptorvigra
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
			LINK_LIBRARIES filereaderhdf5 filewriterhdf5
			PLUGIN_LISTS Plugins Plugins_Qt Plugins_Vigra
		)

		IF(USE_MathGL)
			QT4_WRAP_CPP(quiverplot_MOC
				${CHARON_UTILS_INCLUDE_DIR}/QuiverPlot.h)
			ADD_CHARON_PLUGIN(quiver
				Quiver.cpp
				${CHARON_UTILS_INCLUDE_DIR}/Quiver.h
				${CHARON_UTILS_INCLUDE_DIR}/Quiver.hxx
				${CHARON_UTILS_INCLUDE_DIR}/QuiverPlot.h
				QuiverPlot.cpp
				${quiverplot_MOC}
				LINK_LIBRARIES ${MathGL_LIBS}
				PLUGIN_LISTS Plugins Plugins_Qt Plugins_CImg
			)
		ENDIF(USE_MathGL)

		#IF(USE_Qwt5)
		#	ADD_CHARON_PLUGIN(residualplot
		#		ResidualPlot.cpp
		#		${CHARON_UTILS_INCLUDE_DIR}/ResidualPlot.h
		#		${CHARON_UTILS_INCLUDE_DIR}/ResidualPlot.hxx
		#		LINK_LIBRARIES ${Qwt5_LIBRARIES}
		#		PLUGIN_LISTS Plugins Plugins_Qt Plugins_CImg
		#	)
		#ENDIF(USE_Qwt5)

		IF(USE_Boost)
			INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
			SET(Plugins_Boost)
			#right now we only use boost header-only libraries, no linkage needed
			ADD_CHARON_PLUGIN(statisticsdisplay
				StatisticsDisplay.cpp
				${CHARON_UTILS_INCLUDE_DIR}/StatisticsDisplay.h
				${CHARON_UTILS_INCLUDE_DIR}/StatisticsDisplay.hxx
				PLUGIN_LISTS Plugins Plugins_Qt Plugins_CImg Plugins_Vigra
			)

			ADD_CHARON_PLUGIN(customcolormask
				CustomColorMask.cpp
				${CHARON_UTILS_INCLUDE_DIR}/CustomColorMask.hxx
				${CHARON_UTILS_INCLUDE_DIR}/CustomColorMask.h
				PLUGIN_LISTS Plugins Plugins_Qt Plugins_CImg
			)
		ENDIF(USE_Boost)

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
ENDIF(USE_VIGRA)

IF(USE_CIMG)
	FOREACH(X ${Plugins_CImg})
		TARGET_LINK_LIBRARIES(${X} ${CIMG_LINK_LIBRARIES})
	ENDFOREACH(X)
ENDIF(USE_CIMG)

FOREACH(X ${Plugins})
	SET_TARGET_PROPERTIES(${X} PROPERTIES
		FOLDER "charon-utils Modules"
	)
ENDFOREACH(X)

# install header files to include directory
INSTALL(
	DIRECTORY        ${CHARON_UTILS_INCLUDE_DIR}
	DESTINATION      include
	COMPONENT        headers
)
