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
/** \file Overlay.hxx
 *  Implementation of the parameter class Overlay.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.12.2011
 */

#ifndef _OVERLAY_HXX_
#define _OVERLAY_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "Overlay.h"

template <typename T>
Overlay<T>::Overlay(const std::string& name) :
		TemplatedParameteredObject<T>(
			"Overlay", name,
			"<h2>draw one image above another</h2><br>"
			"One image is used as foreground, a second image as backround. "
			"Using the alpha parameter, the weight of each image may be "
			"adjusted."
		)
{
	ParameteredObject::_addInputSlot(
		foreground, "foreground",
		"foreground image",
		"CImgList<T>");
	ParameteredObject::_addInputSlot(
		background, "background",
		"background image",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"overlay output",
		"CImgList<T>");

	ParameteredObject::_addParameter< float >(
		alpha, "alpha",
		"foreground/background ratio, 0=back,1=front",
		0.5, "float");

}

template <typename T>
void Overlay<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const cimg_library::CImgList<T>& fg = foreground();
	const cimg_library::CImgList<T>& bg = background();
	cimg_library::CImgList<T>& o = out();
	const float f = alpha(), b = (1-f);

	if (!fg.is_sameNXYZC(bg)) {
		ParameteredObject::raise(
					"foreground and background image dimension mismatch");
	}
	if ((f<0)||(b<0)) {
		ParameteredObject::raise("alpha out of range [0,1]");
	}

	o.assign(bg);
	cimglist_for(o, kk) {
		cimg_forXYZC(o[kk],xx,yy,zz,cc) {
			o(kk,xx,yy,zz,cc) = f*fg(kk,xx,yy,zz,cc) + b*bg(kk,xx,yy,zz,cc);
		}
	}
}

#endif /* _OVERLAY_HXX_ */
