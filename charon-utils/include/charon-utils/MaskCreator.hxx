/*  Copyright (C) 2012 Gerald Mwangi

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
/** \file MaskCreator.hxx
 *  Implementation of the parameter class MaskCreator.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 13.11.2012
 */

#ifndef _MASKCREATOR_HXX_
#define _MASKCREATOR_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "MaskCreator.h"


template <typename T>
MaskCreator<T>::MaskCreator(const std::string& name) :
		TemplatedParameteredObject<T>(
			"MaskCreator", name,
			"<h2>Creates a binary mask</h2><br>"
			"Creates a binary mask. One can optionally connect an input "
			"image to set the dimensions or the set them manually with the "
			"parameters. This plugin DOES NOT actually mask the image but "
			"only creates a binary mask. Use binoperator to apply the mask."
		)
{
	ParameteredObject::_addInputSlot(
		roi, "roi",
		"Roi to mask",
		"Roi<int>*");
	ParameteredObject::_addInputSlot(
		image, "image",
		"image to get the dimensions. OPTIONAL",
		"vigraArray5<T>");

	ParameteredObject::_addOutputSlot(
		out, "mask",
		"Binary mask",
		"vigraArray5<T>");

	ParameteredObject::_addParameter< int >(
		size_x, "size_x",
		"Dimension in x direction. Overriden by image",
		0, "int");
	ParameteredObject::_addParameter< int >(
		size_y, "size_y",
		"Dimension in y direction. Overriden by image",
		0, "int");
	ParameteredObject::_addParameter< int >(
		size_z, "size_z",
		"Dimension in z direction. Overriden by image",
		0, "int");
	ParameteredObject::_addParameter< int >(
		sizeT, "size_t",
		"Dimension in t direction. Overriden by image",
		0, "int");
	ParameteredObject::_addParameter< int >(
		size_v, "size_v",
		"Dimension in v direction. Overriden by image",
		0, "int");
	ParameteredObject::_addParameter(
		invert_roi, "invert_roi",
		"Set the roi to false, and the surrounding to true",
		"bool");

}

template <typename T>
void MaskCreator<T>::execute() {
	int sx=size_x;
	int sy=size_y;
	int sz=size_z;
	int st=sizeT;
	int sv=size_v;
	if(image.connected())
	{
		const vigra::MultiArrayView<5, T>& i =image();
		sx=i.size(0);
		sy=i.size(1);
		sz=i.size(2);
		st=i.size(3);
		sv=i.size(4);

	}

	vigra::MultiArray<5, T>& o = out();
	o.reshape(typename vigra::MultiArray<5, T>::difference_type(
			sx, sy, sz, st, sv),invert_roi());
	int xend=std::min(sx,roi()->xEnd());
	int yend=std::min(sy,roi()->yEnd());
	int zend=std::min(sz,roi()->zEnd());
	int tend=std::min(st,roi()->tEnd());
	int vend=std::min(sv,roi()->vEnd());
	for(int x=roi()->xBegin;x<xend;x++)
		for(int y=roi()->yBegin;y<yend;y++)
			for(int z=roi()->zBegin;z<zend;z++)
				for(int t=roi()->tBegin;t<tend;t++)
					for(int v=roi()->vBegin;v<vend;v++)
						o(x,y,z,t,v)=!invert_roi();

}

#endif /* _MASKCREATOR_HXX_ */
