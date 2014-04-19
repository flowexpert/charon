/*  Copyright (C) 2011, 2012, 2013
    University of Heidelberg (IWR/HCI)

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
#include <charon-utils/Roi.h>

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
	public Stencil::EnergyHessian<T>,
	public Stencil::Mask<T>
{
public:
	/// Input slot for penalty function
	InputSlot< PenaltyFunction<T>* > penaltyFunction;

	/// Input slot for current motion components
	InputSlot< cimg_library::CImgList<T> > motionUV;

	/// Input slot for region of interest
	InputSlot< Roi<int>* > roi;

	/// Input slot for regularization mask
	InputSlot< cimg_library::CImgList<T> > regionMask;
	//  do NOT use masked pixels for regularization
	//  perform regularization _at_ these pixels as usually
	//  (effect: no regularization over masked pixels)

	/// Input slot for mask of matches
	InputSlot< cimg_library::CImgList<T> > matchMask;
	//  do not regularize masked pixels
	//  instead, use current pixel value

	/// default constructor
	/// \param name          Instance name
	EnergyClassic(const std::string& name = "");

	/// function yielding stencil's energy
	T getEnergy( int n, int x, int y, int z, int c );

	/// function yielding stencil's energy gradient
	std::vector<T> getEnergyGradient( int n, int x, int y, int z, int c );

	/// function yielding stencil's energy Hessian
	std::vector<T> getEnergyHessian( int n, int x, int y, int z, int c );

	/// stencil's count of gradient components
	int getEnergyGradientDimensions();

	/// if set, also diagonal cliques are considered for regularization
	Parameter<bool> diagonalRegularization;

	/// ParameterList containing all unknowns of the Stencil.
	ParameterList<std::string> pUnknowns;

	virtual void updateStencil(
			const std::string& unknown,
			const Point4D<int>& p=Point4D<int>(), const int& v=0);
	virtual cimg_library::CImg<T> apply(
			const cimg_library::CImgList<T>& seq,
			const unsigned int frame) const;

protected:
	/// stencil's main function
	void execute();

private:
	/// \name clique potentials
	//\{
	T _energy( int pn, int px, int py, int pz, int pc, T x, T xo );
	T _energyGradient( int pn, int px, int py, int pz, int pc, T x, T xo );
	T _energyHessian( int pn, int px, int py, int pz, int pc, T x, T xo );
	//\}

	/// \name cache members
	//\{
	T _lamb;
	cimg_library::CImgList<T> _lambdaMask;
	PenaltyFunction<T>* _penaltyFunction;
	int _xBegin;
	int _xEnd;
	int _yBegin;
	int _yEnd;
	//\}

	/// \name precalculate substencil data
	//  \{
	cimg_library::CImg<T>     _dataMask;    ///< common data
	cimg_library::CImg<char>  _patternMask; ///< common pattern
	Point4D<int>              _center;      ///< common center
	cimg_library::CImgList<T> _rhsVals;     ///< values for rhs (optional)
	//  \}
};

#endif // _ENERGYCLASSIC_H_

