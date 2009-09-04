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
/// @file LocalConstant.cpp
/// implements class LocalConstant
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "LocalConstant.h"

using namespace std;
using namespace cimg_library;

/*
void MotionModels::LocalConstant::calculateDerivatives()
{
	this->deriv()->getX((this->img),dx);
	this->deriv()->getY((this->img),dy);
	this->deriv()->getZ((this->img),dt);
} //*/


virtual std::set<std::string>& LocalConstant::getUnknowns()
{
	this->unknowns.insert("a1");
	this->unknowns.insert("a2");
	if (is3d) this->unknowns.insert("a3");
	return this->unknowns;
}

template <class T>
void MotionModels::LocalConstant::compute(	const int xs, const int ys, const int zs,
											const int t, const int v,
											std::map<std::string, T>& term,)
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
		rhs += dt()(t,xs,ys,zs,v);
		term["a1"]  += dx()(t,xs,ys,zs,v);
		term["a2"]  += dy()(t,xs,ys,zs,v);
		term["a3"]  += dz()(t,xs,ys,zs,v);
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
		rhs         += dt()(0,xs,ys,t,v);
		term["a1"]  += dx()(0,xs,ys,t,v);
		term["a2"]  += dy()(0,xs,ys,t,v);
	}
	//return r;
}

MotionModels::LocalConstant::LocalConstant(const string& name)
		: flowfunc() , MotionModel("motionmodels_localconstant",name)
{
	_addInputSlot(dx,"dx","derivation in x","CImgList");
	_addInputSlot(dy,"dy","derivation in y","CImgList");
	_addInputSlot(dz,"dz","derivation in z","CImgList");
	_addInputSlot(dt,"dt","derivation in t","CImgList");
	
	this->setFlowFunctorParams(0.5,0.4,0.3);
	this->flowFunctor = &flowfunc;
}

void MotionModels::LocalConstant::setFlowFunctorParams(const float a1, const float a2, const float a3)
{
	flowfunc.setParams3d(a1,a2,a3,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
}
void apply(const std::vector<std::string> &, 
           cimg_library::CImg<T>& image)
{

}

