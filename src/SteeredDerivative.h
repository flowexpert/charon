/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// \file SteeredDerivative.h
/// Declaration of the parameter class SteeredDerivative
/// \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// \date 09.10.2009

#ifndef STEERED_DERIVATIVE_H_
#define STEERED_DERIVATIVE_H_

#include <charon-core/ParameteredObject.h>
#include <CImg.h>

#ifdef HANDLE_DLL
#ifdef steeredderivative_EXPORTS
/// DLL handling
#define steeredderivative_DECLDIR __declspec(dllexport)
#else
#define steeredderivative_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// No DLL handling
#define steeredderivative_DECLDIR
#endif

/** Calculation of steered derivatives.
 *
 *  Uses structure tensor information to calculate derivatives
 *  aligned and orthogonal to the local image structure.
 *  The steered derivatives are linear combinations of the standart
 *  derivatives.
 */
template <typename T>
class steeredderivative_DECLDIR SteeredDerivative : 
		public TemplatedParameteredObject<T>
{
public:
	/// standart constructor
	/// \param name		instance name
	SteeredDerivative(const std::string& name = "");

	/// \name derivative inputs and outputs
	//@{
	InputSlot<cimg_library::CImgList<T> > dX;
	InputSlot<cimg_library::CImgList<T> > dY;
	OutputSlot<cimg_library::CImgList<T> > dA;	///< aligned
	OutputSlot<cimg_library::CImgList<T> > dO;	///< orthogonal
	//@}

	/// Eigenvectors of the structure tensor
	InputSlot<cimg_library::CImgList<T> > eigenVectors;

	/// calculate steered derivatives
	virtual void execute();
};

#endif // STEERED_DERIVATIVE_H_
