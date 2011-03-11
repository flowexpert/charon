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
/** \file Mirror.hxx
 *  Implementation of the parameter class Mirror.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 11.03.2011
 */

#ifndef _MIRROR_HXX_
#define _MIRROR_HXX_

#include "Mirror.h"
#include <charon-core/ParameteredObject.hxx>

template <typename T>
Mirror<T>::Mirror(const std::string& name) :
	TemplatedParameteredObject<T>("mirror", name,
		"mirror selected dimension(s)")
{
	ParameteredObject::_addInputSlot(
			in,"in","data input","vigraArray5<T>");
	ParameteredObject::_addOutputSlot(
			out,"out","data output","vigraArray5<T>");

	ParameteredObject::_addParameter(x, "x", "mirror x coordinate", false);
	ParameteredObject::_addParameter(y, "y", "mirror y coordinate", false);
	ParameteredObject::_addParameter(z, "z", "mirror z coordinate", false);
	ParameteredObject::_addParameter(t, "t", "mirror t coordinate", false);
	ParameteredObject::_addParameter(v, "v", "mirror v coordinate", false);
}

template <typename T>
void Mirror<T>::execute()
{
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const vigra::MultiArrayView<5,T>& i = in();
	vigra::MultiArray<5,T>& o = out();
	vigra::MultiArrayShape<5>::type s = i.shape();

	o.reshape(s);
	unsigned int cx, cy, cz, ct, cv;

	// quick and dirty hack, perhaps there are more
	// optimized and/or elegant variants
	for(unsigned int vv=0u; vv < s[4u]; vv++) {
		cv = (v() ? (s[4u]-vv-1) : vv);
		for(unsigned int tt=0u; tt < s[3u]; tt++) {
			ct = (t() ? (s[3u]-tt-1) : tt);
			for(unsigned int zz=0u; zz < s[2u]; zz++) {
				cz = (z() ? (s[2u]-zz-1) : zz);
				for(unsigned int yy=0u; yy < s[1u]; yy++) {
					cy = (y() ? (s[1u]-yy-1) : yy);
					for(unsigned int xx=0u; xx < s[0u]; xx++) {
						cx = (x() ? (s[0u]-xx-1) : xx);
						o(xx,yy,zz,tt,vv) = i(cx,cy,cz,ct,cv);
					}
				}
			}
		}
	}
}

#endif /* _MIRROR_HXX_ */
