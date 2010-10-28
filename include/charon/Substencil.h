/*  This file is part of Charon.

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
/** @file Substencil.h
 *  Implementation of class Subtencil.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *  @date 8.09.2009
 */


#ifndef _SUBSTENCIL_H_
#define _SUBSTENCIL_H_

#include <CImg.h>
#include "Point4D.h"

/// Mask for one unknown or general mask if no unknowns are used
/** This class is used to manage the pattern, the data and the center
 *  of a SubStencil.
 *  \see class Stencil
 */
template <class T>
class SubStencil
{
public:
	///CImg containing the actual data of the SubStencil.
	cimg_library::CImg<T> data;

	/**
	 * CImg containing the pattern of the SubStencil.
	 * All positions that could potentially contain data are 1.
	 * Positions that will never contain data are 0.
	 */
	cimg_library::CImg<char> pattern;

	/// center of the substencil pattern
	Point4D<int> center;

	///default constructor.
	SubStencil();

	///constructor with dimensions and center.
	SubStencil(const unsigned int dimx, const unsigned int dimy,
			const unsigned int dimz, const unsigned int dimt,
			Point4D<int> center);

	///copy constructor.
	SubStencil(const SubStencil& rhs);

	///assignment operator.
	SubStencil& operator= (const SubStencil& rhs);

	///default destructor.
	~SubStencil();
};

#endif //_SUBSTENCIL_H_
