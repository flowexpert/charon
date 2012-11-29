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
/** \file WhileGroupTestStatement.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 27.11.2012
 *  Declaraion of parameter class WhileGroupTestStatement.
 */
#ifndef _WHILEGROUPTESTSTATEMENT_H_
#define _WHILEGROUPTESTSTATEMENT_H_

#ifdef _MSC_VER
#ifdef whilegroupteststatement_EXPORTS
/// Visual C++ specific code
#define whilegroupteststatement_DECLDIR __declspec(dllexport)
#else
#define whilegroupteststatement_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define whilegroupteststatement_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-core/StatementIntf.h>
/// Test statement for whilegroup
/** Test statement for whilegroup
 */
class whilegroupteststatement_DECLDIR WhileGroupTestStatement : public ParameteredObject, public StatementIntf {
public:
	/// default constructor
	/// \param name             instance name
	WhileGroupTestStatement(const std::string& name = "");

	/// num_iter
	Parameter< int > num_iter;
	OutputSlot<int> iteration;
	virtual bool operator()() const;



protected:
	/// Update object.
	virtual void execute();
	int _iter;
};

#endif /* _WHILEGROUPTESTSTATEMENT_H_ */
