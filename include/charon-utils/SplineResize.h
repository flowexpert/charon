/*  Copyright (C) 2009 Daniel Kondermann

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
/** @file SplineResize.h
 * Declaration of the parameter class SplineResize.
 * @author Daniel Kondermann
 * @date 20.01.2010
 *

 */

#ifndef _SPLINERESIZE_H_
#define _SPLINERESIZE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef splineresize_EXPORTS
///Visual C++ specific code
#define splineresize_DECLDIR __declspec(dllexport)
#else
#define splineresize_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define splineresize_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <vigra/multi_array.hxx>
#include <vigra/multi_resize.hxx>
#include <vigra/windows.h>

/// Resize image using B-spline interpolation
/** The function implements separable spline interpolation algorithm described
 *  in M. Unser, A. Aldroubi, M. Eden, "B-Spline Signal Processing"
 *  IEEE Transactions on Signal Processing, vol. 41, no. 2, pp. 821-833
 *  (part I), pp. 834-848 (part II), 1993. to obtain optimal interpolation
 *  quality and speed. The implementation ensures that image values are
 *  interpolated rather than smoothed by first calling a recursive (sharpening)
 *  prefilter as described in the above paper.
 *  The input image must have a size of at least 4x4, the destination of at
 *  least 2x2. The scaling factors are then calculated accordingly.
 *  If the source image is larger than the destination, it is smoothed
 *  (band limited) using a recursive exponential filter.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template <typename T>
class splineresize_DECLDIR SplineResize : public TemplatedParameteredObject<T> {
private:
	vigra::MultiArray<5, T> result; ///< temporary result storage
public:

	/// Factor for resizing. If zero, the parameters newDim* are used. 
	Parameter < float > factorX;
	/// Factor for resizing. If zero, the parameters newDim* are used. 
	Parameter < float > factorY;
	/// Factor for resizing. If zero, the parameters newDim* are used. 
	Parameter < float > factorZ;
	/// Factor for resizing. If zero, the parameters newDim* are used. 
	Parameter < float > factorT;
	/// Factor for resizing. If zero, the parameters newDim* are used. 
	Parameter < float > factorV;
	/// Used if factorX=0. In this case the parameter factorX is ignored. If zero, nothing is scaled.
	Parameter < unsigned int > newDimX;
	/// Used if factorY=0. In this case the parameter factorY is ignored. If zero, nothing is scaled. 
	Parameter < unsigned int > newDimY;
	/// Used if factorZ=0. In this case the parameter factorZ is ignored. If zero, nothing is scaled.
	Parameter < unsigned int > newDimZ;
	/// Used if factorT=0. In this case the parameter factorT is ignored. If zero, nothing is scaled.
	Parameter < unsigned int > newDimT;
	/// Used if factorV=0. In this case the parameter factorV is ignored. If zero, nothing is scaled.
	Parameter < unsigned int > newDimV;
	/// The order of the spline. Minimum is 3, maximum is 10.
	Parameter < unsigned int > splineOrder;


	/// The source image. 
	InputSlot < vigra::MultiArray<5, T> > in;
	/// The resized image. 
	OutputSlot < vigra::MultiArray<5, T> > out;

	/// create a new SplineResize object
	/// @param name             Object name
	SplineResize(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();
};

#endif // _SplineResize_H_



