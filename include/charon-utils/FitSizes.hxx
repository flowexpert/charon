/*  Copyright (C) 2011 Daniel Kondermann

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
/** \file FitSizes.hxx
 *  Implementation of the parameter class FitSizes.
 *  \author <a href="mailto:daniel@kondermann.de">
 *      Daniel Kondermann</a>
 *  \date 05.08.2011
 */

#ifndef _FITSIZES_HXX_
#define _FITSIZES_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "FitSizes.h"
#include <exception>

using namespace std;

template <typename T>
FitSizes<T>::FitSizes(const std::string& name) :
		TemplatedParameteredObject<T>(
			"FitSizes", name,
			"<h2>Takes two images and scales the second image to fit the "
			"size of the first</h2><br>"
			"Uses CImg internal interpolation methods."
		)
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(
		target, "target",
		"This image remains untouched. Only its size (x,y,z) is used as"
		"target size.",
		"CImgList<T>");
	ParameteredObject::_addInputSlot(
		toScale, "toScale",
		"This image will be scaled to fit the target size.",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"The resized image.",
		"CImgList<T>");

	ParameteredObject::_addParameter< std::string >(
		interpolationMethod, "interpolationMethod",
		"The way the image is interpolated.",
		"lanczos", "{nearestNeighbor;bilinear;bicubic;lanczos}");
	ParameteredObject::_addParameter< bool >(
		crop, "crop",
		"The image is scaled so that one side becomes too large. This part is"
		"cropped.",
		0, "bool");

}

template <typename T>
void FitSizes<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	if(!toScale().size())
		ParameteredObject::raise("toScale image list is empty!");
	if(!target().size())
		ParameteredObject::raise("target image list is empty!");
	else if(toScale().size() > 1)
		sout << "WARNING: only the first image in toScale will be scaled. (There are " << toScale().size() << " images in the list...)" << endl;
	else if(target().size() > 1)
		sout << "WARNING: only the size of the first image in target will be considered. (There are " << target().size() << " images in the list...)" << endl;

	//fit toScale into target
	//does horizontal fit also fit vertically?
	float hScale =(float)target()[0].width()/toScale()[0].width();
	sout << "hScale: " << hScale << endl;
	float vScale =0;
	if(toScale()[0].height()*hScale > target()[0].height())
		vScale =(float)target()[0].height()/toScale()[0].height();
	sout << "vScale: " << vScale << endl;
	if(toScale()[0].width()*vScale > target()[0].width())
	{
		sout << "toScale()[0].height()*hScale: " << toScale()[0].height()*hScale << endl;
		sout << "target()[0].height(): " << target()[0].height() << endl;

		sout << "toScale()[0].width()*vScale: " << toScale()[0].width()*vScale << endl;
		sout << "target()[0].width(): " << target()[0].width() << endl;
		throw runtime_error("scaling problem");
	}
	if(vScale && !crop())
		hScale =vScale;
	else if(!vScale && crop()) //in case of crop, switch scale
		hScale =(float)target()[0].height()/toScale()[0].height(); //vScale from above
	else if(vScale && crop())
		hScale =(float)target()[0].width()/toScale()[0].width(); //original hScale
	out().assign(1);
	out()[0] =toScale()[0];
	int method =1;
	if(interpolationMethod() == "nearestNeighbor")
		method =1;
	else if(interpolationMethod() == "bilinear")
		method =3;
	else if(interpolationMethod() == "bicubic")
		method =5;
	else if(interpolationMethod() == "lanczos")
		method =6;
	out()[0].resize(hScale*toScale()[0].width(), hScale*toScale()[0].height(), -100, -100, method);
	out()[0].crop(0, 0, target()[0].width()-1, target()[0].height()-1);
}

#endif /* _FITSIZES_HXX_ */
