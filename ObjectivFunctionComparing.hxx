/*
 * ObjectivFunctionComparing.hxx
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/// @file ObjectivFunctionComparing.hxx
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 24.08.2009

#ifndef OBJECTIVFUNCTIONCOMPARING_HXX_
#define OBJECTIVFUNCTIONCOMPARING_HXX_

#include "ObjectivFunctionComparing.h"

template<typename T>
ObjectivFunctionComparing<T>::ObjectivFunctionComparing(const std::string& name) :
	ObjectivFunction<T>::ObjectivFunction()

{

}

template<typename T>
std::vector<T> ObjectivFunctionComparing<T>::execute(
		const std::vector<Pixel> & pixelList)
{
	for (int i = 0; i <= pixelList.size(); i++)
	{
		///@TODO make something to find the pixelsq
	}
	return this->tempFoundPixel;

}

#endif /* OBJECTIVFUNCTIONCOMPARING_HXX_ */
