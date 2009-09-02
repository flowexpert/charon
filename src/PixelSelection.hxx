/*
 * AbstractPixelSelection.hxx
 *
 *  Created on: 17.08.2009
 *      Author: andreas
 */

#ifndef _PIXELSELECTION_HXX_
#define _PIXELSELECTION_HXX_

#include "PixelSelection.h"

template <typename T>
PixelSelection<T>::PixelSelection(const std::string& name) :
        ParameteredObject("blockmatching", name,
            "save range of interest in a list of pixels")
{
	_addInputSlot(range, "range", "Range of Interest",  "rangeofinterest");
	_addInputSlot(image, "image", "Image to work with", "image");
}

#endif
