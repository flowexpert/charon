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
/** @file Gbcce.hxx
 *  Implementation of class Gbcce.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *  @date 8.09.2009
 */

#ifndef _GBCCE_HXX_
#define _GBCCE_HXX_

#include "../Stencil.hxx"
#include "Gbcce.h"

template <class T>
Gbcce<T>::Gbcce(const std::string& name) : 
		Stencil<T>("GBCCE", name) {
	this->_addInputSlot(brightnessIn,"brightnessmodel","Brightness Model","BrightnessModel<T>");
	this->_addInputSlot(motionIn,"motionmodel","Motion Model","MotionModel<T>");
}

template <class T>
void Gbcce<T>::execute() {
	ParameteredObject::execute();
	// erase the old set of unknowns
	this->_unknowns.clear();
	std::set<std::string> bmUnknowns = this->brightnessIn()->getUnknowns();
	std::set<std::string> mmUnknowns = this->motionIn()->getUnknowns();
	std::set<std::string>::iterator bmIt = bmUnknowns.begin();
	std::set<std::string>::iterator mmIt = mmUnknowns.begin();

	// Collision detection

	// go through the BrightnessModel-Vector of unknowns
	while (bmIt != bmUnknowns.end()) {
		// go through the MotionModel-Vector of unknowns
		while (mmIt != mmUnknowns.end()) {
			// and compare them
			if (*bmIt == *mmIt) {
				throw "name collision in unknowns";
			}
			mmIt++;
		} 				
		// reset the inner loop iterator
		mmIt = mmUnknowns.begin();
		bmIt++;
	}
	// if no collision is detected, merge both input vectors into the
	// unkowns set which was inherited from the Stencil class
	this->_unknowns.insert(bmUnknowns.begin(),bmUnknowns.end());
	this->_unknowns.insert(mmUnknowns.begin(),mmUnknowns.end());

	// expanding stencil-map to appropriate size and filling with dummy
	// values
	std::set<std::string>::iterator uIt;
	for(uIt=this->_unknowns.begin();uIt!=this->_unknowns.end();uIt++) {
		Point4D<unsigned int> center(0,0,0,0);
		SubStencil<T> entry(1,1,1,1,center);
		entry.pattern(0,0) = 1;
		this->_subStencils[*uIt] = entry;
	}
}

template <class T>
void Gbcce<T>::updateStencil(
		const std::string& unknown,
		const unsigned int x,
		const unsigned int y,
		const unsigned int z,
		const unsigned int t,
		const unsigned int v) {
	//std::map<std::string, T> term;
	//std::map<std::string, T> termD;
	this->_rhs = 0;
	this->_rhsD = 0;

	// collect unknowns
	const std::set<std::string>& mUnknowns = motionIn()->getUnknowns();
	const std::set<std::string>& bUnknowns = brightnessIn()->getUnknowns();
	std::set<std::string> allUnknowns;
	allUnknowns.insert(bUnknowns.begin(), bUnknowns.end());
	allUnknowns.insert(mUnknowns.begin(), mUnknowns.end());

	// initialize term for all unknowns
	std::set<std::string>::const_iterator unkIt;
	for (unkIt = allUnknowns.begin(); unkIt != allUnknowns.end(); unkIt++)
		term[*unkIt] = T(0);

	// compute term for D'
	this->brightnessIn()->compute(x, y, z, t, v, term, this->_rhs, unknown);
	this->motionIn()->compute(x, y, z, t, v, term, this->_rhs, unknown);

	// compute term for D
	this->brightnessIn()->computeD(x, y, z, t, v, termD, this->_rhsD, unknown);
	this->motionIn()->computeD(x, y, z, t, v, termD, this->_rhsD, unknown);

	// and fill into substencils
	typename std::map<std::string,T>::iterator termIt;
	for(termIt=term.begin();termIt!=term.end();termIt++) {
		const T val = termIt->second * this->lambda();
		this->_subStencils[termIt->first].data(0,0) = val;
	}
	this->_rhs *= this->lambda();
}



//not yet implemented
template <class T>
cimg_library::CImg<T> Gbcce<T>::apply(const cimg_library::CImgList<T>& seq,
                                      const unsigned int frame) const {
	return seq[frame];
}


template <class T>
Gbcce<T>::~Gbcce() {
}

#endif //_GBCCE_HXX_
