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
/** \file EnergyBCC.hxx
 *  Implementation of the parameter class EnergyBCC.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.10.2011
 */

#ifndef _ENERGYBCC_HXX_
#define _ENERGYBCC_HXX_

#include "EnergyBCC.h"

#include <charon/EnergyStencil.hxx>

#include <cstdlib>

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
	this->_addInputSlot(motionUV,
	                    "motionUV",
                            "current motion components",
	                    "CImgList<T>");
}

template <class T>
void EnergyBCC<T>::execute() {
  PARAMETEREDOBJECT_AVOID_REEXECUTION;
  ParameteredObject::execute();

  cimg_library::CImgList<T> tmp = motionUV();
  tmp.save( "/home/mbaron/tmp/motionUV.cimg" );
}

template <class T>
T EnergyBCC<T>::getEnergy( int, int xI, int yI, int zI, int )
{
//	std::cout << "(II) EnergyBCC :: called" << std::endl;
	T energy;
        T Ix, Iy, It;
        T u, v;

        Ix = img_dx().atNXYZC( 0, xI, yI, zI, 0 );
        Iy = img_dy().atNXYZC( 0, xI, yI, zI, 0 );
        It = img_dt().atNXYZC( 0, xI, yI, zI, 0 );

        u = motionUV().atNXYZC( 0, xI, yI, zI, 0 );
        v = motionUV().atNXYZC( 1, xI, yI, zI, 0 );

//	if (u || v) {
//		std::cout << "(II) EnergyBCC :: u = " << u << std::endl;
//		std::cout << "(II) EnergyBCC :: v = " << v << std::endl;
//	}

        energy = It + Ix*u + Iy*v;

	return T(this->lambda() * energy * energy);
}

template <class T>
std::vector<T> EnergyBCC<T>::getEnergyGradient( int, int xI, int yI, int zI, int )
{
	std::vector<T> ret( 2, T(0.0) );
	T tmp, pixelGradientU, pixelGradientV;
        T Ix, Iy, It;
        T u, v;

        Ix = img_dx().atNXYZC( 0, xI, yI, zI, 0 );
        Iy = img_dy().atNXYZC( 0, xI, yI, zI, 0 );
        It = img_dt().atNXYZC( 0, xI, yI, zI, 0 );

        u = motionUV().atNXYZC( 0, xI, yI, zI, 0 );
        v = motionUV().atNXYZC( 1, xI, yI, zI, 0 );

        tmp = It + Ix*u + Iy*v;

	pixelGradientU = 2.0 * Ix * tmp;
	pixelGradientV = 2.0 * Iy * tmp;

	ret[0] = T(this->lambda() * pixelGradientU);
	ret[1] = T(this->lambda() * pixelGradientV);

        return ret;
}

template <class T>
int EnergyBCC<T>::getGradientComponentsCnt() { return 2; }

template <class T>
EnergyBCC<T>::~EnergyBCC()
{
}

#endif /* _ENERGYBCC_HXX_ */

