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
/** \file EnergyBCC.hxx
 *  Implementation of the parameter class EnergyBCC.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 30.08.2011
 */

#ifndef _ENERGYBCC_HXX_
#define _ENERGYBCC_HXX_

#include "EnergyBCC.h"

#include <charon/EnergyStencil.hxx>

/**
 *  This energy stencil implements the brightness constancy constraint.
 *  Its energy equals (I_x * u + I_y * v + I_t)^2.
 *  Its partial derivatives wrt u/v (constituing its gradient) are
 *    2 * I_x * (I_x * u + I_y * v + I_t)
 *    2 * I_y * (I_x * u + I_y * v + I_t)
 */

template <class T>
EnergyBCC<T>::EnergyBCC(const std::string& name) :
  EnergyStencil<T>(
	     "EnergyBCC", name,
	     "<h2>Implementation of the brightness constancy constraint."
	     )
{
        this->_addInputSlot(img_dx,
                            "img_dx",
                            "derivatives wrt x/u of input images",
                            "CImgList<T>");
        this->_addInputSlot(img_dy,
                            "img_dy",
                            "derivatives wrt y/v of input images",
                            "CImgList<T>");
	this->_addInputSlot(img_dt,
	                    "img_dt",
	                    "derivatives wrt t of input images",
	                    "CImgList<T>");
}

template <class T>
void EnergyBCC<T>::execute() {
  PARAMETEREDOBJECT_AVOID_REEXECUTION;
  ParameteredObject::execute();

}

template <class T>
int EnergyBCC<T>::_linearIndex( int n, int x, int y, int z, int c,
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
std::vector<T> EnergyBCC<T>::getEnergy(
                    std::vector<T> motionUV,
                    int pSize,
                    int pWidth, int pHeight, int pDepth,
                    int pSpectrum )
{
        int energySize = pWidth * pHeight * pDepth * pSpectrum;
        std::vector<T> ret( energySize, T(0) );

	T pixelEnergy;

        int xI, yI, zI;
        for (xI = 0; xI < pWidth; ++xI)
        for (yI = 0; yI < pHeight; ++yI)
        for (zI = 0; zI < pDepth; ++zI)
	{
		pixelEnergy =  0.0;
                pixelEnergy += img_dt().atNXYZC( 0, xI, yI, zI, 0 );  //  I_t
                pixelEnergy += img_dx().atNXYZC( 0, xI, yI, zI, 0 )
                            *  motionUV[_linearIndex(0, xI, yI, zI, 0,
                               pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  u * I_x
                pixelEnergy += img_dy().atNXYZC( 0, xI, yI, zI, 0 )
                            *  motionUV[_linearIndex(1, xI, yI, zI, 0,
                               pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  v * I_y
                ret[_linearIndex(0, xI, yI, zI, 0, pSize, pWidth, pHeight, pDepth, pSpectrum)] = T(pixelEnergy * pixelEnergy);
	}

        return ret;
}

template <class T>
std::vector<T> EnergyBCC<T>::getEnergyGradient(
                    std::vector<T> motionUV,
                    int pSize,
                    int pWidth, int pHeight, int pDepth,
                    int pSpectrum )
{
        int energyGradientSize = pSize * pWidth * pHeight * pDepth * pSpectrum;
        std::vector<T> ret( energyGradientSize, T(0) );

	T tmp, pixelGradientU, pixelGradientV;

        int xI, yI, zI;
        for (xI = 0; xI < pWidth; ++xI)
        for (yI = 0; yI < pHeight; ++yI)
        for (zI = 0; zI < pDepth; ++zI)
	{
		pixelGradientU = 0.0;
		pixelGradientV = 0.0;
                tmp =  0.0;
                tmp += img_dt().atNXYZC( 0, xI, yI, zI, 0 );  //  I_t
                tmp += img_dx().atNXYZC( 0, xI, yI, zI, 0 )
                    *  motionUV[_linearIndex(0, xI, yI, zI, 0,
                       pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  + u * I_x
                tmp += img_dy().atNXYZC( 0, xI, yI, zI, 0 )
                    *  motionUV[_linearIndex(1, xI, yI, zI, 0,
                       pSize, pWidth, pHeight, pDepth, pSpectrum)];  //  + v * I_y ) ...
                pixelGradientU += tmp * img_dx().atNXYZC( 0, xI, yI, zI, 0 ) * 2.0;  //  ... * I_x * 2.0
                pixelGradientV += tmp * img_dy().atNXYZC( 0, xI, yI, zI, 0 ) * 2.0;  //  ... * I_y * 2.0

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
EnergyBCC<T>::~EnergyBCC()
{
}

#endif /* _ENERGYBCC_HXX_ */

