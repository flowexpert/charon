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
/** @file InterpolatorDiscrete.hxx
 *  Implementation of template class InterpolatorDiscrete.
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

#ifndef _Interpolator_Discrete_HXX_
#define _Interpolator_Discrete_HXX_

#include "Interpolator.hxx"
#include "InterpolatorDiscrete.h"

template <typename T>
InterpolatorDiscrete<T>::InterpolatorDiscrete(const std::string& name) :
		Interpolator<T>("interpolatordiscrete", name, "discrete interpolator") {
	ParameteredObject::_setTags("charon-utils;CImg") ;

}

template <typename T>
T InterpolatorDiscrete<T>::interpolate(
		const cimg_library::CImg<T>& src,
		float fx, float fy, float fz, int v) const {
	return static_cast<T>(src.atXYZ(int(fx), int(fy), int(fz), int(v)));
}

#endif /*_Interpolator_Discrete_HXX_*/
