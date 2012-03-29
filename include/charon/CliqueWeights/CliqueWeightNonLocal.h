/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file CliqueWeightNonLocal.h
 *  Declaration of the parameter class CliqueWeightNonLocal.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 29.03.2012
 */

#ifndef _CLIQUEWEIGHTNONLOCAL_H_
#define _CLIQUEWEIGHTNONLOCAL_H_

#ifdef _MSC_VER
#ifdef cliqueweightnonlocal_EXPORTS
/// Visual C++ specific code
#define cliqueweightnonlocal_DECLDIR __declspec(dllexport)
#else
#define cliqueweightnonlocal_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define cliqueweightnonlocal_DECLDIR
#endif

#include <charon/CliqueWeight.h>

#include <charon-utils/CImg.h>

/// Uniform clique weighting
/** Uniform clique weighting
 *
 *  e.g. for various weighted median filters.
 */
template <typename T>
class cliqueweightnonlocal_DECLDIR CliqueWeightNonLocal :
	public CliqueWeight<T> {
public:
	/// default constructor
	/// \param name          Instance name
	CliqueWeightNonLocal(const std::string& name = "");

	/// data input slot
	InputSlot<cimg_library::CImgList<T> > data;

	/// motion input slot
	InputSlot<cimg_library::CImgList<T> > motion;

	/// spatial difference stddev
	Parameter< T > sigma_spatial;

	/// color difference stddev
	Parameter< T > sigma_color;

	/// occlusion divergence stddev
	Parameter< T > sigma_occ_divergence;

	/// occlusion color diff stddev
	Parameter< T > sigma_occ_color;

	/// function returning clique weight
	T getCliqueWeight( int nn, int xx, int yy, int zz, int cc,
	                   int n,  int x,  int y,  int z,  int c );

protected:
	/// main function
	virtual void execute();

private:
	/// gaussian distribution function
	inline T _gauss( T x, T mu, T sigma );

	/// \name cache members
	//\{
	cimg_library::CImgList<T> _data;
	cimg_library::CImgList<T> _motion;
	T _sigma_spatial;
	T _sigma_color;
	T _sigma_occ_divergence;
	T _sigma_occ_color;
	//\}
};

#endif // _CLIQUEWEIGHTNONLOCAL_H_

