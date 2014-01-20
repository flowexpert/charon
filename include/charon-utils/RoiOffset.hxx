/*  Copyright (C) 2014 Jens-Malte Gottfried

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
/** \file RoiOffset.hxx
 *  Implementation of the parameter class RoiOffset.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.01.2014
 */

#ifndef _ROIOFFSET_HXX_
#define _ROIOFFSET_HXX_

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/Roi.hxx>
#include "RoiOffset.h"

template <typename T>
RoiOffset<T>::RoiOffset(const std::string& name) :
		TemplatedParameteredObject<T>(
			"RoiOffset", name,
			"<h2>Adds given values as offset to the ROI borders</h2><br>"
			"The borders of the passed ROI are incremented by the connected "
			"values.<br>"
			"There are five slots corresponding the the five ROI "
			"dimensions.<br>"
			"Unconnected slots are treated as zero, leaving this dimension "
			"unchanged.<br>"
			"The offset is applied as well to the lower and the upper ROI "
			"border."
		),
		dx(true,false),
		dy(true,false),
		dz(true,false),
		dt(true,false),
		dv(true,false),
		out(0)
{
	ParameteredObject::_setTags("charon-utils") ;

	ParameteredObject::_addInputSlot(
		in, "in",
		"ROI input",
		"Roi<T>*");
	ParameteredObject::_addInputSlot(
		dx, "dx",
		"x offset",
		"T");
	ParameteredObject::_addInputSlot(
		dy, "dy",
		"y offset",
		"T");
	ParameteredObject::_addInputSlot(
		dz, "dz",
		"z offset",
		"T");
	ParameteredObject::_addInputSlot(
		dt, "dt",
		"t offset",
		"T");
	ParameteredObject::_addInputSlot(
		dv, "dv",
		"v offset",
		"T");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"ROI output",
		"Roi<T>*");
}

template <typename T>
void RoiOffset<T>::execute() {
	_roi = *(in()); // copy values from input
	if (dx.connected()) {
		const T& d = dx();
		_roi.xBegin() += d;
		_roi.xEnd() += d;
	}
	if (dy.connected()) {
		const T& d = dy();
		_roi.yBegin() += d;
		_roi.yEnd() += d;
	}
	if (dz.connected()) {
		const T& d = dz();
		_roi.zBegin() += d;
		_roi.zEnd() += d;
	}
	if (dt.connected()) {
		const T& d = dt();
		_roi.tBegin() += d;
		_roi.tEnd() += d;
	}
	if (dv.connected()) {
		const T& d = dv();
		_roi.vBegin() += d;
		_roi.vEnd() += d;
	}
	out() = &_roi;
}

#endif /* _ROIOFFSET_HXX_ */
