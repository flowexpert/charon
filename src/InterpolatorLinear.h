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
/** @file InterpolatorLinear.h
 *  Declaration of template class InterpolatorLinear.
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  Based on a file written by Daniel Kondermann, modified by Jens-Malte
 *  Gottfried.
 *
 *  This File contains the declaration of a linear interpolator class.
 *
 *  @date 10.09.2002
 */
#ifndef _Interpolator_Linear_H_
#define _Interpolator_Linear_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#if defined interpolatorlinear_EXPORTS
///Visual C++ specific code
#define interpolatorlinear_DECLDIR __declspec(dllexport)
#else
#define interpolatorlinear_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define interpolatorlinear_DECLDIR
#endif

#include "Interpolator.h"

/// Linear interpolation scheme.
template <typename T>
class interpolatorlinear_DECLDIR InterpolatorLinear : public Interpolator<T>
{
public:
    /// Default constructor.
	InterpolatorLinear();

    /// Init interpolator.
    /// @param name             instance name
    InterpolatorLinear(const std::string& name);

    virtual ~InterpolatorLinear();

    T interpolate(const cimg_library::CImg<T>& src,
        float fx, float fy, int z = 0, int v = 0) const;

    T interpolate(const cimg_library::CImg<T>& src,
        float fx, float fy, float fz, int v = 0) const;
};

#endif /* _Interpolator_Linear_H_ */
