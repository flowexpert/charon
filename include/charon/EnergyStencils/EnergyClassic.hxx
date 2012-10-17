/*  Copyright (C) 2011, 2012
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
/** \file EnergyClassic.hxx
 *  Implementation of the parameter class EnergyClassic.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 20.12.2011
 */

#ifndef _ENERGYCLASSIC_HXX_
#define _ENERGYCLASSIC_HXX_

#include <charon/Stencil.hxx>

#include "EnergyClassic.h"

/**
 *  This energy stencil implements so-called classic regularization,
 *  proposed by Horn and Schunck in their 1981 paper
 *  "Determining Optical Flow".
 *
 *  Its energy equals (dxU^2 + dyU^2 + dxV^2 + dyV^2).
 */

template <class T>
EnergyClassic<T>::EnergyClassic(const std::string& name) :
	Stencil::Base<T>("EnergyClassic", name,
			"<h2>Energy stencil for classic regularization."),
	motionUV(true,false)
{
	this->_addInputSlot(penaltyFunction,
	                    "penaltyFunction",
	                    "penalty function",
	                    "PenaltyFunction<T>*");

	this->_addInputSlot(motionUV,
	                    "motionUV",
	                    "current motion components",
	                    "CImgList<T>");

	this->_addInputSlot(roi, "roi", "region of interest", "Roi<int>*");

	ParameteredObject::_addParameter(
			pUnknowns, "unknowns", "List of unknowns");
}

