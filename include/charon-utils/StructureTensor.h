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
/// \file StructureTensor.h
/// Declaration of the parameter class StructureTensor
/// \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// \date 06.10.2009

#ifndef STRUCTURE_TENSOR_H_
#define STRUCTURE_TENSOR_H_

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

#ifdef HANDLE_DLL
#ifdef structuretensor_EXPORTS
/// DLL handling
#define structuretensor_DECLDIR __declspec(dllexport)
#else
#define structuretensor_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// No DLL handling
#define structuretensor_DECLDIR
#endif

/// Structure Tensor Calculation
/** This module calculates the structur tensor, its eigenvectors and
 *  eigenvalues using CImg functions.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template <typename T>
class structuretensor_DECLDIR StructureTensor :
		public TemplatedParameteredObject<T> {
public:
	/// standart constructor
	/// \param name		instance name
	StructureTensor(const std::string& name = "");

	/// image input
	InputSlot<cimg_library::CImgList<T> > in;
	/// self pointer
	OutputSlot<StructureTensor*> out;
	/// result output
	OutputSlot<cimg_library::CImgList<T> > tensor;
	/// eigenvalues output
	OutputSlot<cimg_library::CImgList<T> > eigenvalues;
	/// eigenvectors output
	OutputSlot<cimg_library::CImgList<T> > eigenvectors;

	/// central scheme
	Parameter<bool> centralScheme;

	/**	Calculate structure tensor and perform eigenvector analysis.
	 *
	 *	The structure tensor is calculated in the xyz dimensions.
	 *	The results are stored in the output List items, make sure that the
	 *	input list contains only one item.
	 *	This uses the built-in CImg routines.
	 *
	 *  \copydetails ParameteredObject::execute()
	 */
	virtual void execute();
};

#endif // STRUCTURE_TENSOR_H_

