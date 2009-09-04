/*
 * PixelSelection.hxx
 *
 *  Created on: 17.08.2009
 *      Author: andreas
 */
/** @file PixelSelection.hxx
 * @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 17.08.2009
 */

#ifndef _PIXELSELECTION_HXX_
#define _PIXELSELECTION_HXX_

#include "PixelSelection.h"

template<typename T>
PixelSelection<T>::PixelSelection(const std::string& name) :
	ParameteredObject("blockmatching", name,
			"save range of interest in a list of pixels")
{
	_addInputSlot(range, "range", "Range of Interest", "roi");
	_addInputSlot(image, "image", "Image to work with", "CImg");
	_addOutputSlot(pixelList, "pixelList", "List of Pixel out of Roi",
			"std::vector<Pixel<T>>*");
}

#endif
