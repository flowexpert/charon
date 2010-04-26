/*  This file is part of Charon.

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
/** @file InterpolatorCubic.hxx
 *  Implementation of template class InterpolatorCubic.
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  Based on a file written by Daniel Kondermann, modified by Jens-Malte
 *  Gottfried.
 *
 *  @date 10.09.2009
 */

#ifndef _Interpolator_Cubic_HXX_
#define _Interpolator_Cubic_HXX_

#include "Interpolator.hxx"
#include "InterpolatorCubic.h"

template <typename T>
InterpolatorCubic<T>::InterpolatorCubic(const std::string& name) :
		Interpolator<T>("interpolatorcubic", name, "cubic interpolator") {
}

template <typename T>
T InterpolatorCubic<T>::interpolate(
		const cimg_library::CImg<T>& src,
		float fx, float fy, int z, int v) const {
	return static_cast<T>(src.cubic_atXY(fx, fy, z, v));
}

template <typename T>
T InterpolatorCubic<T>::interpolate(
		const cimg_library::CImg<T>& src,
		float fx, float fy, float fz, int v) const {
	// this kind of rounding only works for positive values!
	assert(fz > 0);
	return static_cast<T>(src.cubic_atXY(fx, fy, static_cast<int>(fz+0.5), v));
}

#endif /*_Interpolator_Cubic_HXX_*/
