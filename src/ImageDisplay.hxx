/*
	Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file ImageDisplay.hxx
/// Implementation of the parameter class ImageDisplay.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009

#ifndef _IMAGEDISPLAY_HXX_
#define _IMAGEDISPLAY_HXX_

#include <stdexcept>
#include "ImageDisplay.h"
#include "ImgTool.hxx"

template <typename T>
ImageDisplay<T>::ImageDisplay(const std::string& name) :
		TemplatedParameteredObject<T>("imagedisplay", name,
			"read image from image file using cimg"),
		title("Display")
{
	this->_addParameter(slice,   "slice",
		"select slice (dimz) to display", 0u);
	this->_addParameter(channel, "channel",
		"select channel to display (-1=all)", -1);
	this->_addParameter(frame,   "frame",
		"select frame to display", 0u);
	this->_addParameter(wait,    "wait",
		"display wait time (in milliseconds)", 500u);
	this->_addParameter(width,   "width",
		"display width (0=auto)", 0u);
	this->_addParameter(height,  "height",
		"display height (0=auto)", 0u);
	this->_addParameter(title,   "title",
		"display title");
	this->_addInputSlot(in,      "in",
		"image input", "CImgList<T>");
}

template <typename T>
void ImageDisplay<T>::execute() {
	ParameteredObject::execute();

	std::ostringstream msg;
	msg	<< __FILE__ << ":" << __LINE__;
	msg << "\nImageDisplay:";
	bool error = false;

	// check preconditions
	if(in().size() == 0) {
		msg << "\n\tImage Sequence is empty!";
		throw std::runtime_error(msg.str().c_str());
	}
	if(int(slice()) >= in()[0].depth()) {
		msg << "\n\t"
			<< "Selected slice is out of range!\n\t"
			<< "number of slices: " << in()[0].depth() << "\n\t"
			<< "selected slice: " << slice();
		error = true;
	}
	if(channel() >= in()[0].spectrum()) {
		msg << "\n\t"
			<< "Selected channel is out of range!\n\t"
			<< "number of channels: " << in()[0].spectrum() << "\n\t"
			<< "selected channel: " << slice();
		error = true;
	}
	if(frame() >= in().size()) {
		std::ostringstream msg;
		msg << "\n\t"
			<< "Selected frame out of range!\n\t"
			<< "number of images: " << in().size() << "\n\t"
			<< "selected frame: " << frame();
		error = true;
	}
	if (error)
		throw std::out_of_range(msg.str().c_str());

	sout << "\tDisplaying image \"" << title() << "\":" << std::endl;
	ImgTool::printInfo(sout, in()[frame()], "\t\t");

	// select range to show
	cimg_library::CImg<T> toShow(in()[frame()]);
	if (channel() >= 0)
		toShow.crop(
				0,0,slice(),channel(),
				toShow.width()-1,toShow.height()-1,slice(),channel());
	else
		// display all channels (color image)
		toShow.crop(
				0,0,slice(),0,
				toShow.width()-1,toShow.height()-1,slice(),toShow.spectrum()-1);

	// set desired image size
	cimg_library::CImgDisplay* disp = new cimg_library::CImgDisplay(
			width(), height(), title().c_str());
	assert(disp);

	// show image
	toShow.display(*disp);
	disp->show();

	// and wait, if necessary
	if(wait != 0) {
		disp->wait(wait);
	}
	else
		while(disp->is_keyESC() || disp->is_keyQ() || disp->is_closed())
			disp->wait();

	delete disp;
}

#endif /* _IMAGEDISPLAY_HXX_ */
