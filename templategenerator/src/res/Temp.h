/*  Copyright (C) @Year@ @Author@

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
/** \file @ModuleName@.h
 *  Declaration of the parameter class @ModuleName@.
 *  \author @AuthorEmail@
 *  \date @Date@
 */

#ifndef _@MODULENAME@_H_
#define _@MODULENAME@_H_

#ifdef _MSC_VER
#ifdef @modulename@_EXPORTS
/// Visual C++ specific code
#define @modulename@_DECLDIR __declspec(dllexport)
#else
#define @modulename@_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define @modulename@_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
@addHeaders@
@DoxyDoc@
template <typename T>
class @modulename@_DECLDIR @ModuleName@ :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	@ModuleName@(const std::string& name = "");
@ParamSlots@

protected:
	/// Update object.
	virtual void execute();
};

#endif // _@MODULENAME@_H_
