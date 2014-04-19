/*  Copyright (C) 2012 Jens-Malte Gottfried

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
/** @file CasedDummy.h
 *  CasedDummy plugin class for PluginManager tests
 *  \date 23.07.2012
 *  \author <a href="mailto:jmgottfried@web.de>Jens-Malte Gottfried</a>
 */

#ifndef CASEDDUMMY_H
#define CASEDDUMMY_H

/** \def caseddummy_DLL_PUBLIC dll import/export flags */
#ifdef _MSC_VER
	#ifdef caseddummy_EXPORTS
		#define caseddummy_DLL_PUBLIC __declspec(dllexport)
	#else
		#define caseddummy_DLL_PUBLIC __declspec(dllimport)
	#endif
#else
	#define caseddummy_DLL_PUBLIC
#endif

#include <charon-core/ParameteredObject.hxx>

/// Module with cased name and slot names
class caseddummy_DLL_PUBLIC CasedDummy : public ParameteredObject {

public:
	/// sample integer output slot
	OutputSlot<int>   oUt;

	/// sample integer input slot
	InputSlot<int>   iN;

	/// create a new sample object
	/// @param name             Object name
	CasedDummy(const std::string& name = "") :
			ParameteredObject("CasedDummy", name, "cased name and slot names"),
			iN(true, true)    // optional and multislot
	{
		// slots
		_addInputSlot (iN,  "iN",  "sample integer input slot");
		_addOutputSlot(oUt, "oUt", "sample integer output slot");
	}
protected:
	virtual void execute() {}
};

#endif // CASEDDUMMY_H
