/*  Copyright (C) 2012 Gerald Mwangi

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
/** \file StatementGenerator.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 20.01.2012
 *  Declaraion of parameter class StatementGenerator.
 */
#ifndef _STATEMENTGENERATOR_H_
#define _STATEMENTGENERATOR_H_

#ifdef _MSC_VER
#ifdef statementgenerator_EXPORTS
/// Visual C++ specific code
#define statementgenerator_DECLDIR __declspec(dllexport)
#else
#define statementgenerator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define statementgenerator_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Takes a bool as parameter and outputs it to a slot
/** Takes a bool as parameter and outputs it to a slot
 */
class statementgenerator_DECLDIR StatementGenerator : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	StatementGenerator(const std::string& name = "");

	/// statement
	OutputSlot< bool > statement;

	/// statement
	Parameter< bool > statementpar;

	/// Update object.
	virtual void execute();
};

#endif /* _STATEMENTGENERATOR_H_ */
