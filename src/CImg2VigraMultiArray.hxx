/*  Copyright (C) 2009 Daniel Kondermann

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
/// @file CImg2VigraMultiArray.hxx
/// Implementation of the parameter class CImg2VigraMultiArray.
/// @author Daniel Kondermann
/// @date 19.01.2010

#ifndef _CIMG2VIGRAMULTIARRAY_HXX_
#define _CIMG2VIGRAMULTIARRAY_HXX_

#include "CImg2VigraMultiArray.h"

template <typename T>
CImg2VigraMultiArray<T>::CImg2VigraMultiArray(const std::string& name) :
		TemplatedParameteredObject<T>("CImg2VigraMultiArray", name,
			"Converts a CImgList-Object to a vigra::MultiArray<5, T>. "
			"The data copied. As the MultiArray may not have images of "
			"varying size, the maximum image size of the CImgList is applied "
			"to all images. Missing values are filled according to the "
			"missingValue parameter.")
{
	ParameteredObject::_addParameter (missingValue, "missingValue", "Value for locations missing in the CImgList due to smaller image sizes.");

	ParameteredObject::_addInputSlot(in, "in", "The CImg object to be converted", "CImgList<T>");
	ParameteredObject::_addOutputSlot(out, "out", "A copy of the image stored as vigra::MultiArray<5, T>", "vigra::MultiArrayView<5, T>");
}

template <typename T>
void CImg2VigraMultiArray<T>::execute() {
	ParameteredObject::execute();

	sout << "CImg2VigraMultiArray<T>::execute()" << std::endl;
	if(!in().size())
		throw "CImg2VigraMultiArray: Specified in-image list is empty!";

	int dimx =0, dimy =0, dimz =0, dimv =0;
	unsigned int dimt =0;
	for(unsigned int i =0; i < in().size(); ++i)
	{
		if(in()[i].width() > dimx)
			dimx =in()[i].width();
		if(in()[i].height() > dimy)
			dimy =in()[i].height();
		if(in()[i].depth() > dimz)
			dimz =in()[i].depth();
		if(in()[i].spectrum() > dimv)
			dimv =in()[i].spectrum();
		if(in().size() > dimt)
			dimt =in().size();
	}
	result.reshape(typename vigra::MultiArray<5, T>::difference_type(dimx, dimy, dimz, dimv, dimt));
	result.init((T)missingValue);
	sout << "inited" << std::endl;
	for(unsigned int t =0; t < in().size(); ++t)
	{
		sout << "frame " << t << std::endl;
		cimg_forXYZC(in()[t], x, y, z, v)
		{
			result(x, y, z, v, t) =in()[t](x, y, z, v);
		}
	}

	//vigra::MultiArrayView<4, T> view(vigra::MultiArrayView<4, T>::difference_type(inCImg().width(), inCImg().height(), inCImg().depth(), inCImg().spectrum()), const_cast<T*>(inCImg().data()));
	out() =result.view();
	sout << "done" << std::endl;
}

#endif /* _CIMG2VIGRAMULTIARRAY_HXX_ */



