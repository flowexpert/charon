/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

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
/** \file AffineBCC.hxx
 *  Implementation of the parameter class AffineBCC.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 09.02.2013
 */

#ifndef _AFFINEBCC_HXX_
#define _AFFINEBCC_HXX_

#include "AffineBCC.h"

#include <charon/Stencil.hxx>
#include <charon/PenaltyFunction.hxx>
#include <vector>

template <class T>
AffineBCC<T>::AffineBCC(const std::string& name) :
	Stencil::Base<T>(
	     "AffineBCC", name,
	     "<h2>Implementation of the overparameterized affine 6D brightness constancy constraint."
	     ),
	parameterGuess(true,false),
	mask(true,false)
{
	this->_addInputSlot(penaltyFunction,
	                    "penaltyFunction",
	                    "penalty function",
	                    "PenaltyFunction<T>*");

        this->_addInputSlot(img_dx,
                            "img_dx",
                            "derivatives wrt x of input images",
                            "CImgList<T>");
        this->_addInputSlot(img_dy,
                            "img_dy",
                            "derivatives wrt y of input images",
                            "CImgList<T>");
	this->_addInputSlot(img_dt,
	                    "img_dt",
	                    "derivatives wrt t of input images",
	                    "CImgList<T>");
	this->_addInputSlot(parameterGuess,
	                    "parameterGuess",
                            "current parameter guess (optional)",
	                    "CImgList<T>");

	this->_addInputSlot(roi, "roi", "region of interest", "Roi<int>*");
	this->_addInputSlot(mask, "mask", "regularization mask ", "CImgList<T>");

        ParameteredObject::_addParameter(
                        pUnknowns, "unknowns", "List of unknowns");

	ParameteredObject::_setTags("charon-flow;Stencils;CImg");
}

template <class T>
void AffineBCC<T>::execute() {
	Stencil::Base<T>::execute();
	if (!this->lambdaMask.connected()) {
		_lamb = this->lambda();
	} else {
		_lambdaMask = this->lambdaMask();
		_lamb = T(1.0);
	}
	_penaltyFunction = penaltyFunction();

	const Roi<int>& _roi = *(this->roi());
	_xBegin = _roi.xBegin();
	_xEnd = _roi.xEnd();
	_yBegin = _roi.yBegin();
	_yEnd = _roi.yEnd();

        // Copy the unknowns from the Parameter list into the set, which was
        // inherited from the Stencil class
        std::vector<std::string>::iterator puIt;
        for(puIt=pUnknowns().begin() ; puIt!=pUnknowns().end() ; puIt++) {
                Point4D<int> center;
                SubStencil<T> entry(1,1,1,1,center);
                entry.pattern(0,0) = 1;
                this->_subStencils[*puIt] = entry;
                this->_unknowns.insert(*puIt);
        }

        _patternMask.assign(1,1,1,1);
        _dataMask.assign(1,1,1,1);

        _patternMask.fill( 1 );
        _dataMask.fill( 1 );
        _center = Point4D<int>(0,0,0,0);

        _dataMask *= this->_lamb;
}

