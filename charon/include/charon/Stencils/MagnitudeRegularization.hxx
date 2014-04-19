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
/** \file MagnitudeRegularization.hxx
 *  Implementation of the parameter class MagnitudeRegularization.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 05.03.2013
 */

#ifndef _MAGNITUDEREGULARIZATION_HXX_
#define _MAGNITUDEREGULARIZATION_HXX_

#include <charon/Stencil.hxx>

#include "MagnitudeRegularization.h"

/**
 *  This stencil regularizes the magnitude r.
 */

template <class T>
MagnitudeRegularization<T>::MagnitudeRegularization(const std::string& name) :
	Stencil::Base<T>("MagnitudeRegularization", name,
			"Stencil for regularization of the magnitude r.<br>"
			"Used in conjunction with the Trigonometric BCC. "
	),
	mask(true,false)
{
	this->_addInputSlot(penaltyFunction,
	                    "penaltyFunction",
	                    "penalty function",
	                    "PenaltyFunction<T>*");

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
void MagnitudeRegularization<T>::execute() {
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
		this->_unknowns.insert(*puIt);
	}
}

template <class T>
void MagnitudeRegularization<T>::updateStencil(
		const std::string& unknown,
		const Point4D<int>& p, const int&)
{
	if (this->lambdaMask.connected())
		_lamb = _lambdaMask.atNXYZC( 0, p.x, p.y, p.z, 0 );

	// motion component in neighborhood
	T rN, rE, rS, rW;
	T rC = T(0);

	// penalty in neighborhood
	T pCN, pCE, pCS, pCW;
	T pSum = T(0.0);
	T pSumMask = T(0.0);
	T rSum = T(0.0);
	T rCenterSum = T(0.0);

	// fill regularization mask according to boundary conditions
	bool bordC = true;
	bool bordN = false, bordE = false, bordS = false, bordW = false;
	if ((_xBegin   <= p.x) && (p.x < _xEnd)   && (_yBegin+1 <= p.y) && (p.y < _yEnd))   bordN = true;
	if ((_xBegin   <= p.x) && (p.x < _xEnd-1) && (_yBegin   <= p.y) && (p.y < _yEnd))   bordE = true;
	if ((_xBegin   <= p.x) && (p.x < _xEnd)   && (_yBegin   <= p.y) && (p.y < _yEnd-1)) bordS = true;
	if ((_xBegin+1 <= p.x) && (p.x < _xEnd)   && (_yBegin   <= p.y) && (p.y < _yEnd))   bordW = true;

	// fill regularization mask with given regularization mask
	bool maskC = bordC, maskN = bordN, maskE = bordE, maskS = bordS, maskW = bordW;
	if (mask.connected()) {
		maskC &= (bool)mask()[0].atXY(p.x,   p.y);
		maskN &= (bool)mask()[0].atXY(p.x,   p.y-1);
		maskE &= (bool)mask()[0].atXY(p.x+1, p.y);
		maskS &= (bool)mask()[0].atXY(p.x,   p.y+1);
		maskW &= (bool)mask()[0].atXY(p.x-1, p.y);
	}

	// fill stencil with masks
	SubStencil<T> entry;
	rC = parameterGuess().atNXYZC( 0, p.x,   p.y,   p.z, 0 );
	rN = parameterGuess().atNXYZC( 0, p.x,   p.y-1, p.z, 0 );
	rE = parameterGuess().atNXYZC( 0, p.x+1, p.y,   p.z, 0 );
	rS = parameterGuess().atNXYZC( 0, p.x,   p.y+1, p.z, 0 );
	rW = parameterGuess().atNXYZC( 0, p.x-1, p.y,   p.z, 0 );
	pCN = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, pow(double(rC - rN), 2.0) );
	pCE = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, pow(double(rC - rE), 2.0) );
	pCS = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, pow(double(rC - rS), 2.0) );
	pCW = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, pow(double(rC - rW), 2.0) );

	_dataMask.assign(3,3,1,1);
	_patternMask.assign(3,3,1,1);
	_center = Point4D<int>(1,1,0,0);
	_dataMask.fill(    0, 0, 0,     0, 0, 0,     0, 0, 0 );
	if (unknown == "r") {
		_patternMask.fill( 0, 1, 0,     1, 1, 1,     0, 1, 0 );
	} else {
		_patternMask.fill( 0, 0, 0,     0, 0, 0,     0, 0, 0 );
	}
	this->_rhs = T(0.0);

	if (unknown == "r")
	{
		pSum =  T(0.0);
		pSum += (maskN ? pCN : T(0.0));
		pSum += (maskE ? pCE : T(0.0));
		pSum += (maskS ? pCS : T(0.0));
		pSum += (maskW ? pCW : T(0.0));
		pSumMask = pSum;

		if (pSumMask) {
			_dataMask.fill( T(0.0),                  (maskN ? -pCN : T(0.0)), T(0.0),
			                (maskW ? -pCW : T(0.0)), pSum,                    (maskE ? -pCE : T(0.0)),
			                T(0.0),                  (maskS ? -pCS : T(0.0)), T(0.0) );
		} else {  //  if all neighbors are out masked then inpainting
			pSum = T(0.0);
			pSum += (bordN ? pCN : T(0.0));
			pSum += (bordE ? pCE : T(0.0));
			pSum += (bordS ? pCS : T(0.0));
			pSum += (bordW ? pCW : T(0.0));

			_dataMask.fill( T(0.0),                   (bordN ? -pCN : T(0.0)), T(0.0),
			                (bordW ? -pCW : T(0.0)),  pSum,                    (bordE ? -pCE : T(0.0)),
			                T(0.0),                   (bordS ? -pCS : T(0.0)), T(0.0) );
		}

		if (pSumMask) {
			rSum =  T(0.0);
			rSum += (maskN ? rN : T(0.0));
			rSum += (maskE ? rE : T(0.0));
			rSum += (maskS ? rS : T(0.0));
			rSum += (maskW ? rW : T(0.0));
			rCenterSum =  T(0.0);
			rCenterSum += (maskN ? rC : T(0.0));
			rCenterSum += (maskE ? rC : T(0.0));
			rCenterSum += (maskS ? rC : T(0.0));
			rCenterSum += (maskW ? rC : T(0.0));
		} else {
			rSum =  T(0.0);
			rSum += (bordN ? rN : T(0.0));
			rSum += (bordE ? rE : T(0.0));
			rSum += (bordS ? rS : T(0.0));
			rSum += (bordW ? rW : T(0.0));
			rCenterSum =  T(0.0);
			rCenterSum += (bordN ? rC : T(0.0));
			rCenterSum += (bordE ? rC : T(0.0));
			rCenterSum += (bordS ? rC : T(0.0));
			rCenterSum += (bordW ? rC : T(0.0));
		}
		this->_rhs = rSum - rCenterSum;
	}

	entry.data = _dataMask;
	entry.pattern = _patternMask;
	entry.center = _center;
	this->_subStencils[unknown] = entry;
}

template <class T>
cimg_library::CImg<T> MagnitudeRegularization<T>::apply(
		const cimg_library::CImgList<T>& seq,
		const unsigned int frame) const
{
	return seq[frame].get_convolve(_dataMask);
}

#endif /* _MAGNITUDEREGULARIZATION_HXX_ */

