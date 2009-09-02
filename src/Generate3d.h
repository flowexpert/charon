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
/// @file Generate3d.h
/// defines class Generate3d
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _Generate3d_h_
#define _Generate3d_h_

#include "main.h"

//FIXME expansion in z richtung wird nicht mehr benoetigt
// klasse nur noch zur convertierung von CIMg nach CImgList

/// @brief class which generates 3D images
/// @details generates 3D images from 2D images by expanding them
///	in z-direction or converts 2D CImg image into a CImgList
class Generate3d : public ParameteredObject
{
protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const
		{return new Generate3d(name);}		

public:
	/// default constructor
	Generate3d(const std::string& name = "");
	
	/// 2D image input slot
	InputSlot<cimg_library::CImg<> > img;
	
	/// image output slot
	OutputSlot<cimg_library::CImgList<> > list;
	
	/// @brief the depth of the resulting image
	/// @details if z is set to 1 the input image will be transformed into a CImgList without change
	/// 	otherwise a 3D image will be generated by expanding the image into z-direction
	Parameter<int> z;
			
	virtual void update();
	
};

#endif
