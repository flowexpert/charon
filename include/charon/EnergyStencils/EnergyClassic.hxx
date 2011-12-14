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
 *  \date 13.10.2011
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
  ParameteredObject::_addParameter< int >(norm,
                      "norm",
                      "0=mode, 1=median, 2=mean",
                      2, "int");
	this->_addInputSlot(motionUV,
	                    "motionUV",
	                    "current motion components",
	                    "CImgList<T>");
}

template <class T>
void EnergyClassic<T>::execute() {
  PARAMETEREDOBJECT_AVOID_REEXECUTION;
  ParameteredObject::execute();

  _norm = norm();
}

template <class T>
T EnergyClassic<T>::getEnergy( int, int x, int y, int z, int )
{
        T du, dv;
	T pixelEnergy;    //  energy per pixel

	T ret;

        pixelEnergy = 0.0;
	du = motionUV().atNXYZC( 0, x+1, y, z, 0 )  // u(x+1,y)
	   - motionUV().atNXYZC( 0, x-1, y, z, 0 ); // u(x-1,y)
	du *= 0.5;
	dv = motionUV().atNXYZC( 0, x, y+1, z, 0 )  // u(x,y+1)
	   - motionUV().atNXYZC( 0, x, y-1, z, 0 ); // u(x,y-1)
	dv *= 0.5;

        switch (_norm) {
        case 0:
                pixelEnergy += 0;
                pixelEnergy += 0;
                break;
        case 1:
                pixelEnergy += du;
                pixelEnergy += dv;
                break;
        case 2:
                pixelEnergy += (du*du);
                pixelEnergy += (dv*dv);
                break;
        }

	du = motionUV().atNXYZC( 1, x+1, y, z, 0 )  // v(x+1,y)
	   - motionUV().atNXYZC( 1, x-1, y, z, 0 ); // v(x-1,y)
	du *= 0.5;
	dv = motionUV().atNXYZC( 1, x, y+1, z, 0 )  // v(x,y+1)
	   - motionUV().atNXYZC( 1, x, y-1, z, 0 ); // v(x,y-1)
	dv *= 0.5;

	switch (_norm) {
	case 0:
		pixelEnergy += 0;
		pixelEnergy += 0;
		break;
	case 1:
		pixelEnergy += du;
		pixelEnergy += dv;
		break;
	case 2:
		pixelEnergy += (du*du);
		pixelEnergy += (dv*dv);
		break;
	}

	ret = T(this->lambda()*pixelEnergy);

        return ret;
}

template <class T>
std::vector<T> EnergyClassic<T>::getEnergyGradient(
  int, int x, int y, int z, int )
{
	std::vector<T> ret( 2 );

	T pixelGradientU, pixelGradientV;  //  energy gradients wrt the "current" pixel x_k
                                           //  for example dE(u(x_k)) / du(x_k)

if (_norm == 2) {
	pixelGradientU =  0.0;
	pixelGradientU += 4.0
	 * motionUV().atNXYZC( 0, x, y, z, 0 );  //  4 * u(x,y)
	pixelGradientU -=
         motionUV().atNXYZC( 0, x-1, y, z, 0 );  //  - u(x-1,  y)
	pixelGradientU -=
	 motionUV().atNXYZC( 0, x+1, y, z, 0 );  //  - u(x+1,  y)
	pixelGradientU -=
	 motionUV().atNXYZC( 0, x, y-1, z, 0 );  //  - u(x  ,y-1)
	pixelGradientU -=
	 motionUV().atNXYZC( 0, x, y+1, z, 0 );  //  - u(x  ,y+1)

	pixelGradientV =  0.0;
	pixelGradientV += 4.0
	 * motionUV().atNXYZC( 1, x, y, z, 0 );  //  4 * v(x,y)
	pixelGradientV -=
	 motionUV().atNXYZC( 1, x-1, y, z, 0 );  //  - v(x-1,  y)
	pixelGradientV -=
	 motionUV().atNXYZC( 1, x+1, y, z, 0 );  //  - v(x+1,  y)
	pixelGradientV -=
	 motionUV().atNXYZC( 1, x, y-1, z, 0 );  //  - v(x  ,y-1)
	pixelGradientV -=
	 motionUV().atNXYZC( 1, x, y+1, z, 0 );  //  - v(x  ,y+1)
} else {
	pixelGradientU = 0;
	pixelGradientV = 0;
}

	switch (_norm) {
	case 0:
		ret[0] = T( 0 );
		ret[1] = T( 0 );
		break;
	case 1:
		ret[0] = T( 1 );
		ret[1] = T( 1 );
		break;
	case 2:
		ret[0] = T( this->lambda()*pixelGradientU );
		ret[1] = T( this->lambda()*pixelGradientV );
		break;
	}

        return ret;
}

template <class T>
int EnergyClassic<T>::getGradientComponentsCnt() { return 2; }

template <class T>
EnergyClassic<T>::~EnergyClassic()
{
}

#endif /* _ENERGYCLASSIC_HXX_ */

