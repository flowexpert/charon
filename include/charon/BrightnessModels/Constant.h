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
/// @file Constant.h
/// defines class BrightnessModels::Constant
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">
/// René Steinbrügge</a>
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 27.05.2009

#ifndef _BrightnessModels_Constant_H_
#define _BrightnessModels_Constant_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef brightnessmodels_constant_EXPORTS
///Visual C++ specific code
#define brightnessmodels_constant_DECLDIR __declspec(dllexport)
#else
#define brightnessmodels_constant_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define brightnessmodels_constant_DECLDIR
#endif

#include <string>
#include <charon-utils/CImg.h>
#include "../BrightnessModel.h"

/// namespace including the different brightness models
namespace BrightnessModels
{

/// a brightness model for no brightness change
/** \ingroup charon-stencil-helpers
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template<class T>
class brightnessmodels_constant_DECLDIR Constant :
		public BrightnessModel<T> {
public:
	/// default constructor
	Constant(const std::string& name = "");

	virtual void compute(
			const Point4D<int>& p, const int& v,
			std::map<std::string, T>& term, T& rhs,
			const std::string& unknown = "");

	virtual std::set<std::string>& getUnknowns();

protected:
	/// \copydoc ParameteredObject::execute()
	virtual void execute();
};

} // namespace

#endif
