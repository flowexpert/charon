/*  Copyright (C) 2012 Helen Morrison

	This file is part of Charon.

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
/** \file Diffusion.hxx
 *  Implementation of the parameter class Diffusion.
 *  \author <a href="mailto:helen.morrison@iwr.uni-heidelberg.de">
 *      Helen Morrison</a>
 *  \date 27.03.2012
 *
 */

#ifndef _BrightnessModels_Diffusion_HXX_
#define _BrightnessModels_Diffusion_HXX_

#include "Diffusion.h"

template<class T>
BrightnessModels::Diffusion<T>::Diffusion(const std::string& name) :
		BrightnessModel<T>("brightnessmodels_diffusion", name)
{
	_addInputSlot(dx, "dx", "derivation in x", "CImgList<T>");
	_addInputSlot(dy, "dy", "derivation in y", "CImgList<T>");
	_addInputSlot(dxx, "dxx", "second derivation in x", "CImgList<T>");
	_addInputSlot(dyy, "dyy", "second derivation in y", "CImgList<T>");

	_addFunction(BrightnessModels::Diffusion<T>::getUnknowns);
	_addFunction(BrightnessModels::Diffusion<T>::compute);

	// nasty quick fix, forces constructor to be compiled into
	// shared object file, without: optimized away (perhaps too simple)
	if (!this) {
		sout << "dummy call, you should never see this" << std::endl;
	}

}

template<class T>
void BrightnessModels::Diffusion<T>::execute() {
	ParameteredObject::execute();
}

template<class T>
std::set<std::string>& BrightnessModels::Diffusion<T>::getUnknowns()
{
	return this->unknowns;
}

template<class T>
void BrightnessModels::Diffusion<T>::compute(
		const Point4D<int>& p, const int& v,
		std::map<std::string, T>& term, T& rhs,
		const std::string& unknown)
{

	const T& iX = this->dx()(v, p.x, p.y, p.z, p.t);
	const T& iY = this->dy()(v, p.x, p.y, p.z, p.t);
	const T& iXX = this->dxx()(v, p.x, p.y, p.z, p.t);
	const T& iYY = this->dyy()(v, p.x, p.y, p.z, p.t);

	T factor = T(1);

	// multiply with derivative wrt unkown, if any unknown is given
	if (unknown.length()) {
		if (unknown == "a1")
			factor = iX;
		else if (unknown == "a2")
			factor = iY;
		else {
			std::ostringstream msg;
			msg << __FILE__ << ":" << __LINE__ << std::endl;
			msg << "\tInvalid Unknown given!" << std::endl;
			msg << "\tGiven unknown: \"" << unknown << "\"";
			throw std::out_of_range(msg.str().c_str());
		}
	}

	// calculate values to return
	rhs += factor * 0.005 * (iXX + iYY);
//	rhs += 0.005 * (iXX + iYY);
}

#endif /* _DIFFUSION_HXX_ */
