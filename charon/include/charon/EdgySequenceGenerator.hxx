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
/** \file EdgySequenceGenerator.hxx
 *  Implementation of the parameter class EdgySequenceGenerator.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 23.04.2010
 */

#ifndef _EDGYSEQUENCEGENERATOR_HXX_
#define _EDGYSEQUENCEGENERATOR_HXX_

#include "EdgySequenceGenerator.h"
#include <charon-utils/Roi.hxx>
#include <charon-utils/ImgTool.hxx>

template <typename T>
EdgySequenceGenerator<T>::EdgySequenceGenerator(const std::string& name) :
		TemplatedParameteredObject<T>("EdgySequenceGenerator", name,
			"Generate test sequences with motion edges<br><br>"
			"using a random pattern and some given mask representing "
			"a foreground and a background image. "
			 "Velocities for foreground and background may be specified.")
		,foregroundVelocity(" 0.5; 0.9")
		,backgroundVelocity("-0.3;-0.7")
		,size(0)
{
	ParameteredObject::_addInputSlot(
			mask, "mask", "mask input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			interpolator, "interpolator",
			"interpolator input", "Interpolator<T>*");
	ParameteredObject::_addOutputSlot (
			sequence, "sequence",
			"generated test sequence", "CImgList<T>");
	ParameteredObject::_addOutputSlot (
			flow, "flow",
			"flow of generated test sequence", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			size, "size",
			"Sequence size", "Roi<int>*");
	ParameteredObject::_addParameter (
			foregroundVelocity, "foregroundVelocity",
			"Velocity of the foreground pattern");
	ParameteredObject::_addParameter (
			backgroundVelocity, "backgroundVelocity",
			"Velocity of the foreground pattern");
	ParameteredObject::_addParameter(
			noiseScale, "noiseScale",
			"scale parameter for pyramid levels (factor=2^{i*s})", 0.5f);
	ParameteredObject::_addParameter(
			noiseLevels, "noiseLevels",
			"number of pyramid levels", 5);
}

template <typename T>
void EdgySequenceGenerator<T>::execute() {
	size() = &_size;
	assert(foregroundVelocity.size() == 2);
	assert(backgroundVelocity.size() == 2);

	sequence().clear();
	flow().clear();

	// number of border pixels
	unsigned int border = (unsigned int)(cimg_library::CImg<double>::vector(
			std::abs(foregroundVelocity[0]),
			std::abs(foregroundVelocity[1]),
			std::abs(backgroundVelocity[0]),
			std::abs(backgroundVelocity[1])).max()+1.);

	assert(mask().size() == 1);
	cimg_library::CImg<T> maskF = mask()[0];
	maskF.threshold(T(0.5));
	cimg_library::CImg<T> maskB = maskF;
	maskB.fill(1) -= maskF;

	assert(maskF.is_sameXYZC(maskB));
	assert(maskF .max() <= 1);
	assert(maskB.max() <= 1);

	assert((maskF+maskB).min() == 1);
	assert((maskF+maskB).max() == 1);

	const int width   = maskF.width();
	const int height  = maskF.height();
	const int widthB  = width  + 2 * border;
	const int heightB = height + 2 * border;

	size()->assign(0,width,0,height);

	cimg_library::CImg<T> foreground(widthB, heightB);
	cimg_library::CImg<T> background(widthB, heightB);

	srand((unsigned int)time(0));
	ImgTool::createAllScaleRandomPattern(
			foreground, widthB, heightB, noiseScale(), noiseLevels());
	ImgTool::createAllScaleRandomPattern(
			background, widthB, heightB, noiseScale(), noiseLevels());
	//(foreground, background).display("randomPatterns");

	// generate flow fields
	cimg_library::CImgList<T> flowForeground;
	flowForeground.push_back(
			cimg_library::CImg<T>(
					widthB,heightB,1,1,T(foregroundVelocity[0])));
	flowForeground.push_back(
			cimg_library::CImg<T>(
					widthB,heightB,1,1,T(foregroundVelocity[1])));

	cimg_library::CImgList<T> flowBackground;
	flowBackground.push_back(
			cimg_library::CImg<T>(
					widthB,heightB,1,1,T(backgroundVelocity[0])));
	flowBackground.push_back(
			cimg_library::CImg<T>(
					widthB,heightB,1,1,T(backgroundVelocity[1])));

	cimg_library::CImg<T> foreground1(widthB, heightB);
	cimg_library::CImg<T> background1(widthB, heightB);
	cimg_library::CImg<T> foreground2(widthB, heightB);
	cimg_library::CImg<T> background2(widthB, heightB);

	Interpolator<T>* inter = interpolator();
	ImgTool::warp2D(foreground, flowForeground, foreground1, inter, +0.5f);
	ImgTool::warp2D(background, flowBackground, background1, inter, +0.5f);
	ImgTool::warp2D(foreground, flowForeground, foreground2, inter, -0.5f);
	ImgTool::warp2D(background, flowBackground, background2, inter, -0.5f);
	//(foreground1,foreground2,background1,background2).display("warped");

	// crop all to desired size
	foreground1.crop(border, border, width+border-1, height+border-1);
	foreground2.crop(border, border, width+border-1, height+border-1);
	background1.crop(border, border, width+border-1, height+border-1);
	background2.crop(border, border, width+border-1, height+border-1);
	flowForeground[0].crop(0,0,width-1,height-1);
	flowForeground[1].crop(0,0,width-1,height-1);
	flowBackground[0].crop(0,0,width-1,height-1);
	flowBackground[1].crop(0,0,width-1,height-1);
	//(foreground1,foreground2,background1,background2).display("cropped");

	assert(flowForeground[0].width()  == width);
	assert(flowForeground[1].width()  == width);
	assert(flowForeground[0].height() == height);
	assert(flowForeground[1].height() == height);

	// multiply with masks
	assert(foreground1.is_sameXYZC(maskF));
	assert(foreground2.is_sameXYZC(maskF));
	assert(background1.is_sameXYZC(maskF));
	assert(background2.is_sameXYZC(maskF));
	foreground1.mul(maskF);      background1.mul(maskB);
	foreground2.mul(maskF);      background2.mul(maskB);
	flowForeground[0].mul(maskF);  flowBackground[0].mul(maskB);
	flowForeground[1].mul(maskF);  flowBackground[1].mul(maskB);
	//(foreground1,foreground2,background1,background2).display("multiplied");

	// fill in background
	foreground1       += background1;
	foreground2       += background2;
	flowForeground[0] += flowBackground[0];
	flowForeground[1] += flowBackground[1];
	//(foreground1,foreground2).display("added");

	foreground1.append(foreground2, 'c');

	sequence().assign(foreground1);
	flow().assign(flowForeground);
}

#endif /* _EDGYSEQUENCEGENERATOR_HXX_ */



