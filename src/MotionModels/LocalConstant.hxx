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

/*
 void MotionModels::LocalConstant::calculateDerivatives()
 {
 this->deriv()->getX((this->img),dx);
 this->deriv()->getY((this->img),dy);
 this->deriv()->getZ((this->img),dt);
 } //*/

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
		const int xs, const int ys, const int zs, const int t, const int v,
		std::map<std::string, T>& term, T& rhs,
		const std::string& unknown)
{
	if(!dz.connected())
		assert(zs == 0u); // 2D only

	T values[4u] = {
		this->dx()(v, xs, ys, zs, t),                          // I_x
		this->dy()(v, xs, ys, zs, t),                          // I_y
		dz.connected() ? this->dz()(v, xs, ys, zs, t) : T(0),  // I_z
		this->dt()(v, xs, ys, zs, t)                           // I_t
	};
	if (unknown.length()) {
		T factor = T(1);
		if (unknown == "a1")
			factor = values[0];
		else if (unknown == "a2")
			factor = values[1];
		else if (dz.connected() && unknown == "a3")
			factor = values[2];
		else {
			std::ostringstream msg;
			msg << __FILE__ << ":" << __LINE__ << std::endl;
			msg << "\tInvalid Unknown given!" << std::endl;
			msg << "\tGiven unknown: \"" << unknown << "\"";
			throw std::out_of_range(msg.str().c_str());
		}
		for(unsigned int i=0; i<4u; i++)
			values[i] *= factor;
	}                            //    (optional - if unknown set)
	term["a1"] += values[0];     // +I_x (I_u)
	term["a2"] += values[1];     // +I_y (I_u)
	if (dz.connected())
		term["a3"] += values[2]; // +I_z (I_u)
	rhs -= values[3];            // -I_t (I_u)
}

template<class T>
void MotionModels::LocalConstant<T>::computeD(
		const int xs, const int ys, const int zs, const int t, const int v,
		std::map<std::string, T>& termD, T& rhsD,
		const std::string& /*unknown*/)
{
	compute(xs, ys, zs, t, v, termD, rhsD, "");
}

template<class T>
MotionModels::LocalConstant<T>::LocalConstant(const std::string& name) :
		MotionModel<T>::MotionModel("motionmodels_localconstant", name),
		flowfunc(),
		dz(true, false)  // dz is optional, for handling 3D flows
{
	_addInputSlot(dx, "dx", "derivation in x", "CImgList<T>");
	_addInputSlot(dy, "dy", "derivation in y", "CImgList<T>");
	_addInputSlot(dz, "dz", "derivation in z (for 3D mode)", "CImgList<T>");
	_addInputSlot(dt, "dt", "derivation in t", "CImgList<T>");

	this->setFlowFunctorParams(.5f, .4f, .3f);
	this->flowFunctor = &flowfunc;
}

template<class T>
void MotionModels::LocalConstant<T>::setFlowFunctorParams(const float a1,
		const float a2, const float a3)
{
	flowfunc.setParams3d(a1, a2, a3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0);
}

template<class T>
void MotionModels::LocalConstant<T>::apply(const Pixel<T> & inPixel,
		const std::vector<IncrementorParameter<T>*> & /*modifier*/,
		Pixel<T> & outPixel)
{
	outPixel = inPixel;
	//no change in movement
}

#endif
