/*  Copyright (C) 2011 Heidelberg Collaboratory for Image Processing

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
/** \file EnergyClassic.h
 *  Declaration of the parameter class EnergyClassic.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.10.2011
 */

#ifndef _ENERGYCLASSIC_H_
#define _ENERGYCLASSIC_H_

#ifdef _MSC_VER
#ifdef energyclassic_EXPORTS
/// Visual C++ specific code
#define energyclassic_DECLDIR __declspec(dllexport)
#else
#define energyclassic_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define energyclassic_DECLDIR
#endif

#include <charon/Stencil.h>

#include <charon/PenaltyFunction.h>
#include <CImg.h>
#include <vector>

/// EnergyStencil for so called classic regularization
/**
 *  This energy stencil implements so-called classic regularization,
 *  proposed by Horn and Schunck in their 1981 paper
 *  "Determining Optical Flow".
 *
 *  Its energy equals \f$ \Delta u + \Delta v \f$.
 *
 *  \ingroup charon-stencils
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class energyclassic_DECLDIR EnergyClassic :
	public Stencil::EnergyGradient<T>
{
public:
	/// Input slot for penalty function
	InputSlot< PenaltyFunction<T>* > penaltyFunction;

	/// Input slot for current motion components
	InputSlot< cimg_library::CImgList<T> > motionUV;

	/// default constructor
	/// \param name          Instance name
	EnergyClassic(const std::string& name = "");

	/// stencil's main function
	void execute();

	/// function yielding stencil's energy for given parameter vector
	T getEnergy( int n, int x, int y, int z, int c );

	/// function yielding stencil's energy gradient for given parameter vector
	std::vector<T> getEnergyGradient( int n, int x, int y, int z, int c );

	/// stencil's count of gradient components
	int getEnergyGradientDimensions();

private:
	T _energyFunction( T x, T xo );
	T _energyFunctionDeriv( T x, T xo );

	T _lamb;
	PenaltyFunction<T> *_penaltyFunction;
};

#endif // _ENERGYCLASSIC_H_

