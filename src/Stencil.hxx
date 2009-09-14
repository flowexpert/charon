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
Stencil<T>::Stencil(const std::string& classname, const std::string& name = "") : 
				TemplatedParameteredObject<T>(classname,name,
				"discretizes partial differential equation terms or defines derivatives filters for images"),
				out(this) {
	this->_addOutputSlot(out,"this","Pointer to itself","Stencil<T>*");
	this->_addParameter(lambda,"lambda","weight of the pde term",1);
}

template <class T> 
virtual void Stencil<T>::updateStencil(const unsigned int x, const unsigned int y,
                                       const unsigned int z, const unsigned int t,
                                       const unsigned int v=0) {}

template <class T>
std::map<std::string, Substencil<T> >& Stencil<T>::get() const {return substencils;}

template <class T>
std::map<std::string, T>& Stencil<T>::getRhs() const {return rhs;}

template <class T>
virtual cimg_library::CImgList<T>& Stencil<T>::apply(const cimg_library::CImgList<T>& seq,
                                                     const unsigned int frame) const {}

template <class T>
virtual const std::set<std::string>& Stencil<T>::getUnknowns() const {return unknowns;}

template <class T>
virtual Stencil<T>::~Stencil() {}

#endif //_stencil_HXX_