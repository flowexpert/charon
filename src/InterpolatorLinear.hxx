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
/** @file Interpolator.hxx
 *  Implementation of template class Interpolator and related classes.
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

#ifndef _Interpolator_Linear_HXX_
#define _Interpolator_Linear_HXX_

#include "Interpolator.hxx"
#include "InterpolatorLinear.h"

template <typename T>
InterpolatorLinear<T>::InterpolatorLinear() :
         Interpolator<T>("interpolatorlinear", "", "linear interpolator") {
}

template <typename T>
InterpolatorLinear<T>::InterpolatorLinear(const std::string& name) :
         Interpolator<T>("interpolatorlinear", name, "linear interpolator") {
}

template <typename T>
InterpolatorLinear<T>::~InterpolatorLinear() {
}

template <typename T>
T InterpolatorLinear<T>::interpolate(const cimg_library::CImg<T>& src,
                                    float fx, float fy, int z, int v) const {
    return static_cast<T>(src.linear_atXY(fx, fy, z, v));
}

template <typename T>
T InterpolatorLinear<T>::interpolate(const cimg_library::CImg<T>& src,
                                 float fx, float fy, float fz, int v) const {
	return static_cast<T>(src.linear_atXYZ(fx, fy, fz, v));
}

#endif /* _Interpolator_Linear_HXX_ */
