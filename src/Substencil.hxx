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
 *  Implementation of class Substencil.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */


#ifndef _SUBSTENCIL_HXX_
#define _SUBSTENCIL_HXX_

#include "Substencil.h"

template <class T>
SubStencil<T>::SubStencil() {
}

template <class T>
SubStencil<T>::SubStencil(const unsigned int dimx, const unsigned int dimy,
                          const unsigned int dimz, const unsigned int dimt,
                          Point4D<unsigned int> c) {
    //extend CImg objects to given size and initialize all values with 0
	data.assign(dimx, dimy, dimz, dimt, 0);
	pattern.assign(dimx, dimy, dimz, dimt, 0);
	center = c;
}
		
template <class T>
SubStencil<T>::SubStencil(const SubStencil<T> &rhs) {
	data.assign(rhs.data.width(), rhs.data.height(), rhs.data.depth(), rhs.data.spectrum() );
	data = rhs.data;
	pattern.assign(rhs.pattern.width(), rhs.pattern.height(), rhs.pattern.depth(), rhs.pattern.spectrum() );
	pattern = rhs.pattern;
	center = rhs.center;
}
		
template <class T>
SubStencil<T>& SubStencil<T>::operator= (const SubStencil<T> &rhs) {
	if (&rhs == this) {
		return *this;
	}
	data = rhs.data;
	pattern = rhs.pattern;
	center = rhs.center;
	return *this;
}

template <class T>
SubStencil<T>::~SubStencil() {
}

#endif //_SUBSTENCIL_HXX_
