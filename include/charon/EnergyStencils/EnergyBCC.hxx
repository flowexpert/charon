/*  Copyright (C) 2011, 2012 
                  Heidelberg Collaboratory for Image Processing

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

#include <charon/Stencil.hxx>

#include <charon/PenaltyFunction.hxx>
#include <cstdlib>
#include <cmath>

template <class T>
EnergyBCC<T>::EnergyBCC(const std::string& name) :
	Stencil::Base<T>(
	     "EnergyBCC", name,
	     "<h2>Implementation of the brightness constancy constraint."
	     ),
	motionUV(true,false)
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

        ParameteredObject::_addParameter(
                        pUnknowns, "unknowns", "List of unknowns");

}

template <class T>
void EnergyBCC<T>::execute() {
	Stencil::Base<T>::execute();
	_lamb = this->lambda();
	_penaltyFunction = penaltyFunction();

        // Copy the unknowns from the Parameter list into the set, which was
        // inherited from the Stencil class
        std::vector<std::string>::iterator puIt;
        for(puIt=pUnknowns().begin() ; puIt!=pUnknowns().end() ; puIt++) {
                Point4D<int> center;
                SubStencil<T> entry(1,1,1,1,center);
                entry.pattern(0,0) = 1;
                this->_subStencils[*puIt] = entry;
                this->_unknowns.insert(*puIt);
        }

        _patternMask.assign(1,1,1,1);
        _dataMask.assign(1,1,1,1);

        _patternMask.fill( 1 );
        _dataMask.fill( 1 );
        _center = Point4D<int>(0,0,0,0);

        _dataMask *= this->_lamb;
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
std::vector<T> EnergyBCC<T>::getEnergyHessian( int, int xI, int yI, int zI, int )
{
	T Ix = img_dx().atNXYZC( 0, xI, yI, zI, 0 );
	T Iy = img_dy().atNXYZC( 0, xI, yI, zI, 0 );
	T It = img_dt().atNXYZC( 0, xI, yI, zI, 0 );

	T u = motionUV().atNXYZC( 0, xI, yI, zI, 0 );
	T v = motionUV().atNXYZC( 1, xI, yI, zI, 0 );

	T tmp = _penaltyFunction->getPenaltyHessian( It + Ix*u + Iy*v );
	T energyHessianUU = Ix * Ix * tmp;
	T energyHessianUV = Ix * Iy * tmp;
	T energyHessianVV = Iy * Iy * tmp;

	std::vector<T> ret( 4, T(0.0) );
	ret[0] = T(this->_lamb * energyHessianUU);
	ret[1] = T(this->_lamb * energyHessianUV);
	ret[2] = T(this->_lamb * energyHessianUV);
	ret[3] = T(this->_lamb * energyHessianVV);

	return ret;
}

template <class T>
int EnergyBCC<T>::getEnergyGradientDimensions() { return 2; }

template <class T>
void EnergyBCC<T>::updateStencil(
                const std::string& unknown,
                const Point4D<int>& p, const int&)
{
        const bool isU = (unknown == "a1");

        const unsigned int x = p.x;
        const unsigned int y = p.y;
        const unsigned int z = p.z;

        // current values of ix,iy,it
        const T cix = img_dx()(0,x,y,z,0);
        const T ciy = img_dy()(0,x,y,z,0);
        const T cit = img_dt()(0,x,y,z,0);
        const T cik = isU ? cix : ciy;

        // weighted Horn&Schunck equations
        double dataU = cik * cix;
        double dataV = cik * ciy;
        double rhs   = cik * (-cit);

	if (motionUV.connected()) {
        	// initial flow guess from previous iteration
	        const T u0 = motionUV()[0](x,y,z);
	        const T v0 = motionUV()[1](x,y,z);
		rhs += u0*dataU+v0*dataV;
	}

        // fill calculated data into stencil members, applying lambda
        const T      l  = this->lambda();
        this->_subStencils["a1"].data(0,0) = l * T(dataU);
        this->_subStencils["a2"].data(0,0) = l * T(dataV);
        this->_rhs  = l * T(rhs);
}

template <class T>
cimg_library::CImg<T> EnergyBCC<T>::apply(
                const cimg_library::CImgList<T>& seq,
                const unsigned int frame) const
{
        return seq[frame];  //  not implemented yet (!!)
}

#endif /* _ENERGYBCC_HXX_ */

