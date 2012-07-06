/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file PyramidRescale.hxx
 *  Implementation of the parameter class PyramidRescale.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 11.02.2011
 */

#ifndef _PYRAMID_RESCALE_HXX_
#define _PYRAMID_RESCALE_HXX_

#include "PyramidRescale.h"
#include <charon-utils/Convolution.h>
#include <iomanip>

template <typename T>
PyramidRescale<T>::PyramidRescale(const std::string& name) :
		TemplatedParameteredObject<T>("PyramidRescale", name,
			"Rescaling for pyramid-based flow-estimation algorithms.")
{
	ParameteredObject::_addInputSlot(
			seqIn, "seqIn", "sequence input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			flowIn, "flowIn", "flow input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			level, "level", "level select (from small to larger scales)");
	ParameteredObject::_addOutputSlot(
			seqOut, "seqOut", "sequence output", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			flowOut, "flowOut", "flow output", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			size, "size", "current size", "Roi<int>*");
	ParameteredObject::_addParameter (
			scaleFactor, "scaleFactor", "scale factor", 0.5);
	ParameteredObject::_addParameter (
			levels, "levels", "scale levels", 5u);
	ParameteredObject::_addParameter (
			sigma, "sigma", "sigma used to blur before downsampling", 0.7);
	ParameteredObject::_addParameter (
			interpolation, "interpolation",
			"interpolation type (see CImg::resize() documentation)", 3);
}

template <typename T>
void PyramidRescale<T>::execute() {
	size() = &_size;

	const cimg_library::CImgList<T>& si = seqIn();
	cimg_library::CImgList<T>& so = seqOut();

	const cimg_library::CImgList<T>& fi = flowIn();
	cimg_library::CImgList<T>& fo = flowOut();

	const unsigned int curL = level();
	const unsigned int endL = levels();
	const unsigned int maxL = endL-1;
	const unsigned int stepsDown = maxL-curL;

	if(curL > maxL) {
		std::ostringstream msg;
		msg << "current level too large: cur=" << curL << "; max=" << maxL;
		ParameteredObject::raise(msg.str());
	}

	// input sequence remains unscaled over time and is the pyramid base
	const int sx = si[0].width(), sy = si[0].height();

	// target sizes
	const double shrink = std::pow(scaleFactor(),(double)stepsDown);
	const int tx = _size.xEnd = sx * shrink;
	const int ty = _size.yEnd = sy * shrink;

	// rescale sequence
	so = si;
	if(stepsDown > 0) {
		//can be float as CImg::deriche only takes float as input
		const float blur = float(sigma * stepsDown);
		cimglist_for(so,kk) {
			so.at(kk).deriche(blur,0,'x');
			so.at(kk).deriche(blur,0,'y');
			so.at(kk).resize(tx,ty,-100,-100,interpolation());
		}
	}

	// rescale flow
	fo = fi;
	if(fo.is_sameXY(si)) {
		// scale down (initial guess)
#ifndef NDEBUG
		sout << "\t" << "scaling down to " << tx << "x" << ty << std::endl;
#endif
		const float blur = sigma * stepsDown;
		cimglist_for(fo,kk) {
			assert(fo.at(kk).is_sameXY(sx,sy));
			fo.at(kk).deriche(blur,0,'x');
			fo.at(kk).deriche(blur,0,'y');
			fo.at(kk).resize(tx,ty,-100,-100,interpolation());
			fo.at(kk) *= shrink;
		}
	}
	else {
		// scale up last result
#ifndef NDEBUG
		sout << "\t" << "scaling up to "
				<< tx << "x" << ty << ": got "
				<< fo[0].width() << "x" << fo[0].height()
				<< " expected: "
				<< std::fixed << std::setprecision(0)
				<< tx*scaleFactor() << "x" << ty*scaleFactor()
				<< std::endl;
#endif
		cimglist_for(fo,kk) {
			fo.at(kk).resize(tx,ty,-100,-100,interpolation());
			fo.at(kk) /= scaleFactor();
		}
	}
}
#endif /* _PYRAMID_RESCALE_HXX_ */

