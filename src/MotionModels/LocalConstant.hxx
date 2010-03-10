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
		const int xs, const int ys, const int zs, const int t, const int v,
		std::map<std::string, T>& term, T& rhs,
		const std::string& unknown)
{
	if(!dz.connected())
		assert(zs == 0u); // 2D only

	const T& iX = this->dx()(v, xs, ys, zs, t);
	const T& iY = this->dy()(v, xs, ys, zs, t);
	const T& iZ = dz.connected() ? this->dz()(v, xs, ys, zs, t) : T(0);
	const T& iT = this->dt()(v, xs, ys, zs, t);
	T factor = T(1);

	// multiply with derivative wrt unkown, if any unknown is given
	if (unknown.length()) {
		if (unknown == "a1")
			factor = iX;
		else if (unknown == "a2")
			factor = iY;
		else if (dz.connected() && unknown == "a3")
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
	if (dz.connected())
		term["a3"] += factor * iZ;
	rhs -= factor * iT;
}


template<class T>
void MotionModels::LocalConstant<T>::computeEnergy(
		const int xs, const int ys, const int zs, const int ts, const int vs,
		const cimg_library::CImgList<T> flowList, double& energy)
{
	if(!dz.connected())
		assert(zs == 0u); // 2D only

	// derivatives
	const T& iX = this->dx()(vs, xs, ys, zs, ts);
	const T& iY = this->dy()(vs, xs, ys, zs, ts);
	const T& iZ = dz.connected() ? this->dz()(vs, xs, ys, zs, ts) : T(0);
	const T& iT = this->dt()(vs, xs, ys, zs, ts);

	// flow components
	const T& u = flowList[0](xs,ys,zs,ts);
	const T& v = flowList[1](xs,ys,zs,ts);
	const T& w = dz.connected() ? flowList[2](xs,ys,zs,ts) : T(0);

	energy += pow(double(iX*u+iY*v+iZ+w-iT),2.);
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
