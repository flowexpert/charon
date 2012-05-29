/*  Copyright (C) 2011 Heidelberg Collaboratory for Image Processing

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

	ParameteredObject::_addParameter(
			pUnknowns, "unknowns", "List of unknowns");
}

template <class T>
void EnergyClassic<T>::execute() {
	Stencil::Base<T>::execute();
	_lamb = this->lambda();
	_penaltyFunction = penaltyFunction();

	// Copy the unknowns from the Parameter list into the set, which was
	// inherited from the Stencil class
	std::vector<std::string>::iterator puIt;
	for(puIt=pUnknowns().begin() ; puIt!=pUnknowns().end() ; puIt++) {
		this->_unknowns.insert(*puIt);
	}

	_patternMask.assign(3,3,1,1);
	_dataMask.assign(3,3,1,1);

	_patternMask.fill(
			0,  1,  0,
			1,  1,  1,
			0,  1,  0);
	_dataMask.fill(
			 0, -1,  0,
			-1,  4, -1,
			 0, -1,  0);
	_center = Point4D<int>(1,1,0,0);

	_dataMask *= this->_lamb;

	// precalculate rhs values for whole image
	if (motionUV.connected()) {
		const cimg_library::CImgList<T>& flow = motionUV();
		_rhsVals.assign(flow);
		cimglist_for(flow, kk) {
			_rhsVals[kk] = this->apply(flow, kk);
			_rhsVals[kk] *= -1;
		}
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
    const cimg_library::CImgList<T>& uv = motionUV();
    T motionUC = uv.atNXYZC( 0, x,   y,   z, 0 );
    T motionUN = uv.atNXYZC( 0, x,   y-1, z, 0 );
    T motionUE = uv.atNXYZC( 0, x+1, y,   z, 0 );
    T motionUS = uv.atNXYZC( 0, x,   y+1, z, 0 );
    T motionUW = uv.atNXYZC( 0, x-1, y,   z, 0 );
    T motionVC = uv.atNXYZC( 1, x,   y,   z, 0 );
    T motionVN = uv.atNXYZC( 1, x,   y-1, z, 0 );
    T motionVE = uv.atNXYZC( 1, x+1, y,   z, 0 );
    T motionVS = uv.atNXYZC( 1, x,   y+1, z, 0 );
    T motionVW = uv.atNXYZC( 1, x-1, y,   z, 0 );

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
        const cimg_library::CImgList<T>& uv = motionUV();
        T motionUC = uv.atNXYZC( 0, x,   y,   z, 0 );
        T motionUN = uv.atNXYZC( 0, x,   y-1, z, 0 );
        T motionUE = uv.atNXYZC( 0, x+1, y,   z, 0 );
        T motionUS = uv.atNXYZC( 0, x,   y+1, z, 0 );
        T motionUW = uv.atNXYZC( 0, x-1, y,   z, 0 );
        T motionVC = uv.atNXYZC( 1, x,   y,   z, 0 );
        T motionVN = uv.atNXYZC( 1, x,   y-1, z, 0 );
        T motionVE = uv.atNXYZC( 1, x+1, y,   z, 0 );
        T motionVS = uv.atNXYZC( 1, x,   y+1, z, 0 );
        T motionVW = uv.atNXYZC( 1, x-1, y,   z, 0 );

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
	// penalty function derivative
	std::vector<T> d_psi( 2, T(1.0) );
	if (penaltyFunction.connected())
		d_psi = this->getEnergyGradient( 0, p.x, p.y, p.z, 0 );

	// fill stencil with masks
	for(unsigned int i=0; i< this->pUnknowns.size() ; i++) {
		SubStencil<T> entry;
		if(pUnknowns[i] == unknown) {
			entry.center  = _center;

			// shared assignment (no copying of values)
			entry.data.assign(_dataMask,true);
			entry.data *= d_psi[i];

			entry.pattern.assign(_patternMask,true);
			if (motionUV.connected()) {
				this->_rhs = d_psi[i] * _rhsVals[i](p.x,p.y,p.z,p.t);
			} else {
				this->_rhs = T(0);
			}

		} else {
			// empty substencil for other unknowns
			entry.center = Point4D<int>();
			entry.data.clear();
			entry.pattern.clear();
		}
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

