/*  Copyright (C) 2009 Heidelberg Collaboratory for Image Processing

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file Stencil.hxx
 *  Implementation of Stencil interfaces.
 *  \author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 8.09.2009
 */


#ifndef _STENCIL_HXX_
#define _STENCIL_HXX_

#include "Stencil.h"
#include "Substencil.hxx"
#include <charon-core/ParameteredObject.hxx>

template <typename T>
Stencil::Base<T>::Base(
		const std::string& classname, const std::string& name,
		const std::string& doc) :
		TemplatedParameteredObject<T>(classname,name,doc +
				"<br><br>This class is derived from class Stencil::Base.<br>"
				"An base stencil is a universal stencil that can be"
				"enhanced by adding interfaces to it."),
		lambdaMask(true,false)
{
	ParameteredObject::_addInputSlot(
				lambdaMask, "lambdaMask", "lambda coefficient mask", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
				out,"this","Pointer to itself","Stencil<T>*");
	ParameteredObject::_addParameter(
				lambda,"lambda","weight of the pde term",T(1),"T");
}

template <typename T>
void Stencil::Base<T>::execute() {
	out() = this;
}

template <typename T>
Stencil::Energy<T>::Energy() :
	Stencil::Base<T>("Energy","","This should not be visible.") {
	_addFunction(Stencil::Energy<T>::getEnergy);
}

template <typename T>
Stencil::EnergyGradient<T>::EnergyGradient() :
	Stencil::Base<T>("EnergyGradient","","This should not be visible.") {
	_addFunction(Stencil::EnergyGradient<T>::getEnergyGradient);
	_addFunction(Stencil::EnergyGradient<T>::getEnergyGradientDimensions);
}

template <typename T>
Stencil::EnergyHessian<T>::EnergyHessian() :
	Stencil::Base<T>("EnergyHessian","","This should not be visible.") {
	_addFunction(Stencil::EnergyHessian<T>::getEnergyHessian);
}

template <typename T>
Stencil::Mask<T>::Mask() :
	Stencil::Base<T>("Mask","","This should not be visible.") {
	_addFunction(Stencil::Mask<T>::get);
	_addFunction(Stencil::Mask<T>::getRhs);
	_addFunction(Stencil::Mask<T>::getUnknowns);
	_rhs = 0;
}

template <typename T>
const std::map<std::string, SubStencil<T> >& Stencil::Mask<T>::get() const {
	return _subStencils;
}

template <typename T>
const T& Stencil::Mask<T>::getRhs() const {
	return _rhs;
}

template <typename T>
const std::set<std::string>& Stencil::Mask<T>::getUnknowns() const {
	return _unknowns;
}

#endif //_stencil_HXX_

