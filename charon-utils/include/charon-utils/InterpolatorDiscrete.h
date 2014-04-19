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
/** @file InterpolatorDiscrete.h
 *  Declaration of template class InterpolatorDiscrete.
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
#ifndef _Interpolator_Discrete_H_
#define _Interpolator_Discrete_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#if defined interpolatordiscrete_EXPORTS
///Visual C++ specific code
#define interpolatordiscrete_DECLDIR __declspec(dllexport)
#else
#define interpolatordiscrete_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define interpolatordiscrete_DECLDIR
#endif

#include "Interpolator.h"

/// Discrete interpolation scheme.
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup charon-interpolators
 */
template <typename T>
class interpolatordiscrete_DECLDIR InterpolatorDiscrete : public Interpolator<T>
{
public:
	/// Init interpolator.
	/// @param name             instance name
	InterpolatorDiscrete(const std::string& name = "");

	T interpolate(const cimg_library::CImg<T>& src,
		float fx, float fy = 0.f, float fz = 0.f, int v = 0) const;
};

#endif /* _Interpolator_Discrete_H_ */
