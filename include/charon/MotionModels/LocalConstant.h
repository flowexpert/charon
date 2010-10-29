/*  Copyright (C) 2009 René Steinbrügge

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
/// @file LocalConstant.h
/// defines class MotionModels::LocalConstant
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">
///		René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _MotionModels_LocalConstant_h_
#define _MotionModels_LocalConstant_h_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef motionmodels_localconstant_EXPORTS
///Visual C++ specific code
#define motionmodels_localconstant_DECLDIR __declspec(dllexport)
#else
#define motionmodels_localconstant_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define motionmodels_localconstant_DECLDIR
#endif

#include <string>
#include "../MotionModel.h"

/// namespace for the different motion models
namespace MotionModels
{

/** @brief motion model with local constant flow
 *  @details Computes the bcce-term for the parameters
 *  @f$ a_i @f$ for an optical flow of the form
 *  @f[
		\vec f \left( \begin{array}{c} x \\ y \\ z \end{array} \right) =
			\left( \begin{array}{c} a_1 \\ a_2 \\ a_3 \end{array} \right)
	@f]
 */
template<class T>
class motionmodels_localconstant_DECLDIR LocalConstant :
		public MotionModel<T> {
public:
	/// default constructor
	LocalConstant(const std::string& name = "");

	/// @name input slots for the derivatives
	//@{
	InputSlot<cimg_library::CImgList<T> > dx, dy, dz, dt;
	//@}

	virtual void compute(
			const Point4D<int>& p, const int& v,
			std::map<std::string, T>& term, T& rhs,
			const std::string& unknown = "");

	virtual void computeEnergy(
			const Point4D<int>& p, const int& v,
			const cimg_library::CImgList<T>& parameterList,
			double& energy);

	virtual std::set<std::string>& getUnknowns();
};

} // namespace

#endif

