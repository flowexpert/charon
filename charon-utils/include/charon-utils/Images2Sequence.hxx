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
#include <algorithm>

template<typename T>
Images2Sequence<T>::Images2Sequence(const std::string& name) :
	TemplatedParameteredObject<T> ("Images2Sequence", name,
			"Generates an image sequence out of multiple images"),
			images(false, true) // multislot
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(images, "images",
			"One or more images", "CImgList<T>");
	ParameteredObject::_addOutputSlot(image_sequence, "image_sequence",
			"image sequence containing every input image", "CImgList<T>");
	ParameteredObject::_addParameter<std::string>(appendDimension,"appendDimension",
			"select dimension for appending","list","{z;t;list}");
}

template<typename T>
void Images2Sequence<T>::execute() {
	// check number of inputs
	if (images.size() <= 1) {
		sout << "(WW) need more than one connected input "
			<< "to concatenate them, fallback to passthrough" << std::endl;
		image_sequence().assign(images(),true);
		return;
	}
	// Dimensions of the first non-emtpy input image
	cil::CImgList<T> temp;
	temp.assign(images(),true);
	for (unsigned int ii=0; ii < images.size(); ii++) {
		// use first non-emtpy input
		if (temp.size()) {
			if (ii > 0) {
				sout << "(II) skipped " << ii << " empty inputs" << std::endl;
			}
			break;
		}
		temp.assign(images[ii],true);
	}
	if (!temp.size()) {
		sout << "(WW) ALL inputs emtpy" << std::endl;
		image_sequence().assign();
		return;
	}
	unsigned int listSize = temp.size();
	int width    = temp[0].width();
	int height   = temp[0].height();
	int depth    = temp[0].depth();
	int spectrum = temp[0].spectrum();
	temp.assign(); // clear

	// assert that images are orderd in lexicographically ascending
	// order of their instance name
	std::vector<std::size_t> order;
	{
		const std::set<Slot*>& targets = images.getTargets();
		std::size_t num = targets.size();
		std::vector <std::pair<std::string, unsigned int> > tNum;
		tNum.resize(num);
		std::set<Slot*>::const_iterator tIter;
		std::size_t i=0;
		for(tIter = targets.begin();tIter != targets.end(); tIter++) {
			tNum[i] = std::make_pair((*tIter)->getParent().getName(), i);
			i++;
		}
		std::sort(tNum.begin(),tNum.end());
		order.resize(num);
		for (i=0; i < num; i++)
			order[i] = tNum[i].second;
	}

	// Initialize image sequence
	if (appendDimension() == "list") {
		image_sequence().assign(0, width, height, depth, spectrum);
	}
	else if (appendDimension() == "t") {
		image_sequence().assign(listSize, width, height, depth, 0);
	}
	else if (appendDimension() == "z") {
		image_sequence().assign(listSize, width, height, 0, spectrum);
	}

	// iterate over the images in multi input slot
	for (std::size_t i=0; i < images.size(); i++) {
		// Create reference to the CImgList in the current input slot
		const cimg_library::CImgList<T>& imgList = images[order[i]];

		// skip empty images
		if (!imgList.size()) {
			continue;
		}

		// check dimensions of the current image
		if (
			imgList[0].width()    != width ||
			imgList[0].height()   != height)
		{
			ParameteredObject::raise("image XY dimension mismatch");
		}
		// (dimension mismatch allowed for appending dimension)
		if (
			(appendDimension() == "list" || imgList.size() == listSize) &&
			(appendDimension() == "z" || imgList[0].depth() == depth) &&
			(appendDimension() == "t" || imgList[0].spectrum() == spectrum))
		{
			// append image(s) to the end of the image sequence
			if (appendDimension() == "list") {
				cimglist_for(imgList, i) {
					image_sequence().push_back(imgList[i]);
				}
			}
			else if (appendDimension() == "t") {
				cimglist_for(imgList, i) {
					image_sequence()[i].append(imgList[i],'c');
				}
			}
			else if (appendDimension() == "z") {
				cimglist_for(imgList, i) {
					image_sequence()[i].append(imgList[i],'z');
				}
			}
		}
		else {
			ParameteredObject::raise("Image has wrong dimensions");
		}
	}
}

#endif /* _IMAGES2SEQUENCE_HXX_ */

