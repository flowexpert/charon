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
/** \file WhileGroupTestReader.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 27.11.2012
 *  Declaraion of parameter class WhileGroupTestReader.
 */
#ifndef _WHILEGROUPTESTREADER_H_
#define _WHILEGROUPTESTREADER_H_

#ifdef _MSC_VER
#ifdef whilegrouptestreader_EXPORTS
/// Visual C++ specific code
#define whilegrouptestreader_DECLDIR __declspec(dllexport)
#else
#define whilegrouptestreader_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define whilegrouptestreader_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Test module input module for whilegroup
/** Test module input module for whilegroup
 */
class whilegrouptestreader_DECLDIR WhileGroupTestReader : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	WhileGroupTestReader(const std::string& name = "");

	/// in1
	InputSlot< int > in1;
	/// in2
	InputSlot< float > in2;

	/// loopmode
	Parameter< bool > loopmode;
	/// maxIt
	Parameter< int > maxIt;


protected:
	/// Update object.
	virtual void execute();
	int _iter;
};

#endif /* _WHILEGROUPTESTREADER_H_ */
