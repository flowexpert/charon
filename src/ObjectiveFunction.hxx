/*
 * ObjectiveFunction.hxx
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/// @file ObjectiveFunction.hxx
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 12.08.2009

#ifndef OBJECTIVEFUNKTION_HXX_
#define OBJECTIVEFUNKTION_HXX_

#include "ObjectiveFunction.h"

template<typename T>
ObjectiveFunction<T>::ObjectiveFunction(const std::string& name) :
	ParameteredObject("incrementor", name, "Compares the computed pixel"
		" (Incrementor) with the original image in time+1")
{
	_addInputSlot(image, "Image", "Image where to Increment", "CImg");
}

#endif /* OBJECTIVEFUNKTION_HXX_ */
