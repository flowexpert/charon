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
 *  @file PixelSelection.hxx
 *  @brief implementation of abstract class PixelSelection
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 17.08.2009
 */

#ifndef _PIXELSELECTION_HXX_
#define _PIXELSELECTION_HXX_

#include "PixelSelection.h"

template<typename T>
PixelSelection<T>::PixelSelection(const std::string& name,
		const std::string& pluginName) :
	TemplatedParameteredObject<T> (pluginName, name,
			"save range of interest in a list of pixels")
{
	out = this;
	ParameteredObject::_addInputSlot(range, "range", "Range of Interest", "roi<int>");
	ParameteredObject::_addInputSlot(sequence, "sequence", "Sequence to work with",
			"CImgList<T>");
	ParameteredObject::_addOutputSlot(out, "out", "List of Pixel out of Roi",
			"PixelSelection<T> *");
	_addFunction(PixelSelection<T>::getListOfPixel);
}

template<typename T>
std::vector<Pixel<T>*>& PixelSelection<T>::getListOfPixel()
{
	return this->pixelList;
}

#endif
