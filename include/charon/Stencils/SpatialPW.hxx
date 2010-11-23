/*  This file is part of Charon.

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
/** @file SpatialPW.hxx
 *  Implementation of class SpatialPW.
 *  @author <a href="jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 2010-01-18
 */


#ifndef _SPATIAL_PW_HXX_
#define _SPATIAL_PW_HXX_

#ifdef _MSC_VER
#include <cfloat>
namespace std {
	inline bool isinf(double num) {
		return !_finite(num) && !_isnan(num);
	}
	inline bool isnan(double num) {
		return !!_isnan(num);
	}
}
#endif

#include <charon/Stencil.hxx>
#include <charon/Functions/Diff2Function.h>
#include <charon/Functions/Diff2LinFunction.h>
#include "SpatialPW.h"

template <typename T>
SpatialPW<T>::SpatialPW(const std::string& name) :
		Stencil<T>("SpatialPW", name,
			"Stencil modeling spatial smoothness using learned probabilities "
			"for flow changes.") {
	ParameteredObject::_addInputSlot(flowGuess, "flowGuess",
		"initial guess of flow", "CImgList<T>");
	ParameteredObject::_addInputSlot(pu, "pu",
		"fitted probabilities for u", "Function*");
	ParameteredObject::_addInputSlot(pv, "pv",
		"fitted probabilities for v", "Function*");
	ParameteredObject::_addParameter(calcDiff, "calcDiff",
		"drop initial guess terms from RHS", true);
	ParameteredObject::_addParameter(useDiff2Lin, "useDiff2Lin",
		"use linearization of second derivative", true);
}

template <typename T>
void SpatialPW<T>::execute() {
	ParameteredObject::execute();
	if(flowGuess().size() != 2) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n";
		msg << "\tGiven flow field does not have 2 components!";
		throw std::out_of_range(msg.str().c_str());
	}

	assert(pu());
	assert(pv());

	// add unknowns
	this->_unknowns.clear();
	this->_unknowns.insert("a1");
	this->_unknowns.insert("a2");

	this->_subStencils.clear();
	this->_subStencils["a1"] = SubStencil<T>();
	this->_subStencils["a2"] = SubStencil<T>();

	// create masks in appropriate dimensions
	_patternMask.assign(3,3,1,1,0);

	// filling mask with values and setting the center
	_patternMask(1,0) = 1;
	_patternMask(0,1) = 1;
	_patternMask(1,1) = 1;
	_patternMask(2,1) = 1;
	_patternMask(1,2) = 1;

	_center.x=1u;
	_center.y=1u;
	_center.z=0u;
	_center.t=0u;

	_pu  = dynamic_cast<DiffFunction*>    (pu());
	_pv  = dynamic_cast<DiffFunction*>    (pv());
	_pu2 = dynamic_cast<Diff2Function*>   (pu());
	_pv2 = dynamic_cast<Diff2Function*>   (pv());
	_puL = dynamic_cast<Diff2LinFunction*>(pu());
	_pvL = dynamic_cast<Diff2LinFunction*>(pv());
	if(!(bool)_pu || !(bool)_pv)
		throw std::invalid_argument(
				"pu and pv have to implement at least DiffFunction!");
	if(_pu->dims() != 1 || _pv->dims() != 1)
		throw std::invalid_argument(
				"pu and pv have to be one-dimensional!");
	if(!useDiff2Lin() && !((bool)_pu && (bool)_pv))
		throw std::invalid_argument(
				"pu and pv have to implement Diff2Function!");
	if(useDiff2Lin() && !((bool)_puL && (bool)_pvL))
		throw std::invalid_argument(
				"pu and pv have to implement Diff2LinFunction!");
}

template <typename T>
void SpatialPW<T>::updateStencil(
		const std::string& unknown,
		const Point4D<int>& p, const int& v) {

	// only monochrome images can be used
	if (v > 0) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n";
		msg << "\tMultichannel images not supported!";
		throw std::invalid_argument(msg.str());
	}

	// fill stencil with masks
	std::set<std::string>::iterator uIt;
	for(uIt=this->_unknowns.begin() ; uIt!=this->_unknowns.end() ; uIt++) {
		SubStencil<T> entry;
		if(*uIt == unknown) {
			entry.center  = _center;
			if(this->lambda() != 0.) {
				this->_rhs = _calculateData(entry.data,unknown,p);
				entry.data *= this->lambda();
				this->_rhs *= this->lambda();
			}
			else {
				this->_rhs = T(0);
				entry.data.assign(3,3,1,1,0);
			}
			entry.pattern = _patternMask;
		}
		else {
			// empty substencil for other unknowns
			entry.center = Point4D<int>();
			entry.data.clear();
			entry.pattern.clear();
		}
		this->_subStencils[*uIt] = entry;
	}
}

