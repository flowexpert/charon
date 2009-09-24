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
/// @file LocalAffiness.hxx
/// implements class LocalAffiness
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _MotionModels_LocalAffiness_hxx_
#define _MotionModels_LocalAffiness_hxx_

#include "LocalAffiness.h"

using namespace std;
using namespace cimg_library;

using namespace MotionModels;

template<class T>
std::set<std::string>& LocalAffiness<T>::getUnknowns()
{
	this->unknowns.insert("a1");
	this->unknowns.insert("a2");
	this->unknowns.insert("b11");
	this->unknowns.insert("b12");
	this->unknowns.insert("b21");
	this->unknowns.insert("b22");
	if (is3d)
	{
		this->unknowns.insert("a3");
		this->unknowns.insert("b31");
		this->unknowns.insert("b32");
		this->unknowns.insert("b33");
	}
	return this->unknowns;
}

template<class T>
void LocalAffiness<T>::compute(const int xs, const int ys, const int zs,
		const int t, const int v, std::map<std::string, T>& term, T& rhs)
{
	if (is3d) // 3D
	{
		//		assert(ret.width == 13);
		//		ret = CImg<>::vector(
		//			dt()(t,xs,ys,zs,v),
		//			dx()(t,xs,ys,zs,v),
		//			dy()(t,xs,ys,zs,v),
		//			dz()(t,xs,ys,zs,v),
		//			(xs-x)*dx()(t,xs,ys,zs,v), //b11
		//			(ys-y)*dx()(t,xs,ys,zs,v), //b12
		//			(zs-z)*dx()(t,xs,ys,zs,v), //b13
		//			(xs-x)*dy()(t,xs,ys,zs,v), //b21
		//			(ys-y)*dy()(t,xs,ys,zs,v), //b22
		//			(zs-z)*dy()(t,xs,ys,zs,v), //b23
		//			(xs-x)*dz()(t,xs,ys,zs,v), //b31
		//			(ys-y)*dz()(t,xs,ys,zs,v), //b32
		//			(zs-z)*dz()(t,xs,ys,zs,v) //b33
		//			).transpose();
		rhs += dt()(t, xs, ys, zs, v);
		term["a1"] += dx()(t, xs, ys, zs, v);
		term["a2"] += dy()(t, xs, ys, zs, v);
		term["a3"] += dz()(t, xs, ys, zs, v);
		term["b11"] += (xs - this->x) * dx()(t, xs, ys, zs, v);
		term["b12"] += (ys - this->y) * dx()(t, xs, ys, zs, v);
		term["b13"] += (zs - this->z) * dx()(t, xs, ys, zs, v);
		term["b21"] += (xs - this->x) * dy()(t, xs, ys, zs, v);
		term["b22"] += (ys - this->y) * dy()(t, xs, ys, zs, v);
		term["b23"] += (zs - this->z) * dy()(t, xs, ys, zs, v);
		term["b31"] += (xs - this->x) * dz()(t, xs, ys, zs, v);
		term["b32"] += (ys - this->y) * dz()(t, xs, ys, zs, v);
		term["b33"] += (zs - this->z) * dz()(t, xs, ys, zs, v);
	}
	else // 2D
	{
		//		assert(ret.width == 7);
		//		ret = CImg<>::vector(
		//			dt()(0,xs,ys,t,v),
		//			dx()(0,xs,ys,t,v),
		//			dy()(0,xs,ys,t,v),
		//			(xs-x)*dx()(0,xs,ys,t,v),
		//			(ys-y)*dx()(0,xs,ys,t,v),
		//			(xs-x)*dy()(0,xs,ys,t,v),
		//			(ys-y)*dy()(0,xs,ys,t,v)
		//			).transpose();
		term["rhs"] += dt()(0, xs, ys, t, v);
		term["a1"] += dx()(0, xs, ys, t, v);
		term["a2"] += dy()(0, xs, ys, t, v);
		term["b11"] += (xs - this->x) * dx()(0, xs, ys, t, v);
		term["b12"] += (ys - this->y) * dx()(0, xs, ys, t, v);
		term["b21"] += (xs - this->x) * dy()(0, xs, ys, t, v);
		term["b22"] += (ys - this->y) * dy()(0, xs, ys, t, v);
	}
}

template<class T>
LocalAffiness<T>::LocalAffiness(const string& name) :
	MotionModel<T>::MotionModel("motionmodels_localaffiness", name), flowfunc()
{
	_addInputSlot(dx, "dx", "derivation in x", "CImgList");
	_addInputSlot(dy, "dy", "derivation in y", "CImgList");
	_addInputSlot(dz, "dz", "derivation in z", "CImgList");
	_addInputSlot(dt, "dt", "derivation in t", "CImgList");

	this->flowFunctor = &flowfunc;
	//			    2d  2d	2d     2d            2d    2d
	this->setFlowFunctorParams(0.5, 0.4, 0.3, 0.001, -0.002, 0.0015, 0.003,
			-0.001, -0.0008, 0.0012, 0.0001, 0.0021);
}

template<class T>
void LocalAffiness<T>::setFlowFunctorParams(const float a1, const float a2,
		const float a3, const float b11, const float b12, const float b13,
		const float b21, const float b22, const float b23, const float b31,
		const float b32, const float b33)
{
	flowfunc.setParams3d(a1, a2, a3, b11, b12, b13, b21, b22, b23, b31, b32,
			b33, 0.0, 0.0, 0.0);
}

template<class T>
void MotionModels::LocalAffiness<T>::apply(const Pixel<T> & inPixel,
		const std::vector<IncrementorParameter<T>*> & modifier,
		Pixel<T> & outPixel)
{
	outPixel = inPixel;
	// no moving change
}

#endif
