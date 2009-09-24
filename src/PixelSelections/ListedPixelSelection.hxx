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
 *  @file ListedPixelSelection.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 13.08.2009
 */

#ifndef _LISTEDPIXELSELECTION_HXX_
#define _LISTEDPIXELSELECTION_HXX_

#include "ListedPixelSelection.h"
#include <PixelSelection.hxx>
#include <Pixel.hxx>

template<typename T>
ListedPixelSelection<T>::ListedPixelSelection(const std::string& name) :
	PixelSelection<T>::PixelSelection(name, "ListedPixelSelection")
{

}

template<typename T>
void ListedPixelSelection<T>::execute()
{
	ParameteredObject::execute();
	forRoiXYZT(*(this->range()), x, y, z, t)
				{
					Pixel<T>* pixel = new Pixel<T> ;
					pixel->setX(x);
					pixel->setY(y);
					pixel->setZ(z);
					pixel->setT(t);
					for (unsigned int i = 0; i < (*this->sequence()).size; i++)
					{
						pixel->pushBackIntensity(
								(*this->sequence())(i, x, y, z, t));
					}
					this->pixelList.push_back(pixel);
				}
}

#endif
