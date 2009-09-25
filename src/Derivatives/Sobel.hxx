/*  Copyright (C) 2009 René Steinbrügge

    This file is part of Charon.

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
/// @file Sobel.cpp
/// implements class Sobel
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _DERIVATIVES_SOBEL_HXX_
#define _DERIVATIVES_SOBEL_HXX_

#include "Sobel.h"

template <class T>
Sobel<T>::Sobel(const std::string& name) : Derivative<T>("derivatives_sobel",name), filterx(3,3,1,1,\
						 3.0/32,	0.0,	 -3.0/32, \
						10.0/32,	0.0,	-10.0/32, \
						 3.0/32,	0.0,	 -3.0/32)
{
	filtery = filterx.get_transpose();
	this->_addOutputSlot(dx,"dx","derivative in x", "CImgList<T>");
	this->_addOutputSlot(dy,"dy","derivative in y", "CImgList<T>");
}

template <class T>
void Sobel<T>::execute()
{
	if (this->dx.connected())
	{
		*(this->dx()) = cimg_library::CImgList<T>(this->img()->size);
		for (unsigned int t=0; t<this->img()->size; t++)
			this->dx()[t]=this->img()[t].get_convolve(this->filterx);		
	}
	
	if (this->dy.connected())
	{
		*(this->dy()) = cimg_library::CImgList<T>(this->img()->size);
		for (unsigned int t=0; t<this->img()->size; t++)
			this->dy()[t]=this->img()[t].get_convolve(this->filtery);
	}
}

#endif
