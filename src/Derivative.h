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
/// @file Derivative.h
/// defines abstract class Derivative
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _DERIVATIVE_H_
#define _DERIVATIVE_H_

#include <CImg.h>
#include <ParameteredObject.hxx>

/// abstract class for derivatives
template <class T>
class Derivative  : public TemplatedParameteredObject<T>
{
public:
	/// derfault constructor	
	Derivative(const std::string& classname, const std::string& name = "") : 
		TemplatedParameteredObject<T>(classname,name,"calculates the derivatives")
	{
		_addInputSlot(img,"img","image on which derivatives are calculatet","CImgList<T>");
		/*_addOutputSlot(out,"this","Pointer to itself","Derivative*");
		out = this; //*/
	}
	
	/// input slot for the image on which the derivatives are calculated
	InputSlot<cimg_library::CImgList<T> > img;
};

#endif