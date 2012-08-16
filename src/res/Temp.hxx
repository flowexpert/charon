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
/** \file @ModuleName@.hxx
 *  Implementation of the parameter class @ModuleName@.
 *  \author @AuthorEmail@
 *  \date @Date@
 */

#ifndef _@MODULENAME@_HXX_
#define _@MODULENAME@_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "@ModuleName@.h"

template <typename T>
@ModuleName@<T>::@ModuleName@(const std::string& name) :
		TemplatedParameteredObject<T>(
			"@ModuleName@", name,
			"@PluginDoc@"
		)@ctorAdd@
{@ctorCont@
}

template <typename T>
void @ModuleName@<T>::execute() {
	// your code goes here :-)
}

#endif /* _@MODULENAME@_HXX_ */
