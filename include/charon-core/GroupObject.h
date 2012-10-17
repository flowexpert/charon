/*  Copyright (C) 2012 Gerald Mwangi

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
/** \file GroupObject.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 15.10.2012
 *  Declaraion of parameter class GroupObject.
 */
#ifndef _GROUPOBJECT_H_
#define _GROUPOBJECT_H_

#ifdef _MSC_VER
#ifdef groupobject_EXPORTS
/// Visual C++ specific code
#define groupobject_DECLDIR __declspec(dllexport)
#else
#define groupobject_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define groupobject_DECLDIR
#endif

#include <charon-core/ParameteredGroupObject.h>

/// Loads a child workflow and executes it
/** Loads a child workflow and executes it
 */
class groupobject_DECLDIR GroupObject : public ParameteredGroupObject {
public:
	/// default constructor
	/// \param name             instance name
	GroupObject(const std::string& name = "");


protected:
	/// Update object.
	virtual void execute();
};

#endif /* _GROUPOBJECT_H_ */
