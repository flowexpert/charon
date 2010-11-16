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
/** @file L2Norm.h
 *  Declaration of the parameter class L2Norm.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */


#ifndef _L2NORM_H_
#define _L2NORM_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef l2norm_EXPORTS
///Visual C++ specific code
#define l2norm_DECLDIR __declspec(dllexport)
#else
#define l2norm_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define l2norm_DECLDIR
#endif

#include "../Stencil.h"

/// Laplacian stencil used for regularization in global methods.
template <class T>
class l2norm_DECLDIR L2Norm : public Stencil<T>
{
private:
	/// \name precalculate substencil data
	//  \{
	cimg_library::CImg<T>     _dataMask;    ///< common data
	cimg_library::CImg<char>  _patternMask; ///< common pattern
	Point4D<int>              _center;      ///< common center
	cimg_library::CImgList<T> _rhsVals;     ///< values for rhs (optional)
	//  \}


public:
	/// Number of dimensions (1D, 2D, 3D, 4D).
	Parameter<unsigned int> dimensions;
	/// ParameterList containing all unknowns of the Stencil.
	ParameterList<std::string> pUnknowns;

	/// Weight of the calculated RHS (only if initial guess is given)
	Parameter<double> rhsWeight;
	/// Initial flow guess for rhs calculation
	InputSlot<cimg_library::CImgList<T> > flowGuess;

	/// default constructor
	/// \param name instance name
	L2Norm(const std::string& name = "");

	/// \copydoc ParameteredObject::execute()
	virtual void execute();

	virtual void updateStencil(
			const std::string& unknown,
			const Point4D<int>& p=Point4D<int>(), const int& v=0);

	virtual cimg_library::CImg<T> apply(
			const cimg_library::CImgList<T>& seq,
			const unsigned int frame) const;

	virtual ~L2Norm();
};

#endif // _L2NORM_H_
