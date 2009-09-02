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

#ifndef _Derivative_h_
#define _Derivative_h_

#include "main.h"

/// abstract class for derivatives
class Derivative  : public ParameteredObject
{
protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const = 0;
	
	/// calculates the derivatives
	virtual void calculateDerivatives() = 0;
private:
public:
	/// derfault constructor	
	Derivative(const std::string& classname, const std::string& name = "") : 
		ParameteredObject(classname,name,"calculates the derivatives")
	{
		_addInputSlot(img,"img","image on which derivatives are calculatet","CImgList");
		/*_addOutputSlot(out,"this","Pointer to itself","Derivative*");
		out = this; //*/
	}
	
	/// input slot for the image on which the derivatives are calculated
	InputSlot<cimg_library::CImgList<> > img;
	
	virtual void update() {calculateDerivatives();}

	
	/*OutputSlot<Derivative*> out;
	virtual void getX(const cimg_library::CImg<>& img, cimg_library::CImg<>& res)=0;
	virtual void getY(const cimg_library::CImg<>& img, cimg_library::CImg<>& res)=0;
	virtual void getZ(const cimg_library::CImg<>& img, cimg_library::CImg<>& res)=0; //*/
};


#endif