template <class T>
void EnergyClassic<T>::execute() {
	Stencil::Base<T>::execute();
	_lamb = this->lambda();
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
T EnergyClassic<T>::_energy( T x, T xo )
{
	return _penaltyFunction->getPenalty( pow(double(x - xo), 2.0) );
}

template <class T>
T EnergyClassic<T>::_energyGradient( T x, T xo )
{
	return _penaltyFunction->getPenaltyGradient( pow(double(x - xo), 2.0) ) * (x - xo);
}

template <class T>
T EnergyClassic<T>::_energyHessian( T x, T xo )
{
	return _penaltyFunction->getPenaltyGradient( pow(double(x - xo), 2.0) )
	     + pow(double(x - xo), 2.0) * _penaltyFunction->getPenaltyHessian( pow(double(x - xo), 2.0) );
}

template <class T>
T EnergyClassic<T>::getEnergy( int, int x, int y, int z, int )
{
	T motionUC = motionUV().atNXYZC( 0, x,   y,   z, 0 );
	T motionUN = motionUV().atNXYZC( 0, x,   y-1, z, 0 );
	T motionUE = motionUV().atNXYZC( 0, x+1, y,   z, 0 );
	T motionUS = motionUV().atNXYZC( 0, x,   y+1, z, 0 );
	T motionUW = motionUV().atNXYZC( 0, x-1, y,   z, 0 );
	T motionVC = motionUV().atNXYZC( 1, x,   y,   z, 0 );
	T motionVN = motionUV().atNXYZC( 1, x,   y-1, z, 0 );
	T motionVE = motionUV().atNXYZC( 1, x+1, y,   z, 0 );
	T motionVS = motionUV().atNXYZC( 1, x,   y+1, z, 0 );
	T motionVW = motionUV().atNXYZC( 1, x-1, y,   z, 0 );

	T energy =  _energy( motionUC, motionUN )
	         +  _energy( motionUC, motionUE )
	         +  _energy( motionUC, motionUS )
	         +  _energy( motionUC, motionUW );
        energy   += _energy( motionVC, motionVN )
                 +  _energy( motionVC, motionVE )
                 +  _energy( motionVC, motionVS )
                 +  _energy( motionVC, motionVW );

	return T(this->_lamb * energy);
}

template <class T>
std::vector<T> EnergyClassic<T>::getEnergyGradient(
  int, int x, int y, int z, int )
{
        T motionUC = motionUV().atNXYZC( 0, x,   y,   z, 0 );
        T motionUN = motionUV().atNXYZC( 0, x,   y-1, z, 0 );
        T motionUE = motionUV().atNXYZC( 0, x+1, y,   z, 0 );
        T motionUS = motionUV().atNXYZC( 0, x,   y+1, z, 0 );
        T motionUW = motionUV().atNXYZC( 0, x-1, y,   z, 0 );
        T motionVC = motionUV().atNXYZC( 1, x,   y,   z, 0 );
        T motionVN = motionUV().atNXYZC( 1, x,   y-1, z, 0 );
        T motionVE = motionUV().atNXYZC( 1, x+1, y,   z, 0 );
        T motionVS = motionUV().atNXYZC( 1, x,   y+1, z, 0 );
        T motionVW = motionUV().atNXYZC( 1, x-1, y,   z, 0 );

        T energyGradientU = _energyGradient( motionUC, motionUN )
                          + _energyGradient( motionUC, motionUE )
                          + _energyGradient( motionUC, motionUS )
                          + _energyGradient( motionUC, motionUW );
        T energyGradientV = _energyGradient( motionVC, motionVN )
                          + _energyGradient( motionVC, motionVE )
                          + _energyGradient( motionVC, motionVS )
                          + _energyGradient( motionVC, motionVW );

	std::vector<T> ret( 2, T(0.0) );
	ret[0] = T(this->_lamb * energyGradientU);
	ret[1] = T(this->_lamb * energyGradientV);
	return ret;
}

template <class T>
std::vector<T> EnergyClassic<T>::getEnergyHessian(
  int, int x, int y, int z, int )
{
	T motionUC = motionUV().atNXYZC( 0, x,   y,   z, 0 );
	T motionUN = motionUV().atNXYZC( 0, x,   y-1, z, 0 );
	T motionUE = motionUV().atNXYZC( 0, x+1, y,   z, 0 );
	T motionUS = motionUV().atNXYZC( 0, x,   y+1, z, 0 );
	T motionUW = motionUV().atNXYZC( 0, x-1, y,   z, 0 );
	T motionVC = motionUV().atNXYZC( 1, x,   y,   z, 0 );
	T motionVN = motionUV().atNXYZC( 1, x,   y-1, z, 0 );
	T motionVE = motionUV().atNXYZC( 1, x+1, y,   z, 0 );
	T motionVS = motionUV().atNXYZC( 1, x,   y+1, z, 0 );
	T motionVW = motionUV().atNXYZC( 1, x-1, y,   z, 0 );

	T energyHessianUU = _energyHessian( motionUC, motionUN )
	                  + _energyHessian( motionUC, motionUE )
	                  + _energyHessian( motionUC, motionUS )
	                  + _energyHessian( motionUC, motionUW );
	T energyHessianUV = T(0.0);
	T energyHessianVV = _energyHessian( motionVC, motionVN )
	                  + _energyHessian( motionVC, motionVE )
	                  + _energyHessian( motionVC, motionVS )
	                  + _energyHessian( motionVC, motionVW );

	std::vector<T> ret( 4, T(0.0) );
	ret[0] = T(this->_lamb * energyHessianUU);
	ret[1] = T(this->_lamb * energyHessianUV);
	ret[2] = T(this->_lamb * energyHessianUV);
	ret[3] = T(this->_lamb * energyHessianVV);

	return ret;
}

template <class T>
int EnergyClassic<T>::getEnergyGradientDimensions() { return 2; }

template <class T>
void EnergyClassic<T>::updateStencil(
		const std::string& unknown,
		const Point4D<int>& p, const int&)
{
	// motion component in neighborhood
	T motionN, motionE, motionS, motionW;
	T motionC = T(0);

	// penalty in neighborhood
	T pCN, pCE, pCS, pCW;
	T pSum = T(0.0);

	int motionConnected = motionUV.connected();

	// fill stencil with masks
	for(unsigned int i=0; i< this->pUnknowns.size() ; i++) {
		SubStencil<T> entry;
		if (motionConnected) {
			motionC = motionUV().atNXYZC( i, p.x,   p.y,   p.z, 0 );
			motionN = motionUV().atNXYZC( i, p.x,   p.y-1, p.z, 0 );
			motionE = motionUV().atNXYZC( i, p.x+1, p.y,   p.z, 0 );
			motionS = motionUV().atNXYZC( i, p.x,   p.y+1, p.z, 0 );
			motionW = motionUV().atNXYZC( i, p.x-1, p.y,   p.z, 0 );
			pCN = _lamb*_penaltyFunction->getPenaltyGradient( pow(double(motionC - motionN), 2.0) );
			pCE = _lamb*_penaltyFunction->getPenaltyGradient( pow(double(motionC - motionE), 2.0) );
			pCS = _lamb*_penaltyFunction->getPenaltyGradient( pow(double(motionC - motionS), 2.0) );
			pCW = _lamb*_penaltyFunction->getPenaltyGradient( pow(double(motionC - motionW), 2.0) );
		} else {
			pCN = _lamb*_penaltyFunction->getPenaltyGradient( 0.0 );
			pCE = _lamb*_penaltyFunction->getPenaltyGradient( 0.0 );
			pCS = _lamb*_penaltyFunction->getPenaltyGradient( 0.0 );
			pCW = _lamb*_penaltyFunction->getPenaltyGradient( 0.0 );
		}

		_dataMask.assign(3,3,1,1);
		_patternMask.assign(3,3,1,1);
		_center = Point4D<int>(1,1,0,0);
		_dataMask.fill(    0, 0, 0,     0, 0, 0,     0, 0, 0 );
		_patternMask.fill( 0, 0, 0,     0, 0, 0,     0, 0, 0 );

		if ((p.x == _xBegin) && (p.y == _yBegin)   && (pUnknowns[i] == unknown))
		{
			pSum = pCE + pCS;
			_dataMask.fill(    0,    0, 0,        0, pSum, -pCE,     0, -pCS,  0 );
			_patternMask.fill( 0,    1, 0,        1,    1,    1,     0,    1,  0 );
			if (motionConnected) {
				this->_rhs = motionE + motionS - 2*motionC;
			} else {
				this->_rhs = T(0.0);
			}
		}

		if ((p.x > _xBegin) && (p.x < _xEnd-1) && (p.y == _yBegin)   && (pUnknowns[i] == unknown))
		{
			pSum = pCE + pCS + pCW;
			_dataMask.fill(    0,    0, 0,     -pCW, pSum, -pCE,     0, -pCS,  0 );
			_patternMask.fill( 0,    1, 0,        1,    1,    1,     0,    1,  0 );
			if (motionConnected) {
				this->_rhs = motionE + motionS + motionW - 3*motionC;
			} else {
				this->_rhs = T(0.0);
			}
		}

		if ((p.x == _xEnd-1) && (p.y == _yBegin)   && (pUnknowns[i] == unknown))
		{
			pSum = pCS + pCW;
			_dataMask.fill(    0,    0, 0,     -pCW, pSum,    0,     0, -pCS,  0 );
			_patternMask.fill( 0,    1, 0,        1,    1,    1,     0,    1,  0 );
			if (motionConnected) {
				this->_rhs = motionS + motionW - 2*motionC;
			} else {
				this->_rhs = T(0.0);
			}
		}

		if ((p.x == _xBegin) && (p.y > _yBegin) && (p.y < _yEnd-1)   && (pUnknowns[i] == unknown))
		{
			pSum = pCN + pCE + pCS;
			_dataMask.fill(    0, -pCN, 0,        0, pSum, -pCE,     0, -pCS,  0 );
			_patternMask.fill( 0,    1, 0,        1,    1,    1,     0,    1,  0 );
			if (motionConnected) {
				this->_rhs = motionN + motionE + motionS - 3*motionC;
			} else {
				this->_rhs = T(0.0);
			}
		}

		if ((p.x > _xBegin) && (p.x < _xEnd-1) && (p.y > _yBegin) && (p.y < _yEnd-1)   && (pUnknowns[i] == unknown))
		{
			pSum = pCN + pCE + pCS + pCW;
			_dataMask.fill(    0, -pCN, 0,     -pCW, pSum, -pCE,     0, -pCS,  0 );
			_patternMask.fill( 0,    1, 0,        1,    1,    1,     0,    1,  0 );
			if (motionConnected) {
				this->_rhs = motionN + motionE + motionS + motionW - 4*motionC;
			} else {
				this->_rhs = T(0.0);
			}
		}

		if ((p.x == _xEnd-1) && (p.y > _yBegin) && (p.y < _yEnd-1)   && (pUnknowns[i] == unknown))
		{
			pSum = pCN + pCS + pCW;
			_dataMask.fill(    0, -pCN, 0,     -pCW, pSum,    0,     0, -pCS,  0 );
			_patternMask.fill( 0,    1, 0,        1,    1,    1,     0,    1,  0 );
			if (motionConnected) {
				this->_rhs = motionN + motionS + motionW - 3*motionC;
			} else {
				this->_rhs = T(0.0);
			}
		}

		if ((p.x == _xBegin) && (p.y == _yEnd-1)   && (pUnknowns[i] == unknown))
		{
			pSum = pCN + pCE;
			_dataMask.fill(    0, -pCN, 0,        0, pSum, -pCE,     0,    0,  0 );
			_patternMask.fill( 0,    1, 0,        1,    1,    1,     0,    1,  0 );
			if (motionConnected) {
				this->_rhs = motionN + motionE - 2*motionC;
			} else {
				this->_rhs = T(0.0);
			}
		}

		if ((p.x > _xBegin) && (p.x < _xEnd-1) && (p.y == _yEnd-1)   && (pUnknowns[i] == unknown))
		{
			pSum = pCN + pCE + pCW;
			_dataMask.fill(    0, -pCN, 0,     -pCW, pSum, -pCE,     0,    0,  0 );
			_patternMask.fill( 0,    1, 0,        1,    1,    1,     0,    1,  0 );
			if (motionConnected) {
				this->_rhs = motionN + motionE + motionW - 3*motionC;
			} else {
				this->_rhs = T(0.0);
			}
		}

		if ((p.x == _xEnd-1) && (p.y == _yEnd-1)   && (pUnknowns[i] == unknown))
		{
			pSum = pCN + pCW;
			_dataMask.fill(    0, -pCN, 0,     -pCW, pSum,    0,     0,    0,  0 );
			_patternMask.fill( 0,    1, 0,        1,    1,    1,     0,    1,  0 );
			if (motionConnected) {
				this->_rhs = motionN + motionW - 2*motionC;
			} else {
				this->_rhs = T(0.0);
			}
		}

		entry.data = _dataMask;
		entry.pattern = _patternMask;
		entry.center = _center;
		this->_subStencils[pUnknowns[i]] = entry;
	}
}

template <class T>
cimg_library::CImg<T> EnergyClassic<T>::apply(
		const cimg_library::CImgList<T>& seq,
		const unsigned int frame) const
{
	return seq[frame].get_convolve(_dataMask);
}

#endif /* _ENERGYCLASSIC_HXX_ */