template <typename T>
inline T SpatialPW<T>::_calculateData(
		cimg_library::CImg<T>& entry,
		const std::string& unknown,
		const Point4D<int>& p) const {
	entry.assign(3,3,1,1,0);
	const int& x = p.x;
	const int& y = p.y;
	const int& z = p.z;
	const int& t = p.t;

	// this can actually handle only 2D flow estimation problems
	const bool isU = (unknown == "a1");
	assert(isU || unknown == "a2");

	// initial guess of flow for the given unknown
	const cimg_library::CImg<T>& u = flowGuess()[isU ? 0 : 1];

	// Flow is continued constant at the image borders,
	// i.e. dudx and dudy is zero there.
	const double u_cc = u.atXY(x  ,y  ,z,t);
	const double u_pc = u.atXY(x-1,y  ,z,t);
	const double u_nc = u.atXY(x+1,y  ,z,t);
	const double u_cp = u.atXY(x  ,y-1,z,t);
	const double u_cn = u.atXY(x  ,y+1,z,t);

	// flow differences (relative to central pixel)
	const std::vector<double> du_pc(1u, u_pc - u_cc);
	const std::vector<double> du_nc(1u, u_nc - u_cc);
	const std::vector<double> du_cp(1u, u_cp - u_cc);
	const std::vector<double> du_cn(1u, u_cn - u_cc);

	// here come the regularization weights
	double d2Eduu_pc, d2Eduu_nc, d2Eduu_cp, d2Eduu_cn;
	if(useDiff2Lin()) {
		const Diff2LinFunction& psi2 = *(isU ? _puL : _pvL);
		d2Eduu_pc = psi2.diff2Linearized(du_pc);
		d2Eduu_nc = psi2.diff2Linearized(du_nc);
		d2Eduu_cp = psi2.diff2Linearized(du_cp);
		d2Eduu_cn = psi2.diff2Linearized(du_cn);
	}
	else {
		const Diff2Function& psi2 = *(isU ? _pu2 : _pv2);
		d2Eduu_pc = psi2.diff2(du_pc);
		d2Eduu_nc = psi2.diff2(du_nc);
		d2Eduu_cp = psi2.diff2(du_cp);
		d2Eduu_cn = psi2.diff2(du_cn);
	}

	// center is sum of outer values
	const double d2Eduu_cc = d2Eduu_pc+d2Eduu_nc+d2Eduu_cp+d2Eduu_cn;

	if(std::isnan(d2Eduu_cc) || std::isinf(d2Eduu_cc)) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "Inf/Nan occured during SpatialPW Calculation!";
		throw std::runtime_error(msg.str());
	}

	// fill stencil (with correct sign)
	entry(1,1) = T(+d2Eduu_cc);
	entry(0,1) = T(-d2Eduu_pc);
	entry(2,1) = T(-d2Eduu_nc);
	entry(1,0) = T(-d2Eduu_cp);
	entry(1,2) = T(-d2Eduu_cn);

	// this is the term -dE/du (-dE/dv)
	const DiffFunction& psi = *(isU ? _pu : _pv);
	const double ndEdu = psi.diff(du_pc) + psi.diff(du_nc)
			+ psi.diff(du_cp) + psi.diff(du_cn);

	return T(calcDiff() ? ndEdu : 0);
}

template <class T>
double SpatialPW<T>::getEnergy() const {
	cimg_library::CImg<T> res(
			flowGuess()[0].width(),
			flowGuess()[0].height());
	res.fill(T(0));

	CImg_2x2(i, T);

	cimg_for2x2(flowGuess()[0], x,y,0,0, i, T) {
		res(x,y) += T(_pu->calculate(std::vector<double>(1u,inc-icc)));
		res(x,y) += T(_pu->calculate(std::vector<double>(1u,icn-icc)));
	}
	cimg_for2x2(flowGuess()[1], x,y,0,0, i, T) {
		res(x,y) += T(_pv->calculate(std::vector<double>(1u,inc-icc)));
		res(x,y) += T(_pv->calculate(std::vector<double>(1u,icn-icc)));
	}
	return res.sum();
}

template <typename T>
cimg_library::CImg<T> SpatialPW<T>::apply(
		const cimg_library::CImgList<T>& flow,
		const unsigned int) const {
	// check preconditions
	assert(flow.size() == 2);
	assert(flow[0].depth() == 1);
	assert(flow[0].spectrum() == 1);

	// simpler aliases
	const cimg_library::CImg<T>& u = flow[0];
	const cimg_library::CImg<T>& v = flow[1];
	assert(u.is_sameXYZC(v));

	// store the four possible calculation results
	cimg_library::CImg<T> dudx(u.width(),u.height());
	cimg_library::CImg<T> dudy(u.width(),u.height());
	cimg_library::CImg<T> dvdx(v.width(),v.height());
	cimg_library::CImg<T> dvdy(v.width(),v.height());

	// calculate values
	CImg_2x2(u_, T);
	CImg_2x2(v_, T);

	std::vector<double> ux(1u,0.), uy(1u,0.);
	cimg_for2x2(u, x,y,0,0, u_, T) {
		ux[0] = u_nc - u_cc;
		uy[0] = u_cn - u_cc;
		if(useDiff2Lin) {
			dudx(x,y) = T(_puL->diff2Linearized(ux));
			dudy(x,y) = T(_puL->diff2Linearized(uy));
		}
		else {
			dudx(x,y) = T(_pu2->diff2(ux));
			dudy(x,y) = T(_pu2->diff2(uy));
		}
	}

	std::vector<double> vx(1u,0.), vy(1u,0.);
	cimg_for2x2(v, x,y,0,0, v_, T) {
		vx[0] = v_nc - v_cc;
		vy[0] = v_cn - v_cc;
		if(useDiff2Lin) {
			dvdx(x,y) = T(_pvL->diff2Linearized(vx));
			dvdy(x,y) = T(_pvL->diff2Linearized(vy));
		}
		else {
			dvdx(x,y) = T(_pv2->diff2(vx));
			dvdy(x,y) = T(_pv2->diff2(vy));
		}
	}

	// return as appended image
	return (dudx,dudy,dvdx,dvdy).get_append('x');
}

#endif // _SPATIAL_PW_HXX_
