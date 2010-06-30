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
//	this->_addParameter(epsilon,"epsilon","parameter of the Robustness Term",0.001,"double");
	this->_addInputSlot(parameterListIn, "parameterListIn",
			"CImgList containing the parameters","CImgList<T>");
	this->_addInputSlot(robustnessTerm, "robustnessTerm",
			"containing the robustness term","RobustnessTerm*");

}

template <class T>
void PsiStencil<T>::execute() {
	ParameteredObject::execute();

	// erase the old set of unknowns
	this->_unknowns.clear();
	std::set<std::string> myUnknowns = this->stencilIn()->getUnknowns();
	this->_unknowns.insert(myUnknowns.begin(),myUnknowns.end());
	
	// expanding stencil-map to appropriate size and filling with
	// values from imported substencil
	std::set<std::string>::iterator uIt;
	for(uIt=this->_unknowns.begin();uIt!=this->_unknowns.end();uIt++) {
		stencilIn()->updateStencil(*uIt);
		this->_subStencils[*uIt] = stencilIn()->get().find(*uIt)->second;
	}
}

template <class T>
void PsiStencil<T>::updateStencil(
		const std::string& unknown,
		const Point4D<int>& p, const int& v) {


	// get values of unknowns 
	const cimg_library::CImgList<T>& parameterList = this->parameterListIn();

	// update stencil of stencilIn
	stencilIn()->updateStencil(unknown,p,v);
	stencilIn()->updateEnergy(parameterList,p,v);

	this->_rhs = stencilIn()->getRhs();
	this->lambda = stencilIn()->lambda;

	// update rhs
	const double factor = robustnessTerm()->DPsi(stencilIn()->getEnergy());//,epsilon());
	this->_rhs *= T(factor);

	// initialize term for all unknowns
	std::set<std::string>::const_iterator uIt;
	for (uIt = this->_unknowns.begin(); uIt != this->_unknowns.end(); uIt++){
		// transfer substencil data
		const SubStencil<T>& inSubStencil=stencilIn()->get().find(*uIt)->second;
		this->_subStencils[*uIt] = inSubStencil;
		this->_subStencils[*uIt].data *= factor;
	}
}

template <class T>
void PsiStencil<T>::updateEnergy(
		const cimg_library::CImgList<T>& flowList,
		const Point4D<int>& p, const int& v) {
}


// robustness term
/*
template <class T>
double PsiStencil<T>::DPsi(double s, double e) const {
	double dpsi = s/(sqrt(pow(s, 2) + pow(e, 2)));
	return dpsi;
}*/

//not yet implemented: check this!
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
