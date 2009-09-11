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
 *  This file does only contain the interface, implementation of the
 *  template functions is in Interpolator.hxx. Include that file, if
 *  you want to instanciate any objects of the interpolator classes.
 *
 *  @date 05.05.2008
 */
#ifndef _Interpolator_H_
#define _Interpolator_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#if defined interpolator_EXPORTS
///Visual C++ specific code
#define interpolator_DECLDIR __declspec(dllexport)
#else
#define interpolator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define interpolator_DECLDIR
#endif

#include "CImg.h"
#include "ParameteredObject.h"

/// Abstract interpolation scheme.
/// This class is used to choose an interpolation
/// scheme during runtime without the
/// the need for if or switch-statements.
/// To use an interpolator use subclasses of Interpolator
/// e.g. InterpolatorLinear or InterpolatorCubic.
template <typename T>
class interpolator_DECLDIR Interpolator : public TemplatedParameteredObject<T> {
public:
    /// This pointer as outupt slot.
    OutputSlot<Interpolator<T>*> out;

    /// Init interpolator.
    /// Parameters are passed to the ParameteredObject constructor.
    /// @param className        class name
    /// @param name             instance name
    /// @param doc              class documentation
    Interpolator(const std::string& className, const std::string& name,
        const std::string& doc);

    virtual ~Interpolator();

    /// Calculate 2D interpolation.
    /// @param src              interpolation source
    /// @param fx               x position
    /// @param fy               y position
    /// @param z                z position
    /// @param v                4th dimension
    virtual T interpolate(const cimg_library::CImg<T>& src,
        float fx, float fy, int z, int v) const = 0;

    /// Calculate 3D interpolation.
    /// @param src              interpolation source
    /// @param fx               x position
    /// @param fy               y position
    /// @param fz               z position
    /// @param v                4th dimension
    virtual T interpolate(const cimg_library::CImg<T>& src,
        float fx, float fy, float fz, int v) const = 0;
};

#endif // _Interpolator_H_
