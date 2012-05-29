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
/** \file EnergyCoupling.hxx
 *  Implementation of the parameter class EnergyCoupling.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 24.11.2011
 */

#ifndef _ENERGYCOUPLING_HXX_
#define _ENERGYCOUPLING_HXX_

#include "EnergyCoupling.h"

#include <charon/Stencil.hxx>

template <class T>
EnergyCoupling<T>::EnergyCoupling(const std::string& name) :
	Stencil::Base<T>(
		"EnergyCoupling", name, "<h2>Coupling of two motion fields</h2>."
	)
{
	this->_addInputSlot(penaltyFunction,
	                    "penaltyFunction",
	                    "penalty function",
	                    "PenaltyFunction<T>*");

	this->_addInputSlot(firstMotionUV,"firstMotionUV",
			"first flow field", "CImgList<T>");
	this->_addInputSlot(secondMotionUV, "secondMotionUV",
			"second flow field", "CImgList<T>");
}

template <class T>
void EnergyCoupling<T>::execute() {
	Stencil::Base<T>::execute();
	_lamb = this->lambda();
	_penaltyFunction = penaltyFunction();
}

template <class T>
T EnergyCoupling<T>::getEnergy( int, int xI, int yI, int zI, int )
{
	T u1 = firstMotionUV().atNXYZC( 0, xI, yI, zI, 0 );
	T v1 = firstMotionUV().atNXYZC( 1, xI, yI, zI, 0 );
	T u2 = secondMotionUV().atNXYZC( 0, xI, yI, zI, 0 );
	T v2 = secondMotionUV().atNXYZC( 1, xI, yI, zI, 0 );

	T energy = _penaltyFunction->getPenalty( pow(double(u1 - u2), 2.0) )
	         + _penaltyFunction->getPenalty( pow(double(v1 - v2), 2.0) );

	return T(_lamb * energy);
}

template <class T>
std::vector<T> EnergyCoupling<T>::getEnergyGradient(
		int, int xI, int yI, int zI, int)
{
	T u1 = firstMotionUV().atNXYZC( 0, xI, yI, zI, 0 );
	T v1 = firstMotionUV().atNXYZC( 1, xI, yI, zI, 0 );
	T u2 = secondMotionUV().atNXYZC( 0, xI, yI, zI, 0 );
	T v2 = secondMotionUV().atNXYZC( 1, xI, yI, zI, 0 );

	// IMPORTANT! Gradients wrt u1 resp. v1 !
	T energyGradientU1 = _penaltyFunction->getPenaltyGradient( pow(double(u1 - u2), 2.0) );
	T energyGradientV1 = _penaltyFunction->getPenaltyGradient( pow(double(v1 - v2), 2.0) );

	std::vector<T> ret( 2, T(0.0) );
	ret[0] = T(_lamb * energyGradientU1);
	ret[1] = T(_lamb * energyGradientV1);

	return ret;
}

template <class T>
std::vector<T> EnergyCoupling<T>::getEnergyHessian(
		int, int xI, int yI, int zI, int)
{
	T u1 = firstMotionUV().atNXYZC( 0, xI, yI, zI, 0 );
	T v1 = firstMotionUV().atNXYZC( 1, xI, yI, zI, 0 );
	T u2 = secondMotionUV().atNXYZC( 0, xI, yI, zI, 0 );
	T v2 = secondMotionUV().atNXYZC( 1, xI, yI, zI, 0 );

	T energyHessianU1U1 = _penaltyFunction->getPenaltyHessian( pow(double(u1 - u2), 2.0) );
	T energyHessianU1V1 = T(0.0);
	T energyHessianV1V1 = _penaltyFunction->getPenaltyHessian( pow(double(v1 - v2), 2.0) );

	std::vector<T> ret( 4, T(0.0) );
	ret[0] = T(_lamb * energyHessianU1U1);
	ret[1] = T(_lamb * energyHessianU1V1);
	ret[2] = T(_lamb * energyHessianU1V1);
	ret[3] = T(_lamb * energyHessianV1V1);

	return ret;
}

template <class T>
int EnergyCoupling<T>::getEnergyGradientDimensions() { return 2; }

#endif /* _ENERGYCOUPLING_HXX_ */

