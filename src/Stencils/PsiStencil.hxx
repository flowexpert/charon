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

template <class T>
PsiStencil<T>::PsiStencil(const std::string& name) :
		Stencil<T>("PsiStencil", name,
			"Wrapper stencil applying robustness terms")
{
	this->_addInputSlot(stencilIn, "stencilIn",
			"Input slot for stencil","Stencil<T>*");
	this->_addOutputSlot(flowListIn, "flowListIn",
			"CImgList containing the flow","CImgList<T>");
	this->_addInputSlot(robustnessTermIn, "robustnessTerm",
			"Input slot for the Robustness Term", "RobustnessTerm*");
}

template <class T>
void PsiStencil<T>::execute() {
	ParameteredObject::execute();
	//...
}

template <class T>
void PsiStencil<T>::updateStencil(
		const std::string& unknown,
		const unsigned int x,
		const unsigned int y,
		const unsigned int z,
		const unsigned int t,
		const unsigned int v) {
	std::map<std::string, T> newTerm;
	
	this->_rhs = 0;
	T _rhsD = 0;
	T _newRhs = 0;

	//update stencilIn
	stencilIn()->updateStencil(unknown, x, y, z, t, v);

	// get term and rhs for D'
	const std::map<std::string, T>& term = stencilIn()->getTerm();
	this->_rhs = stencilIn()->getRhs();
	// get term and rhs for D
	const std::map<std::string, T>& termD = stencilIn()->getTermD();
	_rhsD = stencilIn()->getRhsD();

	// get values of unknowns u,v,w
	const cimg_library::CImgList<T>& flowList = this->flowListIn();
	
	// calculate D
	// TODO: handle 2D case (no "a3" present!)
	T _D = pow((termD.find("a1")->second * flowList[0](x,y,z,t) +
				termD.find("a2")->second * flowList[1](x,y,z,t) +
				termD.find("a3")->second * flowList[2](x,y,z,t) +
				this->_rhsD), 2 );
	T _dD = term.find("a1")->second * flowList[0](x,y,z,t) +
			term.find("a2")->second * flowList[1](x,y,z,t) +
			term.find("a3")->second * flowList[2](x,y,z,t) +
			this->_rhs;
	

	// set parameter for robustness term
	robustnessTermIn()->setE(0.001);
	// calculate Psi'(D)*D' and return as stencilOut
	// ATTENTION: out() is a pointer to the stencil. This should always
	// be equal to "this". Never assign anything else to this->out()!
	// this->out() = DPsi(_D,0.002)*_dD;
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
