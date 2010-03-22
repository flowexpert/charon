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
/** @file PsiStencil.hxx
 *  Implementation of class PsiStencil.
 *  PsiStencil is
 *	derived from class Stencil.
 *  It is used to apply a robustness term on a stencil

 *  @author <a href="techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 3.03.2010
 */

#ifndef _PSISTENCIL_HXX_
#define _PSISTENCIL_HXX_

#include "../Stencil.hxx"
#include "PsiStencil.h"
#include <ParameteredObject.hxx>


template <class T>
PsiStencil<T>::PsiStencil(const std::string& name) :
		Stencil<T>("PsiStencil", name,
			"Wrapper stencil applying robustness terms")
{
	this->_addInputSlot(stencilIn, "stencilIn",
			"Input slot for stencil","Stencil<T>*");
	this->_addParameter(epsilon,"epsilon","parameter of the Robustness Term",0.001,"double");
	this->_addInputSlot(parameterListIn, "parameterListIn",
			"CImgList containing the parameters","CImgList<T>");
}

template <class T>
void PsiStencil<T>::execute() {
	ParameteredObject::execute();
	//...
}

template <class T>
void PsiStencil<T>::updateStencil(
		const std::string& unknown,
		const unsigned int& x,
		const unsigned int& y,
		const unsigned int& z,
		const unsigned int& t,
		const unsigned int& v) {
	
	// get values of unknowns 
	const cimg_library::CImgList<T>& parameterList = this->parameterListIn();

	// update stencil of stencilIn
	stencilIn()->updateStencil(unknown, x, y, z, t, v);

	// update energy of stencilIn
	stencilIn()->updateEnergy(parameterList, x, y, z, t, v);

	this->_rhs = stencilIn()->getRhs();
	this->_unknowns = stencilIn()->getUnknowns();
	this->lambda = stencilIn()->lambda;

	// get subStencils
	this->_subStencils = stencilIn()->get();
	
	double factor = DPsi(stencilIn()->getEnergy(),epsilon());

	this->_rhs*=factor;
	
	// calculate Psi'(D)*D' 
	this->_subStencils[unknown].data = this->_subStencils[unknown].data * factor;

}

template <class T>
void PsiStencil<T>::updateEnergy(
		const cimg_library::CImgList<T>&,
		const unsigned int&, const unsigned int&,
		const unsigned int&, const unsigned int&,
		const unsigned int&) {
}

// robustness term

template <class T>
double PsiStencil<T>::DPsi(double s, double e) const {
	double dpsi = s/(sqrt(pow(s, 2) + pow(e, 2)));
	return dpsi;
}

//not yet implemented
template <class T>
cimg_library::CImg<T> PsiStencil<T>::apply(
		const cimg_library::CImgList<T>& seq,
		const unsigned int frame) const {
	return seq[frame];
}

template <class T>
PsiStencil<T>::~PsiStencil() {
}

#endif //_PSISTENCIL_HXX_
