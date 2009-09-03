/*
 * ObjectivFunction.hxx
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/// @file ObjectivFunction.hxx
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 12.08.2009

#ifndef OBJECTIVFUNCTION_HXX_
#define OBJECTIVFUNCTION_HXX_

#include "ObjectivFunction.h"

template<typename T>
ObjectivFunction<T>::ObjectivFunction(const std::string& name) :
	ParameteredObject("incrementor", name, "Compares the computed pixel"
		" (Incrementor) with the original image in time+1")
{
	_addInputSlot(image, "Image", "Image where to Increment", "CImg");
}

#endif /* OBJECTIVFUNCTION_HXX_ */
