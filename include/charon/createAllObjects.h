/*  Copyright (C) 2009 René Steinbrügge

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// @file createAllObjects.h
/// include file for creation of all objects
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _createAllObjects_h_
#define _createAllObjects_h_

#include "MotionEstimation.h"
#include "DataAssemblers/nRange.h"
#include "MotionModels/LocalConstant.h"
#include "MotionModels/LocalAffiness.h"
#include "MotionModels/LocalPlanarity.h"
#include "MotionModels/LocalRotation.h"
#include "MotionModels/LocalStretch.h"
#include "BrightnessModels/Constant.h"
#include "BrightnessModels/Exponential.h"
#include "BrightnessModels/Diffusion.h"
#include "BrightnessModels/MovingIllumination.h"
#include "Fitters/LeastSquare.h"
#include "Derivatives/bin2.h"
#include "Derivatives/Sobel.h"
#include "functions.h"
#include "SequenceGeneration/SequenceGeneration.h"
#include "Generate3d.h"
#include <Roi.hxx>
#include "IO/FileReader.hxx"
#include "IO/FileWriter.hxx"

/// macro for creating all parametered objects to make them known
# define createAllObjects() \
	DataAssemblers::nRange __ass("dataassemblers_nrange"); \
	MotionModels::LocalRotation __mr("motionmodels_localrotation"); \
	MotionModels::LocalAffiness __ma("motionmodels_localaffiness"); \
	MotionModels::LocalStretch __ms("motionmodels_localstretch"); \
	MotionModels::LocalPlanarity __mp("motionmodels_localplanarity"); \
	MotionModels::LocalConstant __mc("motionmodels_localconstant"); \
	BrightnessModels::MovingIllumination __bm("brightnessmodels_movingillumination"); \
	BrightnessModels::Constant __bc("brightnessmodels_constant"); \
	BrightnessModels::Exponential __be("brightnessmodels_exponential"); \
	BrightnessModels::Diffusion __bd("brightnessmodels_diffusion"); \
	Fitters::LeastSquare __fi("fitters_leastsquare"); \
	Derivatives::bin2 __dbin("derivatives_bin2"); \
	Derivatives::Sobel __dsob("derivatives_sobel"); \
	MotionEstimation __moest("motionestimation"); \
	Generate3d __gen("generate3d"); \
	SequenceGeneration __seq("sequencegeneration"); \
	Roi<int> __roi("roi"); \
	FileReader<float> __filereader("filereader"); \
	FileWriter<float> __filewriter("filewriter")
	

#endif
