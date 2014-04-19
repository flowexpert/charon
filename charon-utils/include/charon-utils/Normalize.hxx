/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file Normalize.hxx
/// Implementation of the parameter class Normalize.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009

#ifndef _NORMALIZE_HXX_
#define _NORMALIZE_HXX_

#include "Normalize.h"

template<typename T>
Normalize<T>::Normalize(const std::string& name) :
	TemplatedParameteredObject<T>(
			"normalize", name, "normalize images using cimg")
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	this->_addParameter(lower, "lower", "lower bound",  (T) 0);
	this->_addParameter(upper, "upper", "upper bound",  (T) 255);
	this->_addInputSlot(in,    "in",    "image input",  "CImgList<T>");
	this->_addOutputSlot(out,  "out",   "image output", "CImgList<T>");
}

template<typename T>
void Normalize<T>::execute() {
	out = in();
	cimglist_for(out(), i)
		out()[i] = in()[i].get_normalize(lower, upper);
}

#endif /* _NORMALIZE_HXX_ */
