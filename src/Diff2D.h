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
/** \file Diff2D.h
 *  Declaration of the parameter class Diff2D.
 *  \author Jens-Malte Gottfried
 *  \date 10.02.2010
 */

#ifndef _DIFF2D_H_
#define _DIFF2D_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef diff2d_EXPORTS
/// Visual C++ specific code
#define diff2d_DECLDIR __declspec(dllexport)
#else
#define diff2d_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define diff2d_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <CImg.h>

/// Calculate 2D derivatives.
/// 
/// This plugin serves to simplify calculations of 2D derivations.
/// Although derivatives may be calculated using class LinearFilter, this plugin
/// makes derivative calculations more compact.
/// Only the derivatives of connected output slots are calculated.
template <typename T>
class diff2d_DECLDIR Diff2D :
		public TemplatedParameteredObject<T> {
public:
	/// linear 1D filter mask used for derivation wrt x,y 
	Parameter < T > diffMaskXY;
	/// linear 1D filter mask applied orthogonal of derivation (wrt x,y) 
	Parameter < T > smoothMaskXY;
	/// linear 1D filter mask used for derivation wrt t 
	Parameter < T > diffMaskT;
	

	/// image input 
	InputSlot < CImgList<T> > img;
	/// if connected, calculations are performed on count change only 
	InputSlot < unsigned int > count;
	/// derivative wrt x 
	OutputSlot < CImgList<T> > dx;
	/// derivative wrt y 
	OutputSlot < CImgList<T> > dy;
	/// derivative wrt t 
	OutputSlot < CImgList<T> > dt;
	/// second derivative wrt x 
	OutputSlot < CImgList<T> > dxx;
	/// second derivative wrt x,y 
	OutputSlot < CImgList<T> > dxy;
	/// second derivative wrt y 
	OutputSlot < CImgList<T> > dyy;
	

	/// create a new Diff2D object
	/// \param name          Instance name
	Diff2D(const std::string& name);

	/// Update object.
	virtual void execute();
};

#endif // _DIFF2D_H_



