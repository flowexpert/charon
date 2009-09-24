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
/** @file SubStencil.hxx
 *  Implementation of class Subtencil.
 *  This class is used to manage the pattern, the data and the center
 *  of a SubStencil.
 *  @see Stencil.hxx
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */


#ifndef _SUBSTENCIL_HXX_
#define _SUBSTENCIL_HXX_

#include "Substencil.h"

template <class T>
SubStencil<T>::SubStencil() {}


///default constructor
template <class T>
SubStencil<T>::SubStencil(const unsigned int dimx, const unsigned int dimy,
                          const unsigned int dimz, const unsigned int dimt,
                          Point4D<unsigned int> center) {
    //extend CImg objects to given size and initialize all values with 0
	data.assign(dimx, dimy, dimz, dimt, 0);
	pattern.assign(dimx, dimy, dimz, dimt, 0);
	this->center = center;
}
		
///copy constructor
template <class T>
SubStencil<T>::SubStencil(const SubStencil<T> &rhs) {
	this->data.assign(rhs.data.dimx(), rhs.data.dimy(), rhs.data.dimz(), rhs.data.dimv() );
	this->data = rhs.data;
	this->pattern.assign(rhs.pattern.dimx(), rhs.pattern.dimy(), rhs.pattern.dimz(), rhs.pattern.dimv() );
	this->pattern = rhs.pattern;
	this->center = rhs.center;
}
		
///assignment operator
template <class T>
SubStencil<T>& SubStencil<T>::operator= (const SubStencil<T> &rhs) {
	if (&rhs == this) {return *this;}
	this->data = rhs.data;
	this->pattern = rhs.pattern;
	this->center = rhs.center;
	return *this;
}

template <class T>
SubStencil<T>::~SubStencil() {}

#endif //_SUBSTENCIL_HXX_