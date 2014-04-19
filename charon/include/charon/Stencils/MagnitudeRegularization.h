/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file MagnitudeRegularization.h
 *  Declaration of the parameter class MagnitudeRegularization.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 05.03.2013
 */

#ifndef _MAGNITUDEREGULARIZATION_H_
#define _MAGNITUDEREGULARIZATION_H_

#ifdef _MSC_VER
#ifdef magnituderegularization_EXPORTS
/// Visual C++ specific code
#define magnituderegularization_DECLDIR __declspec(dllexport)
#else
#define magnituderegularization_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define magnituderegularization_DECLDIR
#endif

#include <charon/Stencil.h>

#include <charon/PenaltyFunction.h>
#include <CImg.h>
#include <charon-utils/Roi.h>

/// Stencil for regularization of the magnitude r
/**
 *  This stencil regularizes the magnitude r.
 *
 *  \ingroup charon-stencils
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class magnituderegularization_DECLDIR MagnitudeRegularization :
	public Stencil::Mask<T>
{
public:
	/// Input slot for penalty function
	InputSlot< PenaltyFunction<T>* > penaltyFunction;

	/// Input slot for current motion components
	InputSlot< cimg_library::CImgList<T> > parameterGuess;

	/// Input slot for region of interest
	InputSlot< Roi<int>* > roi;

	/// Input slot for regularization mask
	InputSlot< cimg_library::CImgList<T> > mask;

	/// default constructor
	/// \param name          Instance name
	MagnitudeRegularization(const std::string& name = "");

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

#endif // _MAGNITUDEREGULARIZATION_H_

