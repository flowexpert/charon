/*  Copyright (C) 2010 Stephan Meister

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
/** \file BinOperatorCImg.h
 *  Declaration of the parameter class BinOperatorCImg.
 *  \author Stephan Meister
 *  \date 15.01.2012
 */

#ifndef _BINOPERATORCIMG_H_
#define _BINOPERATORCIMG_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef binoperatorcimg_EXPORTS
/// Visual C++ specific code
#define binoperatorcimg_DECLDIR __declspec(dllexport)
#else
#define binoperatorcimg_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define binoperatorcimg_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>


///Class to apply binary operation to CImgs
template <typename T>
class binoperatorcimg_DECLDIR BinOperatorCImg :
		public TemplatedParameteredObject<T> {
public:
	/// operation type
	Parameter < std::string > _opType ;

	/// second operand of operation of _withValue is true
	Parameter < T > _value ;

	/// use scalar as second input operand
	Parameter < bool > _withValue ;

	/// input image 1
	InputSlot < cimg_library::CImgList<T> > _in1;

	/// input image 1
	InputSlot < cimg_library::CImgList<T> > _in2;

	/// output image
	OutputSlot < cimg_library::CImgList<T> > _out;

	/// create a new BinOperatorCImg object
	/// \param name          Instance name
	BinOperatorCImg(const std::string& name);

protected:
	/// Update object.
	virtual void execute();
};

#endif // _DISP2DEPTH_H_



