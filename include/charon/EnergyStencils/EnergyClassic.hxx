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

#include <charon/EnergyStencil.hxx>

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
  EnergyStencil<T>(
	     "EnergyClassic", name,
	     "<h2>Energy stencil for classic regularization."
	     )
{
	this->_addInputSlot(penaltyFunction,
	                    "penaltyFunction",
	                    "penalty function",
	                    "PenaltyFunction<T>*");

	this->_addInputSlot(motionUV,
	                    "motionUV",
	                    "current motion components",
	                    "CImgList<T>");
}

template <class T>
void EnergyClassic<T>::execute() {
  PARAMETEREDOBJECT_AVOID_REEXECUTION;
  ParameteredObject::execute();

  _lamb = this->lambda();
  _penaltyFunction = penaltyFunction();
}

template <class T>
T EnergyClassic<T>::_energyFunction( T x, T xo )
{
	T energy = _penaltyFunction->getPenalty( x - xo );
	return energy;
}

template <class T>
T EnergyClassic<T>::_energyFunctionDeriv( T x, T xo )
{
	T energyGradient = _penaltyFunction->getPenaltyGradient( x - xo );
	return energyGradient;
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

	T pixelEnergy =  _energyFunction( motionUC, motionUN )
	              +  _energyFunction( motionUC, motionUE )
	              +  _energyFunction( motionUC, motionUS )
	              +  _energyFunction( motionUC, motionUW );
        pixelEnergy   += _energyFunction( motionVC, motionVN )
                      +  _energyFunction( motionVC, motionVE )
                      +  _energyFunction( motionVC, motionVS )
                      +  _energyFunction( motionVC, motionVW );

	return T(this->_lamb * pixelEnergy);
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

        T pixelGradientU = _energyFunctionDeriv( motionUC, motionUN )
                         + _energyFunctionDeriv( motionUC, motionUE )
                         + _energyFunctionDeriv( motionUC, motionUS )
                         + _energyFunctionDeriv( motionUC, motionUW );
        T pixelGradientV = _energyFunctionDeriv( motionVC, motionVN )
                         + _energyFunctionDeriv( motionVC, motionVE )
                         + _energyFunctionDeriv( motionVC, motionVS )
                         + _energyFunctionDeriv( motionVC, motionVW );

	std::vector<T> ret( 2 );
	ret[0] = T(this->_lamb * pixelGradientU);
	ret[1] = T(this->_lamb * pixelGradientV);

        return ret;
}

template <class T>
int EnergyClassic<T>::getGradientComponentsCnt() { return 2; }

template <class T>
EnergyClassic<T>::~EnergyClassic()
{
}

#endif /* _ENERGYCLASSIC_HXX_ */

