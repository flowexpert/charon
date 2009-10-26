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
 *  @brief implements the ObjectiveFunctionComparing class, child class of
 *  ObjectiveFunction
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef OBJECTIVEFUNKTIONCOMPARING_HXX_
#define OBJECTIVEFUNKTIONCOMPARING_HXX_

#include "ObjectiveFunctionComparing.h"
#include "../ObjectiveFunction.hxx"
#include "../Pixel.hxx"

template<typename T>
ObjectiveFunctionComparing<T>::ObjectiveFunctionComparing(
		const std::string& name) :
	ObjectiveFunction<T>::ObjectiveFunction(name, "ObjectiveFunctionComparing")

{

}

template<typename T>
void ObjectiveFunctionComparing<T>::execute()
{
	ParameteredObject::execute();
}

template<typename T>
T ObjectiveFunctionComparing<T>::compare(
		const std::vector<Pixel<T>*> & pixelList, const std::vector<
				IncrementorParameter<T>*>& params)
{
	T foundChange = 0;
	Pixel<T> pixel;
	Pixel<T> outPixel;
	for (unsigned int i = 0; i != pixelList.size() - 1; i++)
	{
		// compute possible changes
		this->brightnessModel()->apply(*(pixelList[i]), params, pixel);
		this->motionModel()->apply(pixel, params, outPixel);
		std::vector<T> piList = outPixel.getIntensity(); //pixel intensity list
		// compare computed pixel with know pixel out
		for (unsigned int j = 0; j != piList.size(); j++)
		{
			T predictedIntenity = piList[j];
			T foundIntensity = this->interpolator()->interpolate(
					this->sequence()[j], (float) pixel.getX(),
					(float) pixel.getY(), (float) pixel.getZ(),
					(int) pixel.getT());
			foundChange += (predictedIntenity - foundIntensity)
					* (predictedIntenity - foundIntensity);
		}
	}
	// return quadratic intensity change between computed and known Pixel
	return foundChange;
}

#endif /* OBJECTIVEFUNKTIONCOMPARING_HXX_ */
