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
	if (is3d)
		this->unknowns.insert("a3");
	return this->unknowns;
}

template<class T>
void MotionModels::LocalConstant<T>::compute(const int xs, const int ys,
		const int zs, const int t, const int v, std::map<std::string, T>& term,
		T& rhs)
{
	// 	cout << " HOP" << endl;
	// 	cout << (is3d? "ja" : "ne");
	// 	cout << ",xs " << xs;
	// 	cout << ",ys " << ys;
	// 	cout << ",zs " << zs;
	// 	cout << ",t " << t;
	// 	cout << ",v " << v;
	// 	cout << "HIP" << endl;
	if (is3d)
	{
		//cout  << "3d" << endl;
		//cout << "ret.width: " << ret.width << endl;
		//		assert(ret.width == 4);
		//		ret = CImg<>::vector(
		//			dt()(t,xs,ys,zs,v),
		//			dx()(t,xs,ys,zs,v),
		//			dy()(t,xs,ys,zs,v),
		//			dz()(t,xs,ys,zs,v)
		//			).transpose();
		rhs += (*this->dt())(v, xs, ys, zs, t);
		term["a1"] += (*this->dx())(v, xs, ys, zs, t);
		term["a2"] += (*this->dy())(v, xs, ys, zs, t);
		term["a3"] += (*this->dz())(v, xs, ys, zs, t);
	}
	else
	{
		// 		cout  << "2d" << endl;
		//
		// 		/*dt()[0].print("dt");
		// 		dx()[0].print("dx");
		// 		dy()[0].print("dy");
		// 		*/
		// 		dt().print("dt");
		// 		dx().print("dx");
		// 		dy().print("dy");
		//
		// 		cout << "HEP" <<endl;

		//		assert(ret.width == 3);
		//		ret = CImg<>::vector(
		//			dt()(0,xs,ys,t,v),
		//			dx()(0,xs,ys,t,v),
		//			dy()(0,xs,ys,t,v)
		//			).transpose();
		rhs += (*this->dt())(0, xs, ys, t, v);
		term["a1"] += (*this->dx())(0, xs, ys, t, v);
		term["a2"] += (*this->dy())(0, xs, ys, t, v);
	}
	//return r;
}

template<class T>
MotionModels::LocalConstant<T>::LocalConstant(const std::string& name) :
	MotionModel<T>::MotionModel("motionmodels_localconstant", name), flowfunc()
{
	_addInputSlot(dx, "dx", "derivation in x", "CImgList<T>*");
	_addInputSlot(dy, "dy", "derivation in y", "CImgList<T>*");
	_addInputSlot(dz, "dz", "derivation in z", "CImgList<T>*");
	_addInputSlot(dt, "dt", "derivation in t", "CImgList<T>*");

	this->setFlowFunctorParams(0.5, 0.4, 0.3);
	this->flowFunctor = &flowfunc;
}

template <class T>
void MotionModels::LocalConstant<T>::execute() {
	ParameteredObject::execute();
	this->is3d = !(this->dx()->is_sameN(1));
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
		const std::vector<IncrementorParameter<T>*> & modifier,
		Pixel<T> & outPixel)
{
	outPixel = inPixel;
	//no change in movement
}

#endif
