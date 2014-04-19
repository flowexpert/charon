/*  Copyright (C) 2012 Gerald Mwangi
    Copyright (C) 2012 Michael Baron

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
/** \file Statement.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 08.11.2012
 *  Declaraion of parameter class ForStatement.
 */
#ifndef _FORSTATEMENT_H_
#define _FORSTATEMENT_H_

#ifdef _MSC_VER
#ifdef forstatement_EXPORTS
/// Visual C++ specific code
#define forstatement_DECLDIR __declspec(dllexport)
#else
#define forstatement_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define forstatement_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include "StatementIntf.h"

/// ForStatement module to be used in counting loops
/** ForStatement module to be used in loops. This plugin is intended for loops
 *  (e.g. WhileGroup). It interfaces the loop  with a bool statement
 *  depending on its counter values.
 */
class forstatement_DECLDIR ForStatement : public ParameteredObject,public StatementIntf {
public:
	/// default constructor
	/// \param name             instance name
	ForStatement(const std::string& name = "");

	/// input slot for initial value
	InputSlot< double > initialValue;
	/// input slot for increment value
	InputSlot< double > incrementValue;
	/// input slot for final value
	InputSlot< double > finalValue;

	/// output slot for current value
	OutputSlot< int > currentValue;

	virtual bool operator ()() const;

protected:
	/// Update object.
	virtual void execute();
    virtual void reset();
private:
	int _initialValue;
	int _incrementValue;
	int _finalValue;
	int _currentValue;
};

#endif /* _FORSTATEMENT_H_ */

