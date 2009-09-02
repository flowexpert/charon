/*
 * ObjectivFunctionComparing.h
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/// @file ObjectivFunctionComparing.hxx
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 24.08.2009

#ifndef OBJECTIVFUNCTIONCOMPARING_H_
#define OBJECTIVFUNCTIONCOMPARING_H_

#include "ObjectivFunction.h"

template <typename T>
class ObjectivFunctionComparing : ObjectivFunction<T>
{
private:
	std::vector<T> tempFoudPixel

public:
	ObjectivFunctionComparing (const std::string& name);
	InputSlot<cimg_library::CImg<T>& > image;
	std::vector<T> execute (const std::vector<Pixel> & pixelList);

};

#endif /* OBJECTIVFUNCTIONCOMPARING_H_ */
