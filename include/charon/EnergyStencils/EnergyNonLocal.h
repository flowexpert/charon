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
/** \file EnergyNonLocal.h
 *  Declaration of the parameter class EnergyNonLocal.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.12.2011
 */

#ifndef _ENERGYNONLOCAL_H_
#define _ENERGYNONLOCAL_H_

#ifdef _MSC_VER
#ifdef energynonlocal_EXPORTS
/// Visual C++ specific code
#define energynonlocal_DECLDIR __declspec(dllexport)
#else
#define energynonlocal_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define energynonlocal_DECLDIR
#endif

#include <charon/Stencil.h>
#include <charon-utils/CImg.h>

/// EnergyStencil for Regularization
/** EnergyStencil for Regularization.
 *
 *  based on the improved median filtering model (section 6)
 *  from "Secrets of Optical Flow Estimation and Their Principles"
 *  (Deqing Sun, Stefan Roth, Michael Black ; CVPR 2010)
 *
 */
template <typename T>
class energynonlocal_DECLDIR EnergyNonLocal :
	public Stencil::EnergyGradient<T>
{
public:
	Parameter< T > norm;
	Parameter< int > useWeight;
	Parameter< int > radius;              ///<  radius of the neighborhood
	Parameter< T > sigma_spatial;         ///<  spatial difference weight
	Parameter< T > sigma_color;           ///<  color difference weight
	Parameter< T > sigma_occ_divergence;  ///<  occlusion divergence weight
	Parameter< T > sigma_occ_color;       ///<  occlusion color weight

	InputSlot< cimg_library::CImgList<T> > img;

	InputSlot< cimg_library::CImgList<T> > motionUV;

	/// default constructor
	/// \param name          Instance name
	EnergyNonLocal(const std::string& name = "");

	/// Update object.
	void execute();

	/// stencil's energy function
	T getEnergy( int n, int x, int y, int z, int c );

	/// stencil's energy gradient function
	std::vector<T> getEnergyGradient( int n, int x, int y, int z, int c );

	/// stencil's gradient's components count
	int getEnergyGradientDimensions();

	T _lamb, _norm;
	int _radius;
	int _useWeight;
	T _sigma_spatial, _sigma_color, _sigma_occ_divergence, _sigma_occ_color;

	inline T _gauss( T x, T mu, T sigma );
	inline T _dgauss( T x, T mu, T sigma );
};

#endif // _ENERGYNONLOCAL_H_

