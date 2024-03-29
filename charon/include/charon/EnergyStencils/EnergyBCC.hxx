/*  Copyright (C) 2011, 2012, 2013
                  Heidelberg Collaboratory for Image Processing

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
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
		"Stencil implementing the brightness constancy constraint. "
		"It can be connected either to linear system solvers, "
		"where it acts in building up the individual Euler-Lagrange equations "
		"for each considered position or used in the context of energy optimizers, "
		"where it yields the residual energy, its gradient and Hessian matrix at each "
		"position. "
		"The necessary image gradients have to be connected via their respective input slots "
		"img_dx, img_dy and img_dt. "
		"Additionally, a so-called penalty-function, which corresponds to the psi-function "
		"has to be connected via the penaltyFunction input slot. "
		"In order to use this stencil for <i>gradient constancy</i>, we can also connect "
		"derivatives of the image </i>derivatives</i> to it. "
		"The optional input slot represents a right-hand-side motion, which has to be "
		"conncected, if the stencil is being used in the context of warping. "
		"In order to weight constancy individually at each position, e.g. "
		"in the context of bootstrapping, "
		"an optional lambdaMask can be connected. "
	     ),
	motionUV(true,false),
	mask(true,false)
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

	this->_addInputSlot(roi, "roi", "region of interest", "Roi<int>*");
	this->_addInputSlot(mask, "mask", "regularization mask ", "CImgList<T>");

        ParameteredObject::_addParameter(
                        pUnknowns, "unknowns", "List of unknowns");

	ParameteredObject::_setTags("charon-flow;Stencils;CImg");
}

template <class T>
void EnergyBCC<T>::execute() {
	Stencil::Base<T>::execute();

	_lamb = this->lambda();
	if (this->lambdaMask.connected()) {
		_lambdaMask = this->lambdaMask();
	}
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
}

template <class T>
T EnergyBCC<T>::getEnergy( int, int xI, int yI, int zI, int )
{
	if (this->lambdaMask.connected())
		_lamb *= _lambdaMask.atNXYZC( 0, xI, yI, zI, 0 );

	T Ix = img_dx().atNXYZC( 0, xI, yI, zI, 1 );
	T Iy = img_dy().atNXYZC( 0, xI, yI, zI, 1 );
	T It = img_dt().atNXYZC( 0, xI, yI, zI, 0 ); // (!!) works for temporal _difference_ [+1,-1] only

	T u = motionUV().atNXYZC( 0, xI, yI, zI, 0 );
	T v = motionUV().atNXYZC( 1, xI, yI, zI, 0 );

	T energy = _penaltyFunction->getPenalty( -1, xI, yI, zI, -1, pow(double(It + Ix*u + Iy*v), 2.0) );

	return T(this->_lamb * energy);
}

template <class T>
std::vector<T> EnergyBCC<T>::getEnergyGradient( int, int xI, int yI, int zI, int )
{
	if (this->lambdaMask.connected())
		_lamb *= _lambdaMask.atNXYZC( 0, xI, yI, zI, 0 );

        T Ix = img_dx().atNXYZC( 0, xI, yI, zI, 1 );
        T Iy = img_dy().atNXYZC( 0, xI, yI, zI, 1 );
        T It = img_dt().atNXYZC( 0, xI, yI, zI, 0 ); // (!!) works for temporal _difference_ [+1,-1] only

        T u = motionUV().atNXYZC( 0, xI, yI, zI, 0 );
        T v = motionUV().atNXYZC( 1, xI, yI, zI, 0 );

	T tmp  = It + Ix*u + Iy*v ;
        T tmp2 = _penaltyFunction->getPenaltyGradient( -1, xI, yI, zI, -1, pow(double(tmp), 2.0) );
	T tmp3 = tmp * tmp2;

	T energyGradientU = Ix * tmp3;
	T energyGradientV = Iy * tmp3;

	std::vector<T> ret( 2, T(0.0) );
	ret[0] = T(this->_lamb * energyGradientU);
	ret[1] = T(this->_lamb * energyGradientV);

        return ret;
}

template <class T>
std::vector<T> EnergyBCC<T>::getEnergyHessian( int, int xI, int yI, int zI, int )
{
	if (this->lambdaMask.connected())
		_lamb *= _lambdaMask.atNXYZC( 0, xI, yI, zI, 0 );

	T Ix = img_dx().atNXYZC( 0, xI, yI, zI, 1 );
	T Iy = img_dy().atNXYZC( 0, xI, yI, zI, 1 );
	T It = img_dt().atNXYZC( 0, xI, yI, zI, 0 ); // (!!) works for temporal _difference_ [+1,-1] only

	T u = motionUV().atNXYZC( 0, xI, yI, zI, 0 );
	T v = motionUV().atNXYZC( 1, xI, yI, zI, 0 );

	T tmp  = pow(double(It + Ix*u + Iy*v), 2.0) ;
	T tmp2 = _penaltyFunction->getPenaltyGradient( -1, xI, yI, zI, -1, tmp );
	T tmp3 = _penaltyFunction->getPenaltyHessian( -1, xI, yI, zI, -1, tmp );
	T tmp4 = tmp2 + tmp * tmp3 ;

	T energyHessianUU = Ix * Ix * tmp4;
	T energyHessianUV = Ix * Iy * tmp4;
	T energyHessianVV = Iy * Iy * tmp4;

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

	T l;
	if (this->lambdaMask.connected()) {
		l = this->lambda() * _lambdaMask.atNXYZC( 0, x, y, z, 0 );
	} else {
		l = this->lambda();
	}

	bool _mask = true;
	if (mask.connected()) _mask =  mask()[0].atXY(x,y);

	if (  (_xBegin > p.x) || (p.x >= _xEnd) || (_yBegin > p.y) || (p.y >= _yEnd)
	   || !_mask ) {
		this->_subStencils["a1"].data(0,0) = T(0.0);
		this->_subStencils["a2"].data(0,0) = T(0.0);
		this->_rhs = T(0.0);
		return;
	}

        // current values of ix,iy,it
        const T cix = img_dx()(0,x,y,z,1);
        const T ciy = img_dy()(0,x,y,z,1);
        const T cit = img_dt()(0,x,y,z,0); // (!!) works for temporal _difference_ [+1,-1] only
        const T cik = isU ? cix : ciy;

        // weighted Horn&Schunck equations
        double dataU = cik * cix;
        double dataV = cik * ciy;
        double rhs   = cik * (-cit);

	// penalty function derivative
	T d_psi = 1.0;

	if (motionUV.connected()) {
        	// initial flow guess from previous iteration
	        const T u0 = motionUV()[0](x,y,z);
	        const T v0 = motionUV()[1](x,y,z);

		d_psi = _penaltyFunction->getPenaltyGradient( -1, x, y, z, -1, pow(double(cit + cix*u0 + ciy*v0), 2.0) );
		rhs -= (cik * cix * u0 + cik * ciy * v0);
	} else {
		d_psi = _penaltyFunction->getPenaltyGradient( -1, x, y, z, -1, pow(double(cit), 2.0) );
	}

        // fill calculated data into stencil members, applying lambda
        this->_subStencils["a1"].data(0,0) = l * d_psi * T(dataU);
        this->_subStencils["a2"].data(0,0) = l * d_psi * T(dataV);
        this->_rhs  = l * d_psi * T(rhs);
}

template <class T>
cimg_library::CImg<T> EnergyBCC<T>::apply(
                const cimg_library::CImgList<T>& seq,
                const unsigned int frame) const
{
        return seq[frame];  //  not implemented yet (!!)
}

#endif /* _ENERGYBCC_HXX_ */

