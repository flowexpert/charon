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
/// @file LocalPlanarity.cpp
/// implements class LocalPlanarity
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _MotionModels_LocalPlanarity_hxx_
#define _MotionModels_LocalPlanarity_hxx_

#include "LocalPlanarity.h"

using namespace std;
using namespace cimg_library;
using namespace MotionModels;

virtual std::set<std::string>& getUnknowns()
{
	this->unknowns.insert("a1");
	this->unknowns.insert("a2");
	this->unknowns.insert("b11");
	this->unknowns.insert("b12");
	this->unknowns.insert("b21");
	this->unknowns.insert("b22");
	this->unknowns.insert("c1");
	this->unknowns.insert("c2");
	if (is3d) {
		this->unknowns.insert("a3");
		this->unknowns.insert("b13");
		this->unknowns.insert("b23");
		this->unknowns.insert("b31");
		this->unknowns.insert("b32");
		this->unknowns.insert("b33");
		this->unknowns.insert("c3")
	}
	return this->unknowns;
}

template <class T>
void LocalPlanarity::compute(	const int xs, const int ys, const int zs,
								const int t, const int v,
								std::map<std::string, T>& term,
								T& rhs)
{
	if (is3d)
	{
//		assert(ret.width == 16);
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
//			(zs-z)*dz()(t,xs,ys,zs,v), //b33
//			(xs-x)*(xs-x)*dx()(t,xs,ys,zs,v) + (xs-x)*(ys-y)*dy()(t,xs,ys,zs,v) + (xs-x)*(zs-z)*dy()(t,xs,ys,zs,v),
//			(ys-y)*(xs-x)*dx()(t,xs,ys,zs,v) + (ys-y)*(ys-y)*dy()(t,xs,ys,zs,v) + (ys-y)*(zs-z)*dy()(t,xs,ys,zs,v),
//			(zs-z)*(xs-x)*dx()(t,xs,ys,zs,v) + (zs-z)*(ys-y)*dy()(t,xs,ys,zs,v) + (zs-z)*(zs-z)*dy()(t,xs,ys,zs,v) 
//			).transpose();
		rhs += dt()(t,xs,ys,zs,v);
		term["a1"]  += dx()(t,xs,ys,zs,v);
		term["a2"]  += dy()(t,xs,ys,zs,v);
		term["a3"]  += dz()(t,xs,ys,zs,v);
		term["b11"] += (xs-x)*dx()(t,xs,ys,zs,v);
		term["b12"] += (ys-y)*dx()(t,xs,ys,zs,v);
		term["b13"] += (zs-z)*dx()(t,xs,ys,zs,v);
		term["b21"] += (xs-x)*dy()(t,xs,ys,zs,v);
		term["b22"] += (ys-y)*dy()(t,xs,ys,zs,v);
		term["b23"] += (zs-z)*dy()(t,xs,ys,zs,v);
		term["b31"] += (xs-x)*dz()(t,xs,ys,zs,v);
		term["b32"] += (ys-y)*dz()(t,xs,ys,zs,v);
		term["b33"] += (zs-z)*dz()(t,xs,ys,zs,v);
		term["c1"]  += (xs-x)*(xs-x)*dx()(t,xs,ys,zs,v)
						+(xs-x)*(ys-y)*dy()(t,xs,ys,zs,v)
						+(xs-x)*(zs-z)*dy()(t,xs,ys,zs,v);
		term["c2"]  += (ys-y)*(xs-x)*dx()(t,xs,ys,zs,v)
						+(ys-y)*(ys-y)*dy()(t,xs,ys,zs,v)
						+(ys-y)*(zs-z)*dy()(t,xs,ys,zs,v);
		term["c3"]  += (zs-z)*(xs-x)*dx()(t,xs,ys,zs,v)
						+(zs-z)*(ys-y)*dy()(t,xs,ys,zs,v)
						+(zs-z)*(zs-z)*dy()(t,xs,ys,zs,v);		
	}
	else
	{
//		assert(ret.width == 9);
//		ret = CImg<>::vector(
//			dt()(0,xs,ys,t,v),
//			dx()(0,xs,ys,t,v),
//			dy()(0,xs,ys,t,v),
//			(xs-x)*dx()(0,xs,ys,t,v),
//			(ys-y)*dx()(0,xs,ys,t,v),
//			(xs-x)*dy()(0,xs,ys,t,v),
//			(ys-y)*dy()(0,xs,ys,t,v),
//			(xs-x)*(xs-x)*dx()(0,xs,ys,t,v) + (xs-x)*(ys-y)*dy()(0,xs,ys,t,v),
//			(xs-x)*(ys-y)*dx()(0,xs,ys,t,v) + (ys-y)*(ys-y)*dy()(0,xs,ys,t,v)
//			).transpose();
		rhs         += dt()(0,xs,ys,t,v);
		term["a1"]  += dx()(0,xs,ys,t,v);
		term["a2"]  += dy()(0,xs,ys,t,v);
		term["b11"] += (xs-x)*dx()(0,xs,ys,t,v);
		term["b12"] += (ys-y)*dx()(0,xs,ys,t,v);
		term["b21"] += (xs-x)*dy()(0,xs,ys,t,v);
		term["b22"] += (ys-y)*dy()(0,xs,ys,t,v);
		term["c1"]  += (xs-x)*(xs-x)*dx()(0,xs,ys,t,v)
						+(xs-x)*(ys-y)*dy()(0,xs,ys,t,v);
		term["c2"]  += (xs-x)*(ys-y)*dx()(0,xs,ys,t,v)
						+(ys-y)*(ys-y)*dy()(0,xs,ys,t,v);
	}
}

LocalPlanarity::LocalPlanarity(const string& name)
		: flowfunc() , MotionModel("motionmodels_localplanarity",name)
{
	_addInputSlot(dx,"dx","derivation in x","CImgList");
	_addInputSlot(dy,"dy","derivation in y","CImgList");
	_addInputSlot(dz,"dz","derivation in z","CImgList");
	_addInputSlot(dt,"dt","derivation in t","CImgList");
	
	this->flowFunctor = &flowfunc;
	
	//			    2d  2d	2d     2d            2d    2d
	this->setFlowFunctorParams(0.5,0.4,0.3,0.001,-0.002,0.0015,0.003,-0.001,-0.0008,0.0012,0.0001,0.0021,
				   0.0001,-0.00007,0.00003);
	//			    2d	     2d
}

void LocalPlanarity::setFlowFunctorParams(
						const float a1,  const float a2,  const float a3,
						const float b11, const float b12, const float b13,
						const float b21, const float b22, const float b23,
						const float b31, const float b32, const float b33,
						const float c1,  const float c2,  const float c3)
{
	flowfunc.setParams3d(a1,a2,a3,b11,b12,b13,b21,b22,b23,b31,b32,b33,c1,c2,c3);
}
void apply(const std::vector<std::string> &, 
           cimg_library::CImg<T>& image)
{

}

#endif
