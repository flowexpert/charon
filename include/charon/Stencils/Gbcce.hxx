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
Gbcce<T>::Gbcce(const std::string& name) :  Stencil<T>("GBCCE", name,
			"Stencil modeling general brightness change constraint.<br>"
			"General means that it can be customized using different "
			"brightness and motion models."),
		mask(true, false), _bmIn(0), _mmIn(0)
{
	ParameteredObject::_addInputSlot(
			brightnessIn, "brightnessmodel",
			"Brightness Model","BrightnessModel<T>");
	ParameteredObject::_addInputSlot(
			motionIn, "motionmodel",
			"Motion Model", "MotionModel<T>");
	ParameteredObject::_addInputSlot(
			mask, "mask",
			"Mask input (precalculated strength)", "CImgList<T>");
}

template <class T>
void Gbcce<T>::execute() {
	Stencil<T>::execute();

	// erase the old set of unknowns
	this->_unknowns.clear();
	std::set<std::string> bmUnknowns = this->brightnessIn()->getUnknowns();
	std::set<std::string> mmUnknowns = this->motionIn()->getUnknowns();
	std::set<std::string>::iterator bmIt;
	std::set<std::string>::iterator mmIt;

	// Collision detection

	// check if there are some unknowns in bm and mm witch have the same name
	for (bmIt = bmUnknowns.begin(); bmIt != bmUnknowns.end(); bmIt++) {
		for (mmIt = mmUnknowns.begin(); mmIt != mmUnknowns.end(); mmIt++) {
			if (*bmIt == *mmIt) {
				std::ostringstream os;
				os << __FILE__ << ":" << __LINE__ << "\n\t";
				os << "Name collision in Gbcce::execute():\n\t\t";
				os << "\"" << *bmIt << "\" occurs in brightness and "
						<< "motion model";
				throw std::runtime_error(os.str());
			}
		}
	}

	// if no collision is detected, merge both input vectors into the
	// unkowns set which was inherited from the Stencil class
	this->_unknowns.insert(bmUnknowns.begin(),bmUnknowns.end());
	this->_unknowns.insert(mmUnknowns.begin(),mmUnknowns.end());

	// expanding stencil-map to appropriate size and filling with dummy
	// values
	std::set<std::string>::iterator uIt;
	for(uIt=this->_unknowns.begin();uIt!=this->_unknowns.end();uIt++) {
		Point4D<int> center;
		SubStencil<T> entry(1,1,1,1,center);
		entry.pattern(0,0) = 1;
		this->_subStencils[*uIt] = entry;
	}

	// refresh model cache
	_bmIn = this->brightnessIn();
	_mmIn = this->motionIn();
}

template <class T>
void Gbcce<T>::updateStencil(
		const std::string& unknown,
		const Point4D<int>& p,
		const int& v) {
	this->_rhs = 0;
	const T& l = this->lambda();

	// initialize term for all unknowns
	std::set<std::string>::const_iterator unkIt;
	static std::map<std::string, T> term;
	for (unkIt = this->_unknowns.begin();
			unkIt != this->_unknowns.end(); unkIt++)
		term[*unkIt] = T(0);

	// compute term
	_bmIn->compute(p,v,term,this->_rhs,unknown);
	_mmIn->compute(p,v,term,this->_rhs,unknown);

	T weight = T(1.);
	if (mask.connected())
		weight = mask().atNXYZC(v,p.x,p.y,p.z,p.t);

	// and fill into substencils
	typename std::map<std::string,T>::iterator termIt;
	for(termIt=term.begin();termIt!=term.end();termIt++) {
		const T val = termIt->second * l * weight;
		this->_subStencils[termIt->first].data(0,0) = val;
	}
	this->_rhs *= l * weight;
}

//not yet implemented
template <class T>
cimg_library::CImg<T> Gbcce<T>::apply(
		const cimg_library::CImgList<T>& seq,
		const unsigned int frame) const {
	return seq[frame];
}

#endif //_GBCCE_HXX_
