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
/// @file MotionModel.h
/// defines abstract class MotionModel
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">
///     René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _MotionModel_h_
#define _MotionModel_h_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef motionmodel_EXPORTS
///Visual C++ specific code
#define motionmodel_DECLDIR __declspec(dllexport)
#else
#define motionmodel_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define motionmodel_DECLDIR
#endif

#include <set>
#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>
#include "Point4D.h"

/// abstract base class for the different motion models
/** If you need some input slots implementing your own MotionModel,
 *  you have to add them in your derived classs.
 *
 *  \ingroup charon-stencil-helpers
 */
template<class T>
class MotionModel: public TemplatedParameteredObject<T>
{
protected:
	/// unknowns handled by this motion model
	std::set<std::string> unknowns;

public:
	/// default constructor
	MotionModel(const std::string& classname, const std::string& name = "") :
		TemplatedParameteredObject<T>(classname, name, "computes the vectors")
	{
		this->_addOutputSlot(out, "this",
				"Pointer to itself", "MotionModel<T>");
		out = this;
	}

	/// output slot containing pointer to this class
	OutputSlot<MotionModel<T>*> out;

	/// compute the bcce-term of the motion model
	/**
	 *  \param[in]  p         coordinates of the current pixel (including time)
	 *  \param[in]  v         channel
	 *  \param[out] term      pre factor for each unknown
	 *  \param[out] rhs       T in which the bcce-term is written
	 *  \param[in]  unknown   name of the unknown to compute the term for
	 *                        (this is used for global methods)
	 */
	virtual void compute(
			const Point4D<int>& p, const int& v,
			std::map<std::string, T>& term, T& rhs,
			const std::string& unknown = "") = 0;

	/// returns a vector of names of unknowns of the model
	/// by asking the vectors length, you get the number of unknowns
	virtual std::set<std::string>& getUnknowns() = 0;
};

#endif
