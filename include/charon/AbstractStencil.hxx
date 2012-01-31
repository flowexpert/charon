/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** @file AbstractStencil.hxx
 *  Implementation of class AbstractStencil.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *
 *  @date 31.01.2012
 */


#ifndef _ABSTRACTSTENCIL_HXX_
#define _ABSTRACTSTENCIL_HXX_

#include "AbstractStencil.h"

#include <ParameteredObject.hxx>

template <typename T>
AbstractStencil::Base<T>::Base(
		const std::string& classname, const std::string& name,
		const std::string& doc) :
		TemplatedParameteredObject<T>(classname,name,doc +
				"<br><br>This class is derived from class AbstractStencil.<br>"
				"An abstractstencil is a universal stencil that can be"
				"enhanced by adding interfaces to it.")
{
	this->_addOutputSlot(out,"this","Pointer to itself","Stencil<T>*");
	this->_addParameter(lambda,"lambda","weight",T(1),"T");
	out = this;
}

#endif //_abstractstencil_HXX_

