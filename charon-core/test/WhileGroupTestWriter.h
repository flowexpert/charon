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
/** \file WhileGroupTestWriter.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 27.11.2012
 *  Declaraion of parameter class WhileGroupTestWriter.
 */
#ifndef _WHILEGROUPTESTWRITER_H_
#define _WHILEGROUPTESTWRITER_H_

#ifdef _MSC_VER
#ifdef whilegrouptestwriter_EXPORTS
/// Visual C++ specific code
#define whilegrouptestwriter_DECLDIR __declspec(dllexport)
#else
#define whilegrouptestwriter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define whilegrouptestwriter_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Test module output module for whilegroup
/** Test module output module for whilegroup
 */
class whilegrouptestwriter_DECLDIR WhileGroupTestWriter : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	WhileGroupTestWriter(const std::string& name = "");

	/// out1
	OutputSlot< int > out1;
	/// out2
	OutputSlot< float > out2;

	/// loopmode
	Parameter< bool > inside_group;


protected:
	/// Update object.
	virtual void execute();

};

#endif /* _WHILEGROUPTESTWRITER_H_ */
