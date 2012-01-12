/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file Mask1D.hxx
/// Implementation of the parameter class Mask1D
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @date 02.10.2009

#ifndef MASK1D_HXX_
#define MASK1D_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "Mask1D.h"
#include <stdexcept>

template <typename T>
Mask1D<T>::Mask1D(const std::string& name) :
		TemplatedParameteredObject<T>(
				"Mask1D", name, "generate simple 1d mask")
{
	ParameteredObject::_addParameter(
			values, "values", "mask values");
	ParameteredObject::_addParameter(
			dir, "dir", "mask direction", 0u);
	ParameteredObject::_addParameter(
			normalize, "normalize", "divide values by L1Norm",
			false);
	ParameteredObject::_addOutputSlot(
			out, "out", "mask output", "CImgList<T>");
}

template <typename T>
void Mask1D<T>::execute() {
	switch (dir()) {
	case 0:
		out().assign(1u, values().size(), 1u, 1u, 1u, T(0));
		break;
	case 1:
		out().assign(1u, 1u, values().size(), 1u, 1u, T(0));
		break;
	case 2:
		out().assign(1u, 1u, 1u, values().size(), 1u, T(0));
		break;
	default:
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t"
			<< "invalid mask direction: " << dir() << "\n\t"
			<< "please select value in 0-2";
		throw std::out_of_range(msg.str().c_str());
	}
	T norm = 1;
	if(normalize()) {
		norm = 0;
		for (typename std::vector<T>::size_type ii=0; ii < values().size(); ii++)
			norm += std::abs(values()[ii]);
	}
	for (typename std::vector<T>::size_type ii=0; ii < values().size(); ii++)
		out()[0][ii] = values()[ii]/norm;
}

#endif // MASK1D_HXX_
