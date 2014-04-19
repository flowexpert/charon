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
/** \file Diffusion.h
 *  Declaration of the parameter class Diffusion.
 *  \author <a href="mailto:helen.morrison@iwr.uni-heidelberg.de">
 *      Helen Morrison</a>
 *  \date 27.03.2012
 *
 */

#ifndef _BrightnessModels_Diffusion_H_
#define _BrightnessModels_Diffusion_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef brightnessmodels_diffusion_EXPORTS
///Visual C++ specific code
#define brightnessmodels_diffusion_DECLDIR __declspec(dllexport)
#else
#define brightnessmodels_diffusion_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define brightnessmodels_diffusion_DECLDIR
#endif

#include <string>
#include <charon-utils/CImg.h>
#include "../BrightnessModel.h"

/// namespace including the different brightness models
namespace BrightnessModels
{
/// includes diffusion in brightness model
/** \ingroup charon-stencil-helpers
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <class T>
class brightnessmodels_diffusion_DECLDIR Diffusion :
		public BrightnessModel<T> {
public:
	/// default constructor
	Diffusion(const std::string& name = "");

	/// \name Image Derivatives wrt mentioned coordinates
	//  \{
	InputSlot<cimg_library::CImgList<T> > dx, dy, dxx, dyy;
	//  \}

	/// Diffusion coefficient
	Parameter < float > D;

	/// \copydoc ParameteredObject::execute()
	virtual void execute();

	virtual void compute(
			const Point4D<int>& p, const int& v,
			std::map<std::string, T>& term, T& rhs,
			const std::string& unknown = "");

	virtual std::set<std::string>& getUnknowns();
};

} // namespace
#endif // _DIFFUSION_H_
