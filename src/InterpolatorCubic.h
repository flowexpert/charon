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
/** @file Interpolator.h
 *  Declaration of template class Interpolator and related classes.
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  Based on a file written by Daniel Kondermann, modified by Jens-Malte
 *  Gottfried.
 *
 *  This file contains the declaration of a cubic interpolator class.
 *
 *  @date 10.09.2009
 */
#ifndef _Interpolator_Cubic_H_
#define _Interpolator_Cubic_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#if defined interpolatorcubic_EXPORTS
///Visual C++ specific code
#define interpolatorcubic_DECLDIR __declspec(dllexport)
#else
#define interpolatorcubic_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define interpolatorcubic_DECLDIR
#endif

#include "Interpolator.h"

/// Cubic interpolation scheme.
template <typename T>
class interpolatorcubic_DECLDIR InterpolatorCubic : public Interpolator<T>
{
public:
    /// Default constructor
    InterpolatorCubic();

    /// Init interpolator.
    /// @param name             instance name
    InterpolatorCubic(const std::string& name);

    virtual ~InterpolatorCubic();

    T interpolate(const cimg_library::CImg<T>& src,
        float fx, float fy, int z = 0, int v = 0) const;
    T interpolate(const cimg_library::CImg<T>& src,
        float fx, float fy, float fz = 0, int v = 0) const;
};

#endif /* _Interpolator_Cubic_H_ */