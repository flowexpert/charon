/*
 * ObjectiveFunctionComparing.h
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/// @file ObjectiveFunctionComparing.hxx
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 24.08.2009

#ifndef OBJECTIVEFUNKTIONCOMPARING_H_
#define OBJECTIVEFUNKTIONCOMPARING_H_

#include "ObjectiveFunction.h"

template <typename T>
class ObjectiveFunctionComparing : ObjectiveFunction<T>
{
private:
	std::vector<T> tempFoudPixel;

public:
	ObjectiveFunctionComparing (const std::string& name);
	InputSlot<cimg_library::CImg<T>& > image;
	std::vector<T> execute (const std::vector<Pixel<T> > & pixelList);

};

#endif /* OBJECTIVEFUNKTIONCOMPARING_H_ */
