/*  Copyright (C) 2012 Helen Morrison

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
/// @file LucasKanade.h
/// defines class MotionModels::LucasKanade
/// @author <a href="mailto:helen.morrison@iwr.uni-heidelberg.de">
///		Helen Morrison</a>
/// @date 13.04.2012

#ifndef _MotionModels_LucasKanade_h_
#define _MotionModels_LucasKanade_h_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef motionmodels_lucaskanade_EXPORTS
///Visual C++ specific code
#define motionmodels_lucaskanade_DECLDIR __declspec(dllexport)
#else
#define motionmodels_lucaskanade_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define motionmodels_lucaskanade_DECLDIR
#endif

#include <string>
#include "../MotionModel.h"

/// namespace for the different motion models
namespace MotionModels
{

/// motion model with local constant flow
/** Computes the bcce-term for the parameters
 *  \f$ a_i \f$ for an optical flow of the form
 *  \f[
		\vec f \left( \begin{array}{c} x \\ y \\ z \end{array} \right) =
			\left( \begin{array}{c} a_1 \\ a_2 \\ a_3 \end{array} \right)
	\f]
 *
 *  uses convolved brightness derivatives, equivalent to the Lucas
 *  & Kanade method.
 *
 *  \ingroup charon-stencil-helpers
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template<class T>
class motionmodels_lucaskanade_DECLDIR LucasKanade :
		public MotionModel<T> {

public:
	/// default constructor
	LucasKanade(const std::string& name = "");

	/// @name input slots for the derivatives
	//@{
	InputSlot<cimg_library::CImgList<T> > IxIx, IxIy, IyIy, IxIt, IyIt;
	//@}

	virtual void compute(
			const Point4D<int>& p, const int& v,
			std::map<std::string, T>& term, T& rhs,
			const std::string& unknown = "");

	virtual std::set<std::string>& getUnknowns();
};

} // namespace

#endif

