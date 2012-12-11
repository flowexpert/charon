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
/** \file VirtualSlotTestWriter.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 28.11.2012
 *  Declaraion of parameter class VirtualSlotTestWriter.
 */
#ifndef _VIRTUALSLOTTESTWRITER_H_
#define _VIRTUALSLOTTESTWRITER_H_

#ifdef _MSC_VER
#ifdef virtualslottestwriter_EXPORTS
/// Visual C++ specific code
#define virtualslottestwriter_DECLDIR __declspec(dllexport)
#else
#define virtualslottestwriter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define virtualslottestwriter_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>


/// Generates an integer and a CImg image to test the virtual slots
/** Generates an integer and a CImg image to test the virtual slots
 */
class virtualslottestwriter_DECLDIR VirtualSlotTestWriter : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	VirtualSlotTestWriter(const std::string& name = "");

	/// integer
	OutputSlot< int > integer;
	/// image
	OutputSlot< std::string > image;


protected:
	/// Update object.
	virtual void execute();
};

#endif /* _VIRTUALSLOTTESTWRITER_H_ */
