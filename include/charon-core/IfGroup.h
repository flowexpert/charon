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
/** \file IfGroup.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 17.01.2012
 *  Declaraion of parameter class IfGroup.
 */
#ifndef _IFGROUP_H_
#define _IFGROUP_H_

#ifdef _MSC_VER
#ifdef ifgroup_EXPORTS
/// Visual C++ specific code
#define ifgroup_DECLDIR __declspec(dllexport)
#else
#define ifgroup_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define ifgroup_DECLDIR
#endif

#include "ParameteredGroupObject.h"

/// Execute a group of objects if a given statement is true
/** Execute a group of objects if a given statement is true
 */
class ifgroup_DECLDIR IfGroup : public ParameteredGroupObject {
public:
	/// default constructor
	/// \param name             instance name
	IfGroup(const std::string& name = "");

	/// determines if group is executed
	InputSlot< bool > statement;

	/// Update object.
	virtual void executeGroup();
};

#endif /* _IFGROUP_H_ */
