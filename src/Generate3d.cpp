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
/// @file Generate3d.cpp
/// implements class Generate3d
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "Generate3d.h"

using namespace std;
using namespace cimg_library;


Generate3d::Generate3d(const std::string& name) : ParameteredObject("generate3d",name,"generates 3D sequence")
{
	_addInputSlot(img,"img","image to expand in z","image");
	_addOutputSlot(list,"list","generated list");
	
	_addParameter(z,"z","depth of sequence");
	
	z=1; //default for 2d
}

void Generate3d::update()
{
	if (z==1) //2d
	{
		list = CImgList<>(img());
	}
	else //3d
	{
		list = CImgList<>(img().depth,img().width,img().height,z,img().dim);
		for(int t=0; t<list().size; t++)
			for(int d=0;d<z;d++)
				list()[t].draw_image(0,0,d,0,img().get_slice(t));
	}
	_outDataChanged(list);
}
