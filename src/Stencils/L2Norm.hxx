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
/** @file L2Norm.hxx
 *  Implementation of class L2Norm.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */


#ifndef _L2NORM_HXX_
#define _L2NORM_HXX_

#include "../Stencil.hxx"
#include "L2Norm.h"

template <class T>
L2Norm<T>::L2Norm(const std::string& name) :
		Stencil<T>("L2Norm", name) {
	this->_addParameter(dimensions, "dimensions", "Number of dimensions", 2u);
	this->_addParameter(pUnknowns, "unknowns", "List of unknowns");
	if (dimensions > 4) {
		throw std::out_of_range("invalid dimensions (too large)");
	}
}

template <class T>
void L2Norm<T>::execute() {
	ParameteredObject::execute();

	// Copy the unknowns from the Parameter list into the set, which was
	// inherited from the Stencil class
	std::vector<std::string>::iterator puIt;
	for(puIt=pUnknowns().begin() ; puIt!=pUnknowns().end() ; puIt++) {
		this->_unknowns.insert(*puIt);
	}

	//create masks in appropriate dimensions
	switch (dimensions) {
		case 1:
			//already has the correct size
			_dataMask.assign(3,1,1,1,0);
			_patternMask.assign(3,1,1,1,0);
			break;
		case 2:
			_dataMask.assign(3,3,1,1,0);
			_patternMask.assign(3,3,1,1,0);
			break;
		case 3:
			_dataMask.assign(3,3,3,1,0);
			_patternMask.assign(3,3,3,1,0);
			break;
		case 4:
			_dataMask.assign(3,3,3,3,0);
			_patternMask.assign(3,3,3,3,0);
			break;
	}

	//filling mask with values and setting the center
	if (dimensions == 1) {
		_dataMask(0) += T(-1 * this->lambda());
		_dataMask(1) += T( 2 * this->lambda());
		_dataMask(2) += T(-1 * this->lambda());

		_patternMask(0) = 1;
		_patternMask(1) = 1;
		_patternMask(2) = 1;

		_center.x=1u;
		_center.y=0u;
		_center.z=0u;
		_center.t=0u;
	} else if (dimensions == 2) {
		_dataMask(1,0) += T(-1 * this->lambda());
		_dataMask(0,1) += T(-1 * this->lambda());
		_dataMask(1,1) += T( 4 * this->lambda());
		_dataMask(2,1) += T(-1 * this->lambda());
		_dataMask(1,2) += T(-1 * this->lambda());

		_patternMask(1,0) = 1;
		_patternMask(0,1) = 1;
		_patternMask(1,1) = 1;
		_patternMask(2,1) = 1;
		_patternMask(1,2) = 1;

		_center.x=1u;
		_center.y=1u;
		_center.z=0u;
		_center.t=0u;
	} else if (dimensions == 3) {
		_dataMask(1,1,0) += T(-1 * this->lambda());
		_dataMask(1,0,1) += T(-1 * this->lambda());
		_dataMask(0,1,1) += T(-1 * this->lambda());
		_dataMask(1,1,1) += T( 6 * this->lambda());
		_dataMask(2,1,1) += T(-1 * this->lambda());
		_dataMask(1,2,1) += T(-1 * this->lambda());
		_dataMask(1,1,2) += T(-1 * this->lambda());

		_patternMask(1,1,0) = 1;
		_patternMask(1,0,1) = 1;
		_patternMask(0,1,1) = 1;
		_patternMask(1,1,1) = 1;
		_patternMask(2,1,1) = 1;
		_patternMask(1,2,1) = 1;
		_patternMask(1,1,2) = 1;

		_center.x=1u;
		_center.y=1u;
		_center.z=1u;
		_center.t=0u;
	} else if (dimensions == 4) {
		_dataMask(1,1,1,0) += T(-1 * this->lambda());
		_dataMask(1,1,0,1) += T(-1 * this->lambda());
		_dataMask(1,0,1,1) += T(-1 * this->lambda());
		_dataMask(0,1,1,1) += T(-1 * this->lambda());
		_dataMask(1,1,1,1) += T( 8 * this->lambda());
		_dataMask(2,1,1,1) += T(-1 * this->lambda());
		_dataMask(1,2,1,1) += T(-1 * this->lambda());
		_dataMask(1,1,2,1) += T(-1 * this->lambda());
		_dataMask(1,1,1,2) += T(-1 * this->lambda());

		_patternMask(1,1,1,0) = 1;
		_patternMask(1,1,0,1) = 1;
		_patternMask(1,0,1,1) = 1;
		_patternMask(0,1,1,1) = 1;
		_patternMask(1,1,1,1) = 1;
		_patternMask(2,1,1,1) = 1;
		_patternMask(1,2,1,1) = 1;
		_patternMask(1,1,2,1) = 1;
		_patternMask(1,1,1,2) = 1;

		_center.x=1u;
		_center.y=1u;
		_center.z=1u;
		_center.t=1u;
	}
}

template <class T>
void L2Norm<T>::updateStencil(
		const std::string& unknown,
		const unsigned int,
		const unsigned int,
		const unsigned int,
		const unsigned int,
		const unsigned int) {
	// fill stencil with masks
	std::vector<std::string>::iterator uIt;
	for(uIt=this->pUnknowns().begin() ; uIt!=this->pUnknowns().end() ; uIt++) {
		SubStencil<T> entry;
		if(*uIt == unknown) {
			entry.center  = _center;
			entry.data    = _dataMask;
			entry.pattern = _patternMask;
		}
		else {
			// empty substencil for other unknowns
			entry.center = Point4D<unsigned int>();
			entry.data.clear();
			entry.pattern.clear();
		}
		this->_subStencils[*uIt] = entry;
	}
}

template <class T>
cimg_library::CImg<T> L2Norm<T>::apply(
		const cimg_library::CImgList<T>& seq,
		const unsigned int frame) const {
	return seq[frame];
}

template <class T>
L2Norm<T>::~L2Norm() {}

#endif // _L2NORM_HXX_
