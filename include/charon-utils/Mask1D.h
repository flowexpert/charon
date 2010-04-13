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
/// @file Mask1D.h
/// Declaration of the parameter class Mask1D
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @date 02.10.2009

#ifndef MASK1D_H_
#define MASK1D_H_

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

#ifdef HANDLE_DLL
#ifdef mask1d_EXPORTS
/// DLL handling
#define mask1d_DECLDIR __declspec(dllexport)
#else
#define mask1d_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// No DLL handling
#define mask1d_DECLDIR
#endif

/// generate simple 1D filter mask
template <typename T>
class mask1d_DECLDIR Mask1D : public TemplatedParameteredObject<T> {
public:
	/// standart constructor
	/// \param name		instance name
	Mask1D(const std::string& name = "");

	/// values to insert into mask
	ParameterList<T> values;
	/// select mask direction
	Parameter<unsigned int> dir;
	/// normalize (L1-Norm)
	Parameter<bool> normalize;
	/// mask output
	OutputSlot<cimg_library::CImgList<T> > out;

	/// copy values into mask
	/// \copydetails ParameteredObject::execute()
	virtual void execute();
};

#endif // MASK1D_H_
