/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file ImageBlur.hxx
/// Implementation of the parameter class ImageBlur.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009

#ifndef _IMAGEBLUR_HXX_
#define _IMAGEBLUR_HXX_

#include "ImageBlur.h"

template<typename T>
ImageBlur<T>::ImageBlur(const std::string& name) :
	TemplatedParameteredObject<T> ("imageblur", name,
			"apply a blur effect on an image"),
	roi(true,false) // optional
{
	this->_addInputSlot(roi, "roi", "region to blur", "Roi<int>");
	this->_addInputSlot(in, "in", "image input", "CImgList<T>");
	this->_addOutputSlot(out, "out", "image output", "CImgList<T>");
	this->_addParameter(strength, "strength", "blur strength");
}

template<typename T>
void ImageBlur<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const cimg_library::CImgList<T>& i = in();
	cimg_library::CImgList<T>& o = out();
	const float& s = strength();

	o = i;

	if(roi.connected()) {
		// check roi ranges
		assert(roi());
		const Roi<int>& r = *roi();
		assert(r.xBegin() < r.xEnd());
		assert(r.yBegin() < r.yEnd());
		assert(r.zBegin() < r.zEnd());
		assert(r.tBegin() < r.tEnd());
		assert(r.vBegin() < r.vEnd());

		cimg_library::CImgList<T> region = i.get_images(
				r.vBegin(), r.vEnd()-1);
		cimglist_for(region, kk) {
			region[kk].crop(
				r.xBegin(), r.yBegin(), r.zBegin(), r.tBegin(),
				r.xEnd()-1, r.yEnd()-1, r.zEnd()-1, r.tEnd()-1);
			region[kk].blur(s);
			o[r.vBegin()+kk].draw_image(
					r.xBegin(), r.yBegin(), r.zBegin(), r.tBegin(),
					region[kk]
			);
		}
	}
	else {
		// blur whole image(list)
		cimglist_for(o,kk){
			o[kk].blur(s);
		}
	}
}
#endif // _IMAGEBLUR_HXX_
