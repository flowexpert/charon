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
/** \file WhileGroup.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 16.02.2012
 *  Declaraion of parameter class WhileGroup.
 */
#ifndef _WHILEGROUP_H_
#define _WHILEGROUP_H_

#ifdef _MSC_VER
#ifdef whilegroup_EXPORTS
/// Visual C++ specific code
#define whilegroup_DECLDIR __declspec(dllexport)
#else
#define whilegroup_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define whilegroup_DECLDIR
#endif

#include "ParameteredGroupObject.h"


/// Executes the group as long as a given statement is true
/** Executes the group as long as a given statement is true
 */
class StatementIntf;

class whilegroup_DECLDIR WhileGroup : public ParameteredGroupObject {
public:
	/// default constructor
	/// \param name             instance name
	WhileGroup(const std::string& name = "");

        void initializeGroup();

	virtual void initializeWhileGroup();

        InputSlot<bool> statement;

	/// Update object.
	 void executeGroup();
private:
	StatementIntf* _innerWhilestatement;

};



#endif /* _WHILEGROUP_H_ */
