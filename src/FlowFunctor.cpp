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
/// @file FlowFunctor.cpp
/// implements class FlowFunctor
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "FlowFunctor.h"

using namespace std;
using namespace cimg_library;

void FlowFunctor::operator ()(const float x, const float y, float* f) const
{
	//if (!a1) throw "FlowFunctor has no 2D parameters, call setParams(...) first";
		f[0] = a1 + b11*x + b12*y + x*x*c1 + x*y*c2;
		f[1] = a2 + b21*x + b22*y + x*y*c1 + y*y*c2;
}

void FlowFunctor::get3d(const float x, const float y, const float z, float* f) const
{
	if (!is3d) throw "FlowFunctor has no 3D parameters, call setParams3d(...) first";
	f[0] = a1 + b11*x + b12*y + b13*z + x*x*c1 + x*y*c2 + x*z*c3;
	f[1] = a2 + b21*x + b22*y + b23*z + x*y*c1 + y*y*c2 + y*z*c3;
	f[2] = a3 + b31*x + b32*y + b33*z + x*z*c1 + y*z*c2 + z*z*c3;
}
/*
FlowFunctor::FlowFunctor(const float _a1, const float _a2, const float _b11, const float _b12, 
						 const float _b21, const float _b22, const float _c1, const float _c2) :
					a1(_a1), a2(_a2), b11(_b11), b12(_b12), 
					b21(_b21), b22(_b22),c1(_c1), c2(_c2)
					{}
*/

void FlowFunctor::setParams(const float a1, const float a2, const float b11, const float b12, const float b21,
			    const float b22, const float c1, const float c2)
{
	this->a1 = a1;
	this->a2 = a2;
	this->b11 = b11;
	this->b12 = b12;
	this->b21 = b21;
	this->b22 = b22;
	this->c1 = c1;
	this->c2 = c2;
	is3d = false;
}

void FlowFunctor::setParams3d(const float a1, const float a2, const float a3, const float b11, const float b12,
			      const float b13,  const float b21, const float b22, const float b23,
	 		      const float b31, const float b32, const float b33, const float c1, const float c2,
	   		      const float c3)
{
	this->a1 = a1;
	this->a2 = a2;
	this->a3 = a3;
	this->b11 = b11;
	this->b12 = b12;
	this->b13 = b13;
	this->b21 = b21;
	this->b22 = b22;
	this->b23 = b23;
	this->b31 = b31;
	this->b32 = b32;
	this->b33 = b33;
	this->c1 = c1;
	this->c2 = c2;
	this->c3 = c3;
	is3d = true;
}
