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
	ParameteredObject::_addParameter< T >(norm,"norm",
			"0=mode, 1=median, 2=mean", 2.0, "T");
	this->_addInputSlot(motionUV, "motionUV",
		"current motion components", "CImgList<T>");
}

template <class T>
void EnergyClassic<T>::execute() {
	_norm = norm();
	_lamb = this->lambda();
}

template <class T>
T signum( T arg ) {
	return (arg < 0) ? T(-1) : T(1) ;
}

template <class T>
T EnergyClassic<T>::_energyFunction( T x, T xo )
{
	T energy;
	energy = pow( fabs( double(x - xo) ), double(_norm) );
	return energy;
}

template <class T>
T EnergyClassic<T>::_energyFunctionDeriv( T x, T xo )
{
	T tmp;
	T f = x - xo;
	tmp = _norm * pow( fabs( double(f) ), double(_norm-1) ) * signum( f );
	return tmp;  //  ATTENTION : this is only the internal derivative !
}

template <class T>
T EnergyClassic<T>::getEnergy( int, int x, int y, int z, int )
{
	T pixelEnergy = 0.0;    //  energy per pixel

	T motionUC = 0.0;
	T motionUN = 0.0;
	T motionUE = 0.0;
	T motionUS = 0.0;
	T motionUW = 0.0;
	T motionVC = 0.0;
	T motionVN = 0.0;
	T motionVE = 0.0;
	T motionVS = 0.0;
	T motionVW = 0.0;

	motionUC = motionUV().atNXYZC( 0, x,   y,   z, 0 );
	motionUN = motionUV().atNXYZC( 0, x,   y-1, z, 0 );
	motionUE = motionUV().atNXYZC( 0, x+1, y,   z, 0 );
	motionUS = motionUV().atNXYZC( 0, x,   y+1, z, 0 );
	motionUW = motionUV().atNXYZC( 0, x-1, y,   z, 0 );
	motionVC = motionUV().atNXYZC( 1, x,   y,   z, 0 );
	motionVN = motionUV().atNXYZC( 1, x,   y-1, z, 0 );
	motionVE = motionUV().atNXYZC( 1, x+1, y,   z, 0 );
	motionVS = motionUV().atNXYZC( 1, x,   y+1, z, 0 );
	motionVW = motionUV().atNXYZC( 1, x-1, y,   z, 0 );

	pixelEnergy =  _energyFunction( motionUC, motionUN )
				+  _energyFunction( motionUC, motionUE )
				+  _energyFunction( motionUC, motionUS )
				+  _energyFunction( motionUC, motionUW );
	pixelEnergy += _energyFunction( motionVC, motionVN )
				+  _energyFunction( motionVC, motionVE )
				+  _energyFunction( motionVC, motionVS )
				+  _energyFunction( motionVC, motionVW );

	T ret = T(this->_lamb * pixelEnergy);
	return ret;
}

template <class T>
std::vector<T> EnergyClassic<T>::getEnergyGradient(
  int, int x, int y, int z, int )
{
	std::vector<T> ret( 2 );

	T motionUC = 0.0;
	T motionUN = 0.0;
	T motionUE = 0.0;
	T motionUS = 0.0;
	T motionUW = 0.0;
	T motionVC = 0.0;
	T motionVN = 0.0;
	T motionVE = 0.0;
	T motionVS = 0.0;
	T motionVW = 0.0;

	motionUC = motionUV().atNXYZC( 0, x,   y,   z, 0 );
	motionUN = motionUV().atNXYZC( 0, x,   y-1, z, 0 );
	motionUE = motionUV().atNXYZC( 0, x+1, y,   z, 0 );
	motionUS = motionUV().atNXYZC( 0, x,   y+1, z, 0 );
	motionUW = motionUV().atNXYZC( 0, x-1, y,   z, 0 );
	motionVC = motionUV().atNXYZC( 1, x,   y,   z, 0 );
	motionVN = motionUV().atNXYZC( 1, x,   y-1, z, 0 );
	motionVE = motionUV().atNXYZC( 1, x+1, y,   z, 0 );
	motionVS = motionUV().atNXYZC( 1, x,   y+1, z, 0 );
	motionVW = motionUV().atNXYZC( 1, x-1, y,   z, 0 );

	T pixelGradientU = _energyFunctionDeriv( motionUC, motionUN )
					 + _energyFunctionDeriv( motionUC, motionUE )
					 + _energyFunctionDeriv( motionUC, motionUS )
					 + _energyFunctionDeriv( motionUC, motionUW );
	T pixelGradientV = _energyFunctionDeriv( motionVC, motionVN )
					 + _energyFunctionDeriv( motionVC, motionVE )
					 + _energyFunctionDeriv( motionVC, motionVS )
					 + _energyFunctionDeriv( motionVC, motionVW );

	ret[0] = T(this->_lamb * pixelGradientU);
	ret[1] = T(this->_lamb * pixelGradientV);

	return ret;
}

template <class T>
int EnergyClassic<T>::getGradientComponentsCnt() { return 2; }

template <class T>
EnergyClassic<T>::~EnergyClassic() {
}

#endif /* _ENERGYCLASSIC_HXX_ */

