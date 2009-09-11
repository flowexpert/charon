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
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
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
	_addInputSlot(sequence, "sequence", "Sequence to work with",
			"cimg_library::CImgList<T>& ");
	_addOutputSlot(pixelList, "pixelList", "List of Pixel out of Roi",
			"std::vector<Pixel<T> > *");
}

template<typename T>
void Pixel<T>::setX(const T& newX)
{
	this->x = newX;
}

template<typename T>
const T& Pixel<T>::getX() const
{
	return this->x;
}

template<typename T>
void Pixel<T>::setY(const T& newY)
{
	this->y = newY;
}

template<typename T>
const T& Pixel<T>::getY() const
{
	return this->y;
}

template<typename T>
void Pixel<T>::setZ(const T& newZ)
{
	this->z = newZ;
}

template<typename T>
const T& Pixel<T>::getZ() const
{
	return this->z;
}

template<typename T>
void Pixel<T>::setT(const T& newT)
{
	this->t = newT;
}

template<typename T>
const T& Pixel<T>::getT() const
{
	return this->t;
}

template<typename T>
void Pixel<T>::pushBackIntensity(const T& newI)
{
	this->intensity.push_back(newI);
}

template<typename T>
void Pixel<T>::insertIntensity(const int pos, const T& updatedI)
{
	this->intensity[pos] = updatedI;
}

template<typename T>
const std::vector<T>& Pixel<T>::getIntensity() const
{
	return this->intensity;
}

#endif
