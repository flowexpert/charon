/*  Copyright (C) 2009 Cornelius Ratsch

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
/** \file Images2Sequence.hxx
 *  Implementation of the parameter class Images2Sequence.
 *  \author Cornelius Ratsch
 *  \date 04.03.2010
 */

#ifndef _IMAGES2SEQUENCE_HXX_
#define _IMAGES2SEQUENCE_HXX_

#include "Images2Sequence.h"

template<typename T>
Images2Sequence<T>::Images2Sequence(const std::string& name) :
	TemplatedParameteredObject<T> ("Images2Sequence", name,
			"Generates an image sequence out of multiple images"),
			images(false, true) // multislot
{
	ParameteredObject::_addInputSlot(images, "images",
			"One or more images", "CImgList<T>");
	ParameteredObject::_addOutputSlot(image_sequence, "image_sequence",
			"image sequence containing every input image", "CImgList<T>");
}

template<typename T>
void Images2Sequence<T>::execute() {
	ParameteredObject::execute();

	// Dimensions of the first input image
	int width = this->images()[0].width();
	int height = this->images()[0].height();
	int depth = this->images()[0].depth();
	int spectrum = this->images()[0].spectrum();

	// Initialize image sequence
	image_sequence().assign(0, width, height, depth, spectrum);

	// iterate over the images in multi input slot
	for (unsigned int i=0; i < images.size(); i++) {
		// Create reference to the CImgList in the current input slot
		const cimg_library::CImgList<T>& imgList = images[i];

		// check dimensions of the current image
		if (imgList[0].width() == width &&
				imgList[0].height()   == height &&
				imgList[0].depth()    == depth &&
				imgList[0].spectrum() == spectrum) {
			// append image(s) to the end of the image sequence
			for(unsigned int i = 0; i < imgList.size(); i++) {
				this->image_sequence().push_back(imgList[i]);
			}
		}
		else {
			sout << "Error: Image has wrong dimensions" << std::endl;
		}
	}
}

#endif /* _IMAGES2SEQUENCE_HXX_ */

