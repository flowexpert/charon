/*  Copyright (C) 2011 Michael Baron

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
 *  \date 30.08.2011
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
}

template <class T>
void EnergyClassic<T>::execute() {
  PARAMETEREDOBJECT_AVOID_REEXECUTION;
  ParameteredObject::execute();

}

template <class T>
int EnergyClassic<T>::_linearIndex( int n, int x, int y, int z, int c,
                                 int pSize, int pWidth, int pHeight, int pDepth, int pSpectrum )
{
        int ret;

        if (n < 0) n = 0; if (n > pSize-1) n = pSize-1;
        if (x < 0) x = 0; if (x > pWidth-1) x = pWidth-1;
        if (y < 0) y = 0; if (y > pHeight-1) y = pHeight-1;
        if (z < 0) z = 0; if (z > pDepth-1) z = pDepth-1;
        if (c < 0) c = 0; if (c > pSpectrum-1) c = pSpectrum-1;

        ret = c
            + z * pSpectrum
            + y * pSpectrum * pDepth
            + x * pSpectrum * pDepth * pHeight
            + n * pSpectrum * pDepth * pHeight * pWidth ;

        return ret;
}

template <class T>
std::vector<T> EnergyClassic<T>::getEnergy(
                    std::vector<T> motionUV,
                    int pSize,
                    int pWidth, int pHeight, int pDepth,
                    int pSpectrum )
{
        T dx, dy;
	T pixelEnergy;    //  energy per pixel

	int energySize = pWidth * pHeight * pDepth * pSpectrum;
	std::vector<T> ret( energySize, T(0) ); // (!!)

	int xI, yI, zI;
	for (xI = 0; xI < pWidth; ++xI)
	for (yI = 0; yI < pHeight; ++yI)
	for (zI = 0; zI < pDepth; ++zI)
	{
	        pixelEnergy = 0.0;
		dx = motionUV[_linearIndex(0, xI+1, yI, zI, 0,
		     pSize, pWidth, pHeight, pDepth, pSpectrum)]  // u(x+1,y)
		   - motionUV[_linearIndex(0, xI-1, yI, zI, 0,
		     pSize, pWidth, pHeight, pDepth, pSpectrum)]; // u(x-1,y)
		dx *= 0.5;
		dy = motionUV[_linearIndex(0, xI, yI+1, zI, 0,
		     pSize, pWidth, pHeight, pDepth, pSpectrum)]  // u(x,y+1)
		   - motionUV[_linearIndex(0, xI, yI-1, zI, 0,
		     pSize, pWidth, pHeight, pDepth, pSpectrum)]; // u(x,y-1)
		dy *= 0.5;
		pixelEnergy += (dx*dx);
		pixelEnergy += (dy*dy);

		dx = motionUV[_linearIndex(1, xI+1, yI, zI, 0,
		     pSize, pWidth, pHeight, pDepth, pSpectrum)]  // u(x+1,y)
		   - motionUV[_linearIndex(1, xI-1, yI, zI, 0,
		     pSize, pWidth, pHeight, pDepth, pSpectrum)]; // u(x-1,y)
		dx *= 0.5;
		dy = motionUV[_linearIndex(1, xI, yI+1, zI, 0,
		     pSize, pWidth, pHeight, pDepth, pSpectrum)]  // u(x,y+1)
		   - motionUV[_linearIndex(1, xI, yI-1, zI, 0,
		     pSize, pWidth, pHeight, pDepth, pSpectrum)]; // u(x,y-1)
		dy *= 0.5;
		pixelEnergy += (dx*dx);
		pixelEnergy += (dy*dy);

                ret[_linearIndex(0, xI, yI, zI, 0, pSize, pWidth, pHeight, pDepth, pSpectrum)] = T(this->lambda()*pixelEnergy);
	}

        return ret;
}

template <class T>
std::vector<T> EnergyClassic<T>::getEnergyGradient(
                    std::vector<T> motionUV,
                    int pSize,
                    int pWidth, int pHeight, int pDepth,
                    int pSpectrum )
{
	int energyGradientSize = pSize * pWidth * pHeight * pDepth * pSpectrum;
	std::vector<T> ret( energyGradientSize, T(0) );

	T pixelGradientU, pixelGradientV;  //  energy gradients wrt the "current" pixel x_k
                                           //  for example dE(u(x_k)) / du(x_k)

	int xI, yI, zI;
	for (xI = 0; xI < pWidth; ++xI)
	for (yI = 0; yI < pHeight; ++yI)
	for (zI = 0; zI < pDepth; ++zI)
	{
                pixelGradientU =  0.0;
                pixelGradientU += 4.0
                               *  motionUV[_linearIndex(0, xI, yI, zI, 0,
                                  pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  4 * u(x,y)
                pixelGradientU -= motionUV[_linearIndex(0, xI-1, yI, zI, 0,
                                  pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  - u(x-1,  y)
                pixelGradientU -= motionUV[_linearIndex(0, xI+1, yI, zI, 0,
                                  pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  - u(x+1,  y)
                pixelGradientU -= motionUV[_linearIndex(0, xI, yI-1, zI, 0,
                                  pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  - u(x  ,y-1)
                pixelGradientU -= motionUV[_linearIndex(0, xI, yI+1, zI, 0,
                                  pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  - u(x  ,y+1)

                pixelGradientV =  0.0;
                pixelGradientV += 4.0
                               *  motionUV[_linearIndex(1, xI, yI, zI, 0,
                                  pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  4 * v(x,y)
                pixelGradientV -= motionUV[_linearIndex(1, xI-1, yI, zI, 0,
                                  pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  - v(x-1,  y)
                pixelGradientV -= motionUV[_linearIndex(1, xI+1, yI, zI, 0,
                                  pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  - v(x+1,  y)
                pixelGradientV -= motionUV[_linearIndex(1, xI, yI-1, zI, 0,
                                  pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  - v(x  ,y-1)
                pixelGradientV -= motionUV[_linearIndex(1, xI, yI+1, zI, 0,
                                  pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  - v(x  ,y+1)

		ret[_linearIndex(0, xI, yI, zI, 0,
		    pSize, pWidth, pHeight, pDepth, pSpectrum)] =
		T( this->lambda()*pixelGradientU );
		ret[_linearIndex(1, xI, yI, zI, 0,
		    pSize, pWidth, pHeight, pDepth, pSpectrum)] =
		T( this->lambda()*pixelGradientV );
	}

        return ret;
}

template <class T>
EnergyClassic<T>::~EnergyClassic()
{
}

#endif /* _ENERGYCLASSIC_HXX_ */

