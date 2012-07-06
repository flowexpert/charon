/*  Copyright (C) 2009 René Steinbrügge

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
/// @file LocalConstant.hxx
/// implements class LocalConstant
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _MotionModels_LocalConstant_hxx_
#define _MotionModels_LocalConstant_hxx_

#include "LocalConstant.h"
template<class T>
std::set<std::string>& MotionModels::LocalConstant<T>::getUnknowns()
{
	this->unknowns.insert("a1");
	this->unknowns.insert("a2");
	if (dz.connected())
		this->unknowns.insert("a3");
	return this->unknowns;
}

template<class T>
void MotionModels::LocalConstant<T>::compute(
		const Point4D<int>& p, const int& v,
		std::map<std::string, T>& term, T& rhs,
		const std::string& unknown)
{
	static const bool is3D = dz.connected();
	if(!is3D)
		assert(p.z == 0u); // 2D only

	const T& iX = this->dx()(v, p.x, p.y, p.z, p.t);
	const T& iY = this->dy()(v, p.x, p.y, p.z, p.t);
	const T& iZ = dz.connected() ? this->dz()(v, p.x, p.y, p.z, p.t) : T(0);
	const T& iT = this->dt()(v, p.x, p.y, p.z, p.t);
	T factor = T(1);

	// multiply with derivative wrt unkown, if any unknown is given
	if (unknown.length()) {
		if (unknown == "a1")
			factor = iX;
		else if (unknown == "a2")
			factor = iY;
		else if (is3D && unknown == "a3")
			factor = iZ;
		else {
			std::ostringstream msg;
			msg << __FILE__ << ":" << __LINE__ << std::endl;
			msg << "\tInvalid Unknown given!" << std::endl;
			msg << "\tGiven unknown: \"" << unknown << "\"";
			throw std::out_of_range(msg.str().c_str());
		}
	}

	// calculate values to return
	term["a1"] += factor * iX;
	term["a2"] += factor * iY;
	if (is3D)
		term["a3"] += factor * iZ;
	rhs -= factor * iT;
}

template<class T>
MotionModels::LocalConstant<T>::LocalConstant(const std::string& name) :
		MotionModel<T>::MotionModel("motionmodels_localconstant", name),
		dz(true, false)  // dz is optional, for handling 3D flows
{
	ParameteredObject::_addInputSlot(
			dx, "dx", "derivation in x", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			dy, "dy", "derivation in y", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			dz, "dz", "derivation in z (for 3D mode)", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			dt, "dt", "derivation in t", "CImgList<T>");
}
#endif
