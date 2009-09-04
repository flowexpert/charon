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
/// @file LocalStretch.cpp
/// implements class LocalStretch
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "LocalStretch.h"

using namespace std;
using namespace cimg_library;

using namespace MotionModels;


virtual std::set<std::string>& getUnknowns()
{
	if (!is3d) {
		this->unknowns.insert("a1");
		this->unknowns.insert("a2");
		this->unknowns.insert("s1");
		this->unknowns.insert("s2");
		return this->unknowns;
	} else {
		throw "MotionModels::LocalStretch has no 3D implementation yet";
	}
}

template <class T>
void LocalStretch::compute(	const int xs, const int ys, const int zs,
							const int t, const int v,
							std::map<std::string, T>& map,
							T& rhs)
{
	if (!is3d) //2D
	{
//		assert(ret.width == 5);
//		ret = CImg<>::vector(
//			dt()(0,xs,ys,t,v),
//			dx()(0,xs,ys,t,v),
//			dy()(0,xs,ys,t,v),
//			(xs-x)*dx()(0,xs,ys,t,v)-(ys-y)*dy()(0,xs,ys,t,v),
//			(ys-y)*dx()(0,xs,ys,t,v)+(xs-x)*dy()(0,xs,ys,t,v)
//			).transpose();
		rhs         += dt()(0,xs,ys,t,v);
		term["a1"]  += dx()(0,xs,ys,t,v);
		term["a2"]  += dy()(0,xs,ys,t,v);
		term["s1"]  += (xs-x)*dx()(0,xs,ys,t,v)-(ys-y)*dy()(0,xs,ys,t,v);
		term["s2"]  += (ys-y)*dx()(0,xs,ys,t,v)+(xs-x)*dy()(0,xs,ys,t,v);
	}
	else	//3D
		throw "MotionModels::LocalStretch has no 3D implementation yet";
}
	
LocalStretch::LocalStretch(const string& name)
		: flowfunc() , MotionModel("motionmodels_localstretch",name)
{
	_addInputSlot(dx,"dx","derivation in x","CImgList");
	_addInputSlot(dy,"dy","derivation in y","CImgList");
	_addInputSlot(dz,"dz","derivation in z","CImgList");
	_addInputSlot(dt,"dt","derivation in t","CImgList");
	
	
	this->flowFunctor = &flowfunc;
	this->setFlowFunctorParams(0.5,0.4,0.001,-0.002);
}

void LocalStretch::setFlowFunctorParams(float a1, float a2, float s1, float s2)
{
	flowfunc.setParams(a1,a2,s1,s2,s2,-s1,0.0,0.0);
}
void apply(const std::vector<std::string> &, 
           cimg_library::CImg<T>& image)
{

}

