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
/** \file EnergyBCC.hxx
 *  Implementation of the parameter class EnergyBCC.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.10.2011
 */

#ifndef _ENERGYBCC_HXX_
#define _ENERGYBCC_HXX_

#include "EnergyBCC.h"

#include <charon/AbstractStencil.hxx>

#include <charon/PenaltyFunction.hxx>
#include <cstdlib>
#include <cmath>

template <class T>
EnergyBCC<T>::EnergyBCC(const std::string& name) :
  AbstractStencil::Base<T>(
	     "EnergyBCC", name,
	     "<h2>Implementation of the brightness constancy constraint."
	     )
{
	this->_addInputSlot(penaltyFunction,
	                    "penaltyFunction",
	                    "penalty function",
	                    "PenaltyFunction<T>*");

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

  _lamb = this->lambda();
  _penaltyFunction = penaltyFunction();
}

template <class T>
T EnergyBCC<T>::getEnergy( int, int xI, int yI, int zI, int )
{
        T Ix = img_dx().atNXYZC( 0, xI, yI, zI, 0 );
        T Iy = img_dy().atNXYZC( 0, xI, yI, zI, 0 );
        T It = img_dt().atNXYZC( 0, xI, yI, zI, 0 );

        T u = motionUV().atNXYZC( 0, xI, yI, zI, 0 );
        T v = motionUV().atNXYZC( 1, xI, yI, zI, 0 );

	T energy = _penaltyFunction->getPenalty( It + Ix*u + Iy*v );

	return T(this->_lamb * energy);
}

template <class T>
std::vector<T> EnergyBCC<T>::getEnergyGradient( int, int xI, int yI, int zI, int )
{
        T Ix = img_dx().atNXYZC( 0, xI, yI, zI, 0 );
        T Iy = img_dy().atNXYZC( 0, xI, yI, zI, 0 );
        T It = img_dt().atNXYZC( 0, xI, yI, zI, 0 );

        T u = motionUV().atNXYZC( 0, xI, yI, zI, 0 );
        T v = motionUV().atNXYZC( 1, xI, yI, zI, 0 );

        T tmp = _penaltyFunction->getPenaltyGradient( It + Ix*u + Iy*v );
	T energyGradientU = Ix * tmp;
	T energyGradientV = Iy * tmp;

	std::vector<T> ret( 2, T(0.0) );
	ret[0] = T(this->_lamb * energyGradientU);
	ret[1] = T(this->_lamb * energyGradientV);

        return ret;
}

template <class T>
int EnergyBCC<T>::getEnergyGradientDimensions() { return 2; }

template <class T>
EnergyBCC<T>::~EnergyBCC()
{
}

#endif /* _ENERGYBCC_HXX_ */

