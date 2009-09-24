/*  This file is part of Charon.

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
/**
 *  @file ObjectiveFunction.h
 *  @brief declaration of class ObjectiveFunction
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef OBJECTIVEFUNKTION_H_
#define OBJECTIVEFUNKTION_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef objectivefunction_EXPORTS
///Visual C++ specific code
#define objectivefunction_DECLDIR __declspec(dllexport)
#else
#define objectivefunction_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define objectivefunction_DECLDIR
#endif

#include <ParameteredObject.h>
#include "BrightnessModel.h"
#include "MotionModel.h"
#include <Interpolator.h>
#include "PixelSelection.h"
#include <vector>

/// ObjectiveFunction is a class to compute the change between computed pixel 
/// parameters and real pixel parameters
template<typename T>
class objectivefunction_DECLDIR ObjectiveFunction: public TemplatedParameteredObject<
		T>
{

public:
	/// standard constructor
	ObjectiveFunction(const std::string& name = "", const std::string pluginName = "");
	/// inputslot for image sequence
	InputSlot<cimg_library::CImgList<T> *> sequence;
	/// inputslot for the list of pixel from PixelSelection
	InputSlot<std::vector<Pixel<T>*> *> pixelList;
	/// inputslot from BrightnessModel
	InputSlot<BrightnessModel<T> *> brightnessModel;
	/// inputslot from MotionModel
	InputSlot<MotionModel<T> *> motionModel;
	/// inputslot from Interpolator
	InputSlot<Interpolator<T> *> interpolator;
	/// this outputslot
	OutputSlot<ObjectiveFunction<T> *> out;
	/// standard execute methode from ParameteredObject
	virtual void execute() =0;

	/**
	 *  compares the changed List of Pixels from the apply function
	 *  (BrightnessModel) with the next picture and try to find the right
	 *  position for every pixel.
	 *  @param pixelList list of pixel from PixelSelection
	 *  @param intensity short vector of computed intensity
	 *  @param [out] vallue of T which shows the quadratic distance between the
	 *  range and the computed intensity
	 */
	virtual T compare(const std::vector<Pixel<T>*> & pixelList,
			const std::vector<IncrementorParameter<T>*>& params) =0;

};

#endif /* OBJECTIVEFUNKTION_H_ */
