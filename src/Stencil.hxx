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
/** @file Stencil.hxx
 *  Implementation of class Stencil.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */


#ifndef _STENCIL_HXX_
#define _STENCIL_HXX_

#include "Stencil.h"
#include "Substencil.hxx"
#include <ParameteredObject.hxx>

template <class T>
Stencil<T>::Stencil(const std::string& classname, const std::string& name) : 
				TemplatedParameteredObject<T>(classname,name,
				"discretizes partial differential equation terms or defines derivatives filters for images")
{
	this->_addOutputSlot(out,"this","Pointer to itself","Stencil<T>*");
	this->_addParameter(lambda,"lambda","weight of the pde term",T(1),"T");
	_addFunction(Stencil<T>::get);
	_addFunction(Stencil<T>::getRhs);
	out = this;
}


template <class T>
const std::map<std::string, SubStencil<T> >& Stencil<T>::get() const {
	return this->substencils;
}

template <class T>
const std::map<std::string, T>& Stencil<T>::getRhs() const {
	return rhs;
}

template <class T>
const std::set<std::string>& Stencil<T>::getUnknowns() const {
	return unknowns;
}

template <class T>
Stencil<T>::~Stencil() {
}

#endif //_stencil_HXX_
