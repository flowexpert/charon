/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License
	as published by the Free Software Foundation, either version 3
	of the License, or (at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// \file Toggle.hxx
/// Implementation of the parameter class Toggle.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///     Michael Baron</a>
/// \date 10.04.2012

#ifndef TOGGLE_HXX_
#define TOGGLE_HXX_

#include "Toggle.h"
#include <charon-core/ParameteredObject.hxx>

template<typename T>
Toggle<T>::Toggle(const std::string& name) :
	TemplatedParameteredObject<T>("Toggle", name,
		"Toggle two inputs wrt a given timeout"),
	toggleTimeout(0)
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addParameter(timeout, "timeout",
		"timeout between toggles", 500);
	
	ParameteredObject::_addInputSlot(in1, "in1",
		"first input slot", "CImgList<T>");
	ParameteredObject::_addInputSlot(in2, "in2",
		"second input slot", "CImgList<T>");
	ParameteredObject::_addOutputSlot(out, "out",
		"output slot", "CImgList<T>");
}

template<typename T>
Toggle<T>::~Toggle()
{
	delete toggleTimeout ;
}

template<typename T>
void Toggle<T>::execute()
{
	if(!toggleTimeout)
		toggleTimeout = new ToggleTimeout( this );

	toggleTimeout->setTimeout( this->timeout() );
	toggleTimeout->start();
}

template<typename T>
void Toggle<T>::toggleToggle()
{
	if (state == 0) {
		state = 1;
		out() = in1();
	} else {
		state = 0;
		out() = in2();
	}
	this->resetExecuted();
}

#endif

