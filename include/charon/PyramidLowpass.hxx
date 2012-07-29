/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file PyramidLowpass.hxx
 *  Implementation of the parameter class PyramidLowpass.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 26.07.2012
 */

#ifndef _PYRAMID_LOWPASS_HXX_
#define _PYRAMID_LOWPASS_HXX_

#include "PyramidLowpass.h"
#include <charon-utils/Convolution.h>
#include <iomanip>

template <typename T>
PyramidLowpass<T>::PyramidLowpass(const std::string& name) :
		TemplatedParameteredObject<T>("PyramidLowpass", name,
			"Lowpass filtering for pyramid-based flow-estimation algorithms.")
{
	ParameteredObject::_addInputSlot(
			seqIn, "seqIn", "sequence input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			level, "level", "level select");
	ParameteredObject::_addOutputSlot(
			seqOut, "seqOut", "sequence output", "CImgList<T>");
	ParameteredObject::_addParameter (
			levels, "levels", "blur levels", 5u);
}

template <typename T>
void PyramidLowpass<T>::execute() {
	const cimg_library::CImgList<T>& si = seqIn();
	cimg_library::CImgList<T>& so = seqOut();

	const unsigned int curL = level();
	const unsigned int endL = levels();
	const unsigned int maxL = endL-1;
	const unsigned int stepsDown = maxL-curL;

	if(curL > maxL) {
		std::ostringstream msg;
		msg << "current level too large: cur=" << curL << "; max=" << maxL;
		ParameteredObject::raise(msg.str());
	}

	// blur sequence
	so = si;
	//can be float as CImg::deriche only takes float as input
	const float blurFactor = 1.0;
	const float blur = float(blurFactor*stepsDown);
	cimglist_for(so,kk) {
		so.at(kk).deriche(blur,0,'x');
		so.at(kk).deriche(blur,0,'y');
	}
}
#endif /* _PYRAMID_LOWPASS_HXX_ */

