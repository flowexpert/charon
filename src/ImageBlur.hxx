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
			"apply a blur effect on an image") {
	this->_addInputSlot(roi, "roi", "region to blur", "Roi<int>");
	this->_addInputSlot(in, "in", "image input", "CImgList<T>");
	this->_addOutputSlot(out, "out", "image output", "CImgList<T>");
	this->_addParameter(strength, "strength", "blur strength");
}

template<typename T>
void ImageBlur<T>::execute() {
	ParameteredObject::execute();

	// check roi ranges
	assert(roi());
	assert(roi()->xBegin() < roi()->xEnd());
	assert(roi()->yBegin() < roi()->yEnd());
	assert(roi()->zBegin() < roi()->zEnd());
	assert(roi()->tBegin() < roi()->tEnd());
	assert(roi()->vBegin() < roi()->vEnd());

	out() = in();
	cimg_library::CImgList<T> region = in().get_images(
			roi()->vBegin(), roi()->vEnd()-1);
	cimglist_for(region, i) {
		region[i].crop(
			roi()->xBegin(), roi()->yBegin(), roi()->zBegin(), roi()->tBegin(),
			roi()->xEnd()-1, roi()->yEnd()-1, roi()->zEnd()-1, roi()->tEnd()-1);
		region[i].blur(strength());
		out()[roi()->vBegin()+i].draw_image(
				roi()->xBegin(), roi()->yBegin(),
				roi()->zBegin(),roi()->tBegin(),
				region[i]
		);
	}

}
#endif // _IMAGEBLUR_HXX_
