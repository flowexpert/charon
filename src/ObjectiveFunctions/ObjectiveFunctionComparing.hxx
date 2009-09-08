/*
 * ObjectiveFunctionComparing.hxx
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/// @file ObjectiveFunctionComparing.hxx
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 24.08.2009

#ifndef OBJECTIVEFUNKTIONCOMPARING_HXX_
#define OBJECTIVEFUNKTIONCOMPARING_HXX_

#include "ObjectiveFunctionComparing.h"

template<typename T>
ObjectiveFunctionComparing<T>::ObjectiveFunctionComparing(const std::string& name) :
	ObjectiveFunction<T>::ObjectiveFunction()

{

}

template<typename T>
std::vector<T> ObjectiveFunctionComparing<T>::execute(
		const std::vector<Pixel<T> > & pixelList)
{
	for (int i = 0; i <= pixelList.size(); i++)
	{
		///@TODO make something to find the pixelsq
	}
	return this->tempFoundPixel;

}

#endif /* OBJECTIVEFUNKTIONCOMPARING_HXX_ */
