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
/** \file Statement.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 29.10.2012
 *  Declaraion of parameter class Statement.
 */
#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#ifdef _MSC_VER
#ifdef statement_EXPORTS
/// Visual C++ specific code
#define statement_DECLDIR __declspec(dllexport)
#else
#define statement_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define statement_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include "../include/charon-core/StatementIntf.h"

/// Statement module to be used in loops
/** Statement module to be used in loops. This plugin is intended for loops
 *  (e.g. WhileGroup). It interfaces the loop  with the bool statement
 *  connected to it.
 */
class statement_DECLDIR Statement : public ParameteredObject,public StatementIntf {
public:
	/// default constructor
	/// \param name             instance name
	Statement(const std::string& name = "");

	/// Statement to interface to the external loop
    InputSlot< bool > statement;

	virtual bool operator ()() const;


protected:
	/// Update object.
	virtual void execute();
private:
	bool _statement;
};

#endif /* _STATEMENT_H_ */
