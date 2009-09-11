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
 *  @file ObjectiveFunctionComparing.hxx
 *  implements the ObjectiveFunctionComparing class
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef OBJECTIVEFUNKTIONCOMPARING_HXX_
#define OBJECTIVEFUNKTIONCOMPARING_HXX_

#include "ObjectiveFunctionComparing.h"
#include "ObjectiveFunction.hxx"
#include "ListedPixelSelection.hxx"

template<typename T>
ObjectiveFunctionComparing<T>::ObjectiveFunctionComparing(
		const std::string& name) :
	ObjectiveFunction<T>::ObjectiveFunction(name)

{

}

template<typename T>
T ObjectiveFunctionComparing<T>::compare(
		const std::vector<Pixel<T> > & pixelList, const std::vector<
				Parameter<T>*>& params)
{
	T foundChange = 0;
	Pixel<T> pixel;
	std::vector<T> piList;
	for (unsigned int i = 0; i != pixelList.size(); i++)
	{
		pixel = this->brightnessModel().apply(pixelList[i], params);
		piList = pixel.getIntensity();
		for (unsigned int j = 0; j != piList.size(); j++)
		{
			foundChange += this->interpolator()->interpolate(
					this->sequence[piList[i]], pixel.getX(), pixel.getY(),
					pixel.getZ(), pixel.getT());
		}
		pixel = this->motionModel.apply(pixelList[i], params);
	}
	return foundChange;
}

#endif /* OBJECTIVEFUNKTIONCOMPARING_HXX_ */
