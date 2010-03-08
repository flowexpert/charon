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
/** \file SequenceGenerator.hxx
 *  Implementation of the parameter class SequenceGenerator.
 *  \author Cornelius Ratsch
 *  \date 04.03.2010
 */

#ifndef _SEQUENCEGENERATOR_HXX_
#define _SEQUENCEGENERATOR_HXX_

#include "SequenceGenerator.h"

template<typename T>
SequenceGenerator<T>::SequenceGenerator(const std::string& name) :
	TemplatedParameteredObject<T> ("SequenceGenerator", name,
			"Generates an image sequence out of multiple images"), images(
			false, true) {

	ParameteredObject::_addInputSlot(images, "images", "One or more images",
			"CImgList<T>");
	ParameteredObject::_addOutputSlot(image_sequence, "image_sequence",
			"image sequence containing every input image", "CImgList<T>");

}

template<typename T>
void SequenceGenerator<T>::execute() {
	ParameteredObject::execute();

	// Dimensions of the first input image
	int width = this->images()[0].width();
	int height = this->images()[0].height();
	int depth = this->images()[0].depth();
	int spectrum = this->images()[0].spectrum();

	// Initialize image sequence
	image_sequence().assign(0, width, height,
			depth, spectrum);

	// iterate over the input slots
	for (typename std::set<AbstractSlot<cimg_library::CImgList<T> > *>::const_iterator
			it = this->images.begin(); it != this->images.end(); it++) {
		// Create reference to the CImgList in the current input slot
		const cimg_library::CImgList<T> &imgList = (*((InputSlot<
				cimg_library::CImgList<T> > *)(*it)))();

		// check dimensions of the current image
		if (imgList[0].width() == width && imgList[0].height() == height
				&& imgList[0].depth() == depth && imgList[0].spectrum()
				== spectrum) {
			// append image(s) to the end of the image sequence
			for(unsigned int i = 0; i < imgList.size(); i++) {
				this->image_sequence().push_back(imgList[i]);
			}
		} else {
			sout << "Error: Image has wrong dimensions" << std::endl;
		}
	}
}

#endif /* _SEQUENCEGENERATOR_HXX_ */

