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
			sigmas, "sigmas", "list of std. deviations in decreasing order", "T_list");

	ParameteredObject::_setTags("charon-flow;MultiscaleApproaches;CImg");
}

template <typename T>
void PyramidLowpass<T>::execute() {
	const cimg_library::CImgList<T>& si = seqIn();
	cimg_library::CImgList<T>& so = seqOut();
	so = si;

	float blurFactor1 = 0.0;
	float blurFactor2 = 0.0;

	if (level() == 0) {
		blurFactor2 = sigmas()[level()];
		cimglist_for(so,kk)
		{
			so.at(kk).blur(blurFactor2);
		}
	} else {
		cimg_library::CImgList<T> tmp1 = si;
		cimg_library::CImgList<T> tmp2 = si;
		blurFactor1 = sigmas()[level()-1];
		cimglist_for(tmp1,kk)
		{
			tmp1.at(kk).blur(blurFactor1);
		}
		blurFactor2 = sigmas()[level()];
		cimglist_for(tmp2,kk)
		{
			tmp2.at(kk).blur(blurFactor2);
		}
		cimglist_for(so,kk)
		{
			so.at(kk) = tmp2.at(kk) - tmp1.at(kk);
		}
	}
}
#endif /* _PYRAMID_LOWPASS_HXX_ */

