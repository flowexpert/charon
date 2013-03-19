/*  Copyright (C) 2010 Jens-Malte Gottfried

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
/** \file DataConstant.hxx
 *  Implementation of class DataConstant.
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried </a>
 *  \date 2010-01-18
 */

#ifndef _DATA_CONSTANT_HXX_
#define _DATA_CONSTANT_HXX_

#include <charon/Stencil.hxx>
#include <charon-utils/ImgTool.hxx>
#include <charon/Functions/Diff2Function.h>
#include <charon/Functions/Diff2LinFunction.h>
#include "DataConstant.h"

template <class T>
DataConstant<T>::DataConstant(const std::string& name) :
		Stencil::Base<T>("DataConstant", name,
			"Stencil modeling weighted brightness constancy. "
			"Weights are calculated using a given Psi function. "
			"See doxygen documentation for detailed derivation. ")
{
	ParameteredObject::_addInputSlot(flowGuess, "flowGuess",
		"initial guess of real flow, no zeros, even in warped mode",
		"CImgList<T>");
	ParameteredObject::_addInputSlot(psi, "psi",
		"brightness constancy probability function", "Function*");
	ParameteredObject::_addInputSlot(ix, "ix",
		"image sequence derivative (wrt x)", "CImgList<T>");
	ParameteredObject::_addInputSlot(iy, "iy",
		"image sequence derivative (wrt y)", "CImgList<T>");
	ParameteredObject::_addInputSlot(it, "it",
		"time derivative of warped image sequence", "CImgList<T>");
	ParameteredObject::_addParameter(calcDifferences, "calcDifferences",
		"set to true if you want to calculate only increments<br><br>"
		"This skips writing an offset caused by the current flow into "
		"the RHS. The solver will then calculate flow differences to "
		"the previous iteration. Set option \"accumulate\" to true in the "
		"iterator.", true);
	ParameteredObject::_addParameter(useImg2, "useImg2",
		"use derivatives of img2<br><br>"
		"Use derivatives I_x,I_y of Img2 istead of Img1.",
		false);
	ParameteredObject::_addParameter(useDiff2Lin, "useDiff2Lin",
		"use linearization of second derivative", true);
}

template <class T>
void DataConstant<T>::execute() {
	Stencil::Base<T>::execute();

	// add unknowns
	this->_unknowns.clear();
	this->_unknowns.insert("a1");
	this->_unknowns.insert("a2");

	assert(flowGuess().size() == 2);  // 2D flows only
	assert(it().size()     == 1);     // monochrome images
	assert(it()[0].depth() == 1);     // 2D images
	assert(it()[0].spectrum() == 2);  // 2 frames
	assert(it().is_sameNXYZC(ix()));
	assert(it().is_sameNXYZC(iy()));

	this->_subStencils.clear();
	this->_subStencils["a1"] =
			SubStencil<T>(1u,1u,1u,1u,Point4D<int>());
	this->_subStencils["a2"] =
			SubStencil<T>(1u,1u,1u,1u,Point4D<int>());
	this->_subStencils["a1"].pattern(0,0) = 1;
	this->_subStencils["a2"].pattern(0,0) = 1;

	if(!dynamic_cast<DiffFunction*>(psi()))
		throw std::invalid_argument(
				"provided psi function has to be at "
				"least one times differentiable");
	if(useDiff2Lin() && !dynamic_cast<Diff2LinFunction*>(psi()))
		throw std::invalid_argument(
				"provided psi function must implement Diff2LinFunction!");
	if(!useDiff2Lin() && !dynamic_cast<Diff2Function*>(psi()))
		throw std::invalid_argument(
				"provided psi function must implement Diff2Function!");
}

template <class T>
void DataConstant<T>::updateStencil(
		const std::string& unknown,
		const Point4D<int>& p, const int&) {
	assert(p.isPositive());
	assert(p.z==0);
	assert(p.t==0);

	const unsigned int x = p.x;
	const unsigned int y = p.y;
	const unsigned int z = p.z;
	const unsigned int t = useImg2()?1u:0u;

	const bool isU = (unknown == "a1");
	assert (isU || unknown == "a2");

	// current values of ix,iy,it
	const double cix = this->ix()(0,x,y,z,t);
	const double ciy = this->iy()(0,x,y,z,t);
	const double cit = this->it()(0,x,y,z,0);
	const double cik = isU ? cix : ciy;
	// initial flow guess from previous iteration
	const double u0 = flowGuess()[0](x,y,z);
	const double v0 = flowGuess()[1](x,y,z);

	// deviation from brightness constancy (BC error) = -i_t,
	// argument to be passed to the psi function
	const std::vector<double> bce(1u,-cit);

	double weight;
	if (useDiff2Lin()) {
		// linearized version of second derivative (see module description)
		static const Diff2LinFunction* psi2 =
				dynamic_cast<Diff2LinFunction*>(psi());
		assert(psi2);
		assert(psi2 == dynamic_cast<Diff2LinFunction*>(psi()));
		weight = psi2->diff2Linearized(bce);
	}
	else {
		// second derivative as weights, corresponds to taylor expansion
		static const Diff2Function* psi2 =
				dynamic_cast<Diff2Function*>(psi());
		assert(psi2);
		assert(psi2 == dynamic_cast<Diff2Function*>(psi()));
		weight = psi2->diff2(bce);
	}

	// weighted Horn&Schunck equations
	double dataU = weight * cik * cix;
	double dataV = weight * cik * ciy;
	double rhs   = weight * cik * (-cit);

	// add rhs offset if not calculating flow increments
	if (!calcDifferences())
		rhs += u0*dataU+v0*dataV;

	// fill calculated data into stencil members, applying lambda
	const T      l  = this->lambda();
	this->_subStencils["a1"].data(0,0) = l * T(dataU);
	this->_subStencils["a2"].data(0,0) = l * T(dataV);
	this->_rhs  = l * T(rhs);
}

template <class T>
cimg_library::CImg<T> DataConstant<T>::apply(
		const cimg_library::CImgList<T>& /*seq*/,
		const unsigned int /*frame*/) const {
	const cimg_library::CImg<T>& cit = it()[0];
	cimg_library::CImg<T> res(cit.width(),cit.height(),cit.depth(),1,T(0));
	std::vector<double> bce(1u,0.);

	if(useDiff2Lin()) {
		const Diff2LinFunction* psi2 =
				dynamic_cast<Diff2LinFunction*>(psi());
		assert(psi2);
		cimg_forXYZ(res,x,y,z) {
			bce[0] = -cit(x,y,z);
			res(x,y,z)=T(psi2->diff2Linearized(bce));
		}
	}
	else {
		const Diff2Function* psi2 =
				dynamic_cast<Diff2Function*>(psi());
		assert(psi2);
		cimg_forXYZ(res,x,y,z) {
			bce[0] = -cit(x,y,z);
			res(x,y,z)=T(psi2->diff2(bce));
		}
	}
	return res;
}

template <class T>
double DataConstant<T>::getEnergy() const {
	const cimg_library::CImg<T>& cit = it()[0];
	assert(cit.depth()    == 1);
	assert(cit.spectrum() == 2 || cit.spectrum() == 1);

	cimg_library::CImg<double> res(cit.width(), cit.height());
	res.fill(0.);

	const Function* psi0 = psi();

	std::vector<double> bce(1u,0.);
	cimg_forXY(res,x,y) {
		bce[0] = -cit(x,y);
		res(x,y) += psi0->calculate(bce);
	}
	return res.sum();
}

#endif //_DATA_CONSTANT_HXX_