template <class T>
void AffineBCC<T>::updateStencil(
                const std::string& unknown,
                const Point4D<int>& p, const int&)
{
	if (this->lambdaMask.connected())
		_lamb = _lambdaMask.atNXYZC( 0, p.x, p.y, p.z, 0 );

	bool _mask = true;
	if (mask.connected()) _mask =  mask()[0].atXY(p.x,p.y);

	if (  (_xBegin > p.x) || (p.x >= _xEnd) || (_yBegin > p.y) || (p.y >= _yEnd)
	   || !_mask ) {
		this->_subStencils["a11"].data(0,0) = T(0.0);
		this->_subStencils["a12"].data(0,0) = T(0.0);
		this->_subStencils["a21"].data(0,0) = T(0.0);
		this->_subStencils["a22"].data(0,0) = T(0.0);
		this->_subStencils["b1"].data(0,0) = T(0.0);
		this->_subStencils["b2"].data(0,0) = T(0.0);
		this->_rhs = T(0.0);
		return;
	}

        // current values of ix,iy,it
        const T Ix = img_dx()(0,p.x,p.y,p.z,1);
        const T Iy = img_dy()(0,p.x,p.y,p.z,1);
        const T It = img_dt()(0,p.x,p.y,p.z,0); // (!!) works for temporal _difference_ [+1,-1] only

	// compute xn and yn like within equation (10) of Tal Nir_s paper
	// Over-Parameterized Variational Optical Flow
	T rho = 0.858;  //  see Tal Nir_s paper
	T x0 = (_xEnd - _xBegin) / 2.0;
	T y0 = (_yEnd - _yBegin) / 2.0;
	T xn = rho * (p.x - x0) / x0;
	T yn = rho * (p.y - y0) / y0;

	// penalty function derivative
	T a11 = 0.0, a12 = 0.0, a21 = 0.0, a22 = 0.0, b1 = 0.0, b2 = 0.0;
	T d_psi = 1.0;
	if (parameterGuess.connected()) {
        	// initial parameter guess from previous iteration
	        a11 = parameterGuess()[0](p.x,p.y,p.z);
	        a12 = parameterGuess()[1](p.x,p.y,p.z);
		a21 = parameterGuess()[2](p.x,p.y,p.z);
	        a22 = parameterGuess()[3](p.x,p.y,p.z);
	        b1  = parameterGuess()[4](p.x,p.y,p.z);
	        b2  = parameterGuess()[5](p.x,p.y,p.z);

		d_psi = _penaltyFunction->getPenaltyGradient(
			-1, p.x, p.y, p.z, -1,
		        pow(double(It + Ix * (a11*xn + a12*yn + b1)
		                      + Iy * (a21*xn + a22*yn + b2)), 2.0) );
	} else {
		d_psi = _penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, pow(double(It), 2.0) );
	}

        // fill calculated data into stencil members
	T c = _lamb * d_psi;
	if (unknown == "a11") {
	        this->_subStencils["a11"].data(0,0) = c * Ix * Ix * xn * xn ;
	        this->_subStencils["a12"].data(0,0) = c * Ix * Ix * xn * yn ;
	        this->_subStencils["a21"].data(0,0) = c * Ix * Iy * xn * xn ;
	        this->_subStencils["a22"].data(0,0) = c * Ix * Iy * xn * yn ;
	        this->_subStencils["b1"].data(0,0)  = c * Ix * Ix * xn ;
	        this->_subStencils["b2"].data(0,0)  = c * Ix * Iy * xn ;
	        this->_rhs = - c * It * Ix * xn ;
		if (parameterGuess.connected()) {
			this->_rhs -= c * Ix * xn
			            * (a11 * Ix * xn + a12 * Ix * yn + b1 * Ix
			              +a21 * Iy * xn + a22 * Iy * yn + b2 * Iy) ;
		}
	} else if (unknown == "a12") {
		this->_subStencils["a11"].data(0,0) = c * Ix * Ix * yn * xn ;
		this->_subStencils["a12"].data(0,0) = c * Ix * Ix * yn * yn ;
		this->_subStencils["a21"].data(0,0) = c * Ix * Iy * yn * xn ;
		this->_subStencils["a22"].data(0,0) = c * Ix * Iy * yn * yn ;
		this->_subStencils["b1"].data(0,0)  = c * Ix * Ix * yn ;
		this->_subStencils["b2"].data(0,0)  = c * Ix * Iy * yn ;
		this->_rhs = - c * It * Ix * yn ;
		if (parameterGuess.connected()) {
			this->_rhs -= c * Ix * yn
			            * (a11 * Ix * xn + a12 * Ix * yn + b1 * Ix
			              +a21 * Iy * xn + a22 * Iy * yn + b2 * Iy) ;
		}
	} else if (unknown == "a21") {
		this->_subStencils["a11"].data(0,0) = c * Iy * Ix * xn * xn ;
		this->_subStencils["a12"].data(0,0) = c * Iy * Ix * xn * yn ;
		this->_subStencils["a21"].data(0,0) = c * Iy * Iy * xn * xn ;
		this->_subStencils["a22"].data(0,0) = c * Iy * Iy * xn * yn ;
		this->_subStencils["b1"].data(0,0)  = c * Iy * Ix * xn ;
		this->_subStencils["b2"].data(0,0)  = c * Iy * Iy * xn ;
		this->_rhs = - c * It * Iy * xn ;
		if (parameterGuess.connected()) {
			this->_rhs -= c * Iy * xn
			            * (a11 * Ix * xn + a12 * Ix * yn + b1 * Ix
			              +a21 * Iy * xn + a22 * Iy * yn + b2 * Iy) ;
		}
	} else if (unknown == "a22") {
		this->_subStencils["a11"].data(0,0) = c * Iy * Ix * yn * xn ;
		this->_subStencils["a12"].data(0,0) = c * Iy * Ix * yn * yn ;
		this->_subStencils["a21"].data(0,0) = c * Iy * Iy * yn * xn ;
		this->_subStencils["a22"].data(0,0) = c * Iy * Iy * yn * yn ;
		this->_subStencils["b1"].data(0,0)  = c * Iy * Ix * yn ;
		this->_subStencils["b2"].data(0,0)  = c * Iy * Iy * yn ;
		this->_rhs = - c * It * Iy * yn ;
		if (parameterGuess.connected()) {
			this->_rhs -= c * Iy * yn
			            * (a11 * Ix * xn + a12 * Ix * yn + b1 * Ix
			              +a21 * Iy * xn + a22 * Iy * yn + b2 * Iy) ;
		}
	} else if (unknown == "b1") {
		this->_subStencils["a11"].data(0,0) = c * Ix * Ix * xn ;
		this->_subStencils["a12"].data(0,0) = c * Ix * Ix * yn ;
		this->_subStencils["a21"].data(0,0) = c * Ix * Iy * xn ;
		this->_subStencils["a22"].data(0,0) = c * Ix * Iy * yn ;
		this->_subStencils["b1"].data(0,0)  = c * Ix * Ix ;
		this->_subStencils["b2"].data(0,0)  = c * Ix * Iy ;
		this->_rhs = - c * It * Ix ;
		if (parameterGuess.connected()) {
			this->_rhs -= c * Ix
			            * (a11 * Ix * xn + a12 * Ix * yn + b1 * Ix
			              +a21 * Iy * xn + a22 * Iy * yn + b2 * Iy) ;
		}
	} else if (unknown == "b2") {
		this->_subStencils["a11"].data(0,0) = c * Iy * Ix * xn ;
		this->_subStencils["a12"].data(0,0) = c * Iy * Ix * yn ;
		this->_subStencils["a21"].data(0,0) = c * Iy * Iy * xn ;
		this->_subStencils["a22"].data(0,0) = c * Iy * Iy * yn ;
		this->_subStencils["b1"].data(0,0)  = c * Iy * Ix ;
		this->_subStencils["b2"].data(0,0)  = c * Iy * Iy ;
		this->_rhs = - c * It * Iy ;
		if (parameterGuess.connected()) {
			this->_rhs -= c * Iy
			            * (a11 * Ix * xn + a12 * Ix * yn + b1 * Ix
			              +a21 * Iy * xn + a22 * Iy * yn + b2 * Iy) ;
		}
	}
}

template <class T>
cimg_library::CImg<T> AffineBCC<T>::apply(
                const cimg_library::CImgList<T>& seq,
                const unsigned int frame) const
{
        return seq[frame];  //  not implemented yet (!!)
}

#endif /* _AFFINEBCC_HXX_ */

