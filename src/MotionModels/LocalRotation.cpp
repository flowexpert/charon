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
/// @file LocalRotation.cpp
/// implements class LocalRotation
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "LocalRotation.h"

using namespace std;
using namespace cimg_library;

using namespace MotionModels;

virtual std::set<std::string>& getUnknowns()
{
	this->unknowns.insert("a1");
	this->unknowns.insert("a2");
	this->unknwons.insert("s1");
	this->unknowns.insert("s2");
	if (!(dx().size == 1)) {
		this->unknowns.insert("a3");
		this->unknowns.insert("s3");
		this->unknowns.insert("s4");
	}
	return this->unknowns;
}

template <class T>
void MotionModels::LocalRotation::compute(
							const int xs, const int ys, const int zs,
							const int t, const int v,
							std::map<std::string, T>& term,
							T& rhs)
{
	if (dx().size == 1) //2D
	{
//		assert(ret.width == 5);
//		ret = CImg<>::vector(
//			dt()(0,xs,ys,t,v),
//			dx()(0,xs,ys,t,v),
//			dy()(0,xs,ys,t,v),
//			(xs-x)*dx()(0,xs,ys,t,v)+(ys-y)*dy()(0,xs,ys,t,v),
//			(ys-y)*dx()(0,xs,ys,t,v)-(xs-x)*dy()(0,xs,ys,t,v)
//			).transpose();
		rhs         += dt()(0,xs,ys,t,v);
		term["a1"]  += dx()(0,xs,ys,t,v);
		term["a2"]  += dy()(0,xs,ys,t,v);
		term["s1"]  += (xs-x)*dx()(0,xs,ys,t,v)+(ys-y)*dy()(0,xs,ys,t,v);
		term["s2"]  += (ys-y)*dx()(0,xs,ys,t,v)-(xs-x)*dy()(0,xs,ys,t,v);
	}
	else	//3D
	{
//		assert(ret.width == 8);
//		ret = CImg<>::vector(
//			dt()(t,xs,ys,zs,v),
//			dx()(t,xs,ys,zs,v),
//			dy()(t,xs,ys,zs,v),
//			dz()(t,xs,ys,zs,v),
//			(xs-x)*dx()(t,xs,ys,zs,v)+(ys-y)*dy()(t,xs,ys,zs,v)+(zs-z)*dz()(t,xs,ys,zs,v),
//			(ys-y)*dx()(t,xs,ys,zs,v)-(xs-x)*dy()(t,xs,ys,zs,v),
//			(zs-z)*dx()(t,xs,ys,zs,v)-(xs-x)*dz()(t,xs,ys,zs,v),
//			(zs-z)*dy()(t,xs,ys,zs,v)-(ys-y)*dz()(t,xs,ys,zs,v)
//			).transpose();
		rhs         += dt()(t,xs,ys,zs,v);
		term["a1"]  += dx()(t,xs,ys,zs,v);
		term["a2"]  += dy()(t,xs,ys,zs,v);
		term["a3"]  += dz()(t,xs,ys,zs,v);
		term["s1"]  += (xs-x)*dx()(t,xs,ys,zs,v)
						+(ys-y)*dy()(t,xs,ys,zs,v)
						+(zs-z)*dz()(t,xs,ys,zs,v);
		term["s2"]  += (ys-y)*dx()(t,xs,ys,zs,v)-(xs-x)*dy()(t,xs,ys,zs,v);
		term["s3"]  += (zs-z)*dx()(t,xs,ys,zs,v)-(xs-x)*dz()(t,xs,ys,zs,v);
		term["s4"]  += (zs-z)*dy()(t,xs,ys,zs,v)-(ys-y)*dz()(t,xs,ys,zs,v);
	}
}

LocalRotation::LocalRotation(const string& name) 
		: flowfunc(), MotionModel("motionmodels_localrotation",name)
{
	_addInputSlot(dx,"dx","derivation in x","CImgList");
	_addInputSlot(dy,"dy","derivation in y","CImgList");
	_addInputSlot(dz,"dz","derivation in z","CImgList");
	_addInputSlot(dt,"dt","derivation in t","CImgList");
	
	
	this->flowFunctor = &flowfunc;
				// 2d  2d	2d	2d
	this->setFlowFunctorParams(0.5,0.4,0.3,0.001,-0.002,0.0008,-0.0012);
}

void LocalRotation::setFlowFunctorParams(
					const float a1, const float a2, const float a3,
					const float s1, const float s2, const float s3, const float s4)
{
	flowfunc.setParams3d(a1,a2,a3,s1,s2,s3,-s2,s1,s4,-s3,-s4,s1,0.0,0.0,0.0);
}
void apply(const std::vector<std::string> &, 
           cimg_library::CImg<T>& image)
{

}

