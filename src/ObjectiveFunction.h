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
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef OBJECTIVEFUNKTION_H_
#define OBJECTIVEFUNKTION_H_

#include "ParameteredObject.h"
#include "BlockMatching.h"
#include "PixelSelection.h"
#include <vector>

template<typename T>
class ObjectiveFunction: ParameteredObject
{

public:
	/// standard constructor
	ObjectiveFunction(const std::string& name);
	InputSlot<cimg_library::CImgList<T>& > sequence;
	InputSlot<std::vector<Pixel<T> > *> pixelList;
//	InputSlot<Pixel<T> *> pixelToCompare;
	InputSlot<BrightnessModel<T>*> brightnessModel;
	InputSlot<MotionModel<T>*> motionModel;
	InputSlot<Interpolator<T>*> interpolator;
	OutputSlot<ObjectiveFunction<T> *> out;
	///standard execute methode from ParameteredObject
	void execute();
	/**
	 *  compares the changed List of Pixels from the apply function
	 *  (BrightnessModel) with the next picture and try to find the right
	 *  position for every pixel.
	 *  @param pixelList list of pixel from PixelSelection
	 *  @param intensity short vector of computed intensity
	 *  @param [out] vallue of T which shows the quadratic distance between the
	 *  range and the computed intensity
	 */
	virtual T compare(const std::vector<Pixel<T> > & pixelList,
			const std::vector<Parameter<T>*>& params) =0;

};

#endif /* OBJECTIVEFUNKTION_H_ */
