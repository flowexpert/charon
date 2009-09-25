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
/// @file bin2.hxx
/// implements class bin2
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _DERIVATIVES_BIN2_HXX_
#define _DERIVATIVES_BIN2_HXX_

#include "bin2.h"

template <class T>
bin2<T>::bin2(const std::string& name) : Derivative<T>("derivatives_bin2",name)
{
	this->_addOutputSlot(dx,"dx","derivative in x", "CImgList<T>");
	this->_addOutputSlot(dy,"dy","derivative in y", "CImgList<T>");
	this->_addOutputSlot(dz,"dz","derivative in z", "CImgList<T>");
	this->_addOutputSlot(dt,"dt","derivative in t", "CImgList<T>");
}

template <class T>
void bin2<T>::execute()
{
	ParameteredObject::execute();
	if (this->dx.connected())
	{
		this->dx()->assign(this->img()->size);
		cimg_library::CImg<T> f(3,1,1,1,0.5,0.0,-0.5);
		for (unsigned int t=0; t < this->img()->size; t++)
			this->dx()[t]=this->img()[t].get_convolve(f);
		
	}
	if (this->dy.connected())
	{
		this->dy()->assign(this->img()->size);
		cimg_library::CImg<T> f(1,3,1,1,0.5,0.0,-0.5);
		for (unsigned int t=0; t < this->img()->size; t++)
			this->dy()[t]=this->img()[t].get_convolve(f);
		
	}
	if (this->dz.connected())
	{
		this->dz()->assign(this->img()->size);
		cimg_library::CImg<T> f(1,1,3,1,0.5,0.0,-0.5);
		for (unsigned int t=0; t < this->img()->size; t++)
			this->dz()[t]=this->img()[t].get_convolve(f);
		
	}
	if (this->dt.connected())
	{
		this->dt()->assign(this->img()->size);
		cimg_library::CImg<T> f(1,1,1,3,0.5,0.0,-0.5);
		for (unsigned int t=0; t < this->img()->size; t++)
			this->dt()[t]=this->img()[t].get_convolve(f);
	}
}

#endif