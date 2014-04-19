/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file TrigonometricBCC.hxx
 *  Implementation of the parameter class TrigonometricBCC.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 26.02.2013
 */

#ifndef _TRIGONOMETRICBCC_HXX_
#define _TRIGONOMETRICBCC_HXX_

#include "TrigonometricBCC.h"

#include <charon/Stencil.hxx>
#include <charon/PenaltyFunction.hxx>
#include <vector>

template <class T>
TrigonometricBCC<T>::TrigonometricBCC(const std::string& name) :
	Stencil::Base<T>(
		"TrigonometricBCC", name,
		"Implementation of the trigonometric brightness constancy constraint.<br>"
		"Motion components u and v are modelled as r*cos(phi) and r*sin(phi), "
		"respectively."
		"Parameters sought for are r, cos, and sin. "
		"Unfourtunately, there is no regularizer for phi, resp. cos(phi) and sin(phi) "
		"available at the moment. "
	     ),
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
                            "current parameter guess",
	                    "CImgList<T>");

	this->_addInputSlot(roi, "roi", "region of interest", "Roi<int>*");
	this->_addInputSlot(mask, "mask", "regularization mask ", "CImgList<T>");

        ParameteredObject::_addParameter(
                        pUnknowns, "unknowns", "List of unknowns");

	ParameteredObject::_setTags("charon-flow;Stencils;CImg");
}

template <class T>
void TrigonometricBCC<T>::execute() {
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
void TrigonometricBCC<T>::updateStencil(
                const std::string& unknown,
                const Point4D<int>& p, const int&)
{
	if (this->lambdaMask.connected())
		_lamb = _lambdaMask.atNXYZC( 0, p.x, p.y, p.z, 0 );

	bool _mask = true;

	// mask out masked positions
	if (mask.connected()) _mask =  mask()[0].atXY(p.x,p.y);

	// mask out fringe
	if (  (_xBegin > p.x) || (p.x >= _xEnd) || (_yBegin > p.y) || (p.y >= _yEnd)
	   || !_mask ) {
		this->_subStencils["r"].data(0,0) = T(0.0);
		this->_subStencils["cos"].data(0,0) = T(0.0);
		this->_subStencils["sin"].data(0,0) = T(0.0);
		this->_rhs = T(0.0);
		return;
	}

        // current values of image derivatives w.r.t. x, y, and t
        const T Ix = img_dx()(0,p.x,p.y,p.z,1);
        const T Iy = img_dy()(0,p.x,p.y,p.z,1);
        const T It = img_dt()(0,p.x,p.y,p.z,0); // (!!) works for temporal _difference_ [+1,-1] only

       	// initial parameter guess (from previous iteration)
	const T r   = parameterGuess()[0](p.x,p.y,p.z);
	const T cos = parameterGuess()[1](p.x,p.y,p.z);
	const T sin = parameterGuess()[2](p.x,p.y,p.z);

	// get first derivative of penalty function
	const T d_psi = _penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1,
	                                                      pow( double(It + Ix * r * cos + Iy * r * sin), 2.0 ) );

        // fill calculated data into stencil members
	const T c = _lamb * d_psi;
	if (unknown == "r") {
	        this->_subStencils["r"].data(0,0)   = c * (Ix * cos + Iy * sin) ;
	        this->_subStencils["cos"].data(0,0) = T(0.0) ;
	        this->_subStencils["sin"].data(0,0) = T(0.0) ;
		this->_rhs = c * (-It) ;
	} else if (unknown == "cos") {
		this->_subStencils["r"].data(0,0)   = T(0.0) ;
		this->_subStencils["cos"].data(0,0) = c * (Ix * r) ;
		this->_subStencils["sin"].data(0,0) = T(0.0) ;
	        this->_rhs = c * (-It - Iy * r * sin) ;
	} else if (unknown == "sin") {
		this->_subStencils["r"].data(0,0)   = T(0.0) ;
		this->_subStencils["cos"].data(0,0) = T(0.0) ;
		this->_subStencils["sin"].data(0,0) = c * (Iy * r) ;
		this->_rhs = c * (-It - Ix * r * cos) ;
	}
}

template <class T>
cimg_library::CImg<T> TrigonometricBCC<T>::apply(
                const cimg_library::CImgList<T>& seq,
                const unsigned int frame) const
{
        return seq[frame];  //  not implemented yet (!!)
}

#endif /* _TRIGONOMETRICBCC_HXX_ */

