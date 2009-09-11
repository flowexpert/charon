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
/** @file Substencil.hxx
 *  Implementation of class Subtencil.
 *  This class is used to manage the pattern, the data and the center
 *  of a substencil.
 *  @see Stencil.hxx
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */


#ifndef _substencil_HXX_
#define _substencil_HXX_

#include "Substencil.h"

///default constructor
template <class T>
Substencil<T>::Substencil(const unsigned int dimx, const unsigned int dimy,
                          const unsigned int dimz, const unsigned int dimt,
                          Point4D center) {
    //extend CImg objects to given size and initialize all values with 0
	data.assign(dimx, dimy, dimz, dimt, 0);
	pattern.assign(dimx, dimy, dimz, dimt, 0);
	this->center = center;
}
		
///copy constructor
template <class T>
Substencil<T>::Substencil(const Substencil &rhs) {
	this->data = rhs.data;
	this->pattern = rhs.pattern;
	this->center = rhs.center;
}
		
///assignment operator
template <class T>
Substencil& Substencil<T>::operator= (const Substencil &rhs) {
	if (&rhs == this) {return *this;}
	this->data = rhs.data;
	this->pattern = rhs.pattern;
	this->center = rhs.center;
	return *this;
}
				
Substencil<T>::~Substencil();

#endif //_substencil_HXX_