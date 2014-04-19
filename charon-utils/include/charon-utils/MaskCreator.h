/*  Copyright (C) 2012 Gerald Mwangi

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
/** \file MaskCreator.h
 *  Declaration of the parameter class MaskCreator.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 13.11.2012
 */

#ifndef _MASKCREATOR_H_
#define _MASKCREATOR_H_

#ifdef _MSC_VER
#ifdef maskcreator_EXPORTS
/// Visual C++ specific code
#define maskcreator_DECLDIR __declspec(dllexport)
#else
#define maskcreator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define maskcreator_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>
#include "Roi.hxx"
/// Creates a binary mask
/** Creates a binary mask. One can optionally connect an input image to set
 *  the dimensions or the set them manually with the parameters. This plugin
 *  DOES NOT actually mask the image but only creates a binary mask. Use
 *  binoperator to apply the mask.
 */
template <typename T>
class maskcreator_DECLDIR MaskCreator :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	MaskCreator(const std::string& name = "");

	/// Roi to mask
	InputSlot< Roi<int>* > roi;
	/// image to get the dimensions. OPTIONAL
	InputSlot< vigra::MultiArray<5,T> > image;

	/// Binary mask
	OutputSlot< vigra::MultiArray<5,T> > out;

	/// Dimension in x direction. Overriden by image
	Parameter< int > size_x;
	/// Dimension in y direction. Overriden by image
	Parameter< int > size_y;
	/// Dimension in z direction. Overriden by image
	Parameter< int > size_z;
	/// Dimension in t direction. Overriden by image
	Parameter< int > sizeT;
	/// Dimension in v direction. Overriden by image
	Parameter< int > size_v;

	/// Set the roi to false, and the surrounding to true
	Parameter< bool > invert_roi;


protected:
	/// Update object.
	virtual void execute();
};

#endif // _MASKCREATOR_H_
