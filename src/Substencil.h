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
/** @file SubStencil.h
 *  Implementation of class Subtencil.
 *  This class is used to manage the pattern, the data and the center
 *  of a SubStencil.
 *  @see Stencil.h
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */


#ifndef _SUBSTENCIL_H_
#define _SUBSTENCIL_H_

#include <CImg.h>
#include "Point4D.h"

template <class T>
class SubStencil
{
	public:
		cimg_library::CImg<T> data;
		cimg_library::CImg<char> pattern;
		Point4D<unsigned int> center;
		
		///empty constructor (for use in std::map)
		SubStencil();
		
		///default constructor
		SubStencil(const unsigned int dimx, const unsigned int dimy,
		           const unsigned int dimz, const unsigned int dimt,
		           Point4D<unsigned int> center);
		
		///copy constructor
		SubStencil(const SubStencil &rhs);
		
		///assignment operator
		SubStencil& operator= (const SubStencil &rhs);
				
		~SubStencil();
};

#endif //_SUBSTENCIL_H_