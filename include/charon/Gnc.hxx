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
/** \file Gnc.hxx
 *  Implementation of the parameter class Gnc.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 05.04.2012
 */

#ifndef _GNC_HXX_
#define _GNC_HXX_

#include "Gnc.h"

template <typename T>
Gnc<T>::Gnc(const std::string& name) :
	TemplatedParameteredObject<T>("gnc", name,
		"this implements the GNC iteration scheme.<br><br>")
{
	ParameteredObject::_addParameter< T >(ratio, "ratio", "gnc mixture ratio", T(0.2));
	ParameteredObject::_addInputSlot (
			initial, "initial", "input for data, which is faded out", "CImgList<T>");
	ParameteredObject::_addInputSlot (
			final, "final", "input for data, which is faded in", "CImgList<T>");
	ParameteredObject::_addInputSlot (
			step, "step", "current GNC iteration step");
	ParameteredObject::_addOutputSlot(
			out, "out", "data output", "CImgList<T>");
	ParameteredObject::_setTags("deprecated");
}

template <typename T>
void Gnc<T>::execute()
{
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const cimg_library::CImgList<T> & _initial = initial();
	const cimg_library::CImgList<T> & _final = final();
	cimg_library::CImgList<T> & _out = out();

	_out = _initial;

	unsigned int _step = step();
	T _ratio = ratio();

	cimglist_for(_out,nn)
	cimg_forXYZC(_out[nn], xx, yy, zz, cc)
	{
		_out( nn, xx, yy, zz, cc ) = _step * _ratio * _final( nn, xx, yy, zz, cc )
		                           + (T(1.0) - _step * _ratio) * _initial( nn, xx, yy, zz, cc );
	}
}

#endif /* _GNC_HXX_ */

