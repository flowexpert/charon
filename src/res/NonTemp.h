/*  Copyright (C) @Year@ @Author@

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
/** \file @ModuleName@.h
 *  \author @AuthorEmail@
 *  \date @Date@
 *  Declaraion of parameter class @ModuleName@.
 */
#ifndef _@MODULENAME@_H_
#define _@MODULENAME@_H_

#ifdef __msc_ver
#ifdef @modulename@_EXPORTS
///Visual C++ specific code
#define @modulename@_DECLDIR __declspec(dllexport)
#else
#define @modulename@_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define @modulename@_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
@addHeaders@
@DoxyDoc@
class @modulename@_DECLDIR @ModuleName@ : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	@ModuleName@(const std::string& name = "");
@ParamSlots@
	/// Update object.
	virtual void execute();
};

#endif /* _@MODULENAME@_H_ */
