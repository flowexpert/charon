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
/// @file BrightnessModel.h
/// defines abstract class BrightnessModel
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009


#ifndef _BrightnessModel_h_
#define _BrightnessModel_h_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef brightnessmodel_EXPORTS
///Visual C++ specific code
#define brightnessmodel_DECLDIR __declspec(dllexport)
#else
#define brightnessmodel_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define brightnessmodel_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>
#include <set>
#include "Point4D.h"

/// abstract base class for the different BrightnessModels
/** \ingroup charon-stencil-helpers */
template<class T>
class brightnessmodel_DECLDIR BrightnessModel :
		public TemplatedParameteredObject<T>
{
protected:
	/// Set of unknowns
	std::set<std::string> unknowns;

	/// \implements ParameteredObject::execute()
	virtual void execute() {
		out() = this;
	}

public:
	/// default constructor
	BrightnessModel(
			const std::string& classname /**[in] class name*/,
			const std::string& name = "" /**[in] instance name*/) :
		TemplatedParameteredObject<T>(classname, name,
				"computes the vectors for brightness-modeling")
	{
		ParameteredObject::_addOutputSlot(
					out, "this", "Pointer to itself", "BrightnessModel<T>");
		ParameteredObject::_addInputSlot(
					img, "image", "Image to work with", "CImgList<T>");
	}

	/// OutputSlot containing pointer to this BrightnessModel
	OutputSlot<BrightnessModel*> out;

	/// imagelist to work on
	InputSlot<cimg_library::CImgList<T> > img;

	/// compute the bcce term
	/** \param p             coordinates (4D)
	 *  \param v             channel
	 *  \param term          map to which the result is written
	 *  \param rhs           right hand side
	 *  \param unknown       name of the unknown to compute the term for
	 *                       (this is used for global methods)
	 */
	virtual void compute(
			const Point4D<int>& p, const int& v,
			std::map<std::string, T>& term, T& rhs,
			const std::string& unknown = "") = 0;

	/**
	 * Get the unknowns of the model.
	 * @return Set of strings which contains the names of the unknowns
	 */
	virtual std::set<std::string>& getUnknowns() = 0;
};

#endif
