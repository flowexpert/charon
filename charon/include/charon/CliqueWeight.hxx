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
/** @file CliqueWeight.hxx
 *  Implementation of class CliqueWeight.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *
 *  @date 29.03.2012
 */

#ifndef _CLIQUEWEIGHT_HXX_
#define _CLIQUEWEIGHT_HXX_

#include "CliqueWeight.h"
#include <ParameteredObject.hxx>

template <typename T>
CliqueWeight<T>::CliqueWeight(
	const std::string& classname, const std::string& name,
	const std::string& doc) :
	TemplatedParameteredObject<T>(classname,name,doc + "")
{
	this->_addOutputSlot(
		out,"this","Pointer to itself", "CliqueWeight<T>*");
}

template <typename T>
void CliqueWeight<T>::execute()
{
	out = this;
}

#endif

