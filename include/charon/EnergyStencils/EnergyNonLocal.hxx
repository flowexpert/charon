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
/** \file EnergyNonLocal.hxx
 *  Implementation of the parameter class EnergyNonLocal.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.12.2011
 */

#ifndef _ENERGYNONLOCAL_HXX_
#define _ENERGYNONLOCAL_HXX_

#include <charon/Stencil.hxx>

#include "EnergyNonLocal.h"

#include <charon/PenaltyFunction.hxx>

#include <cmath>
#define M_PI 3.14159265358979323846

/**
 *  This energy stencil implements regularization.
 */

template <class T>
EnergyNonLocal<T>::EnergyNonLocal(const std::string& name) :
	Stencil::Base<T>(
		"EnergyNonLocal", name,
		"<h2>Example for an EnergyStencil.")
{
	ParameteredObject::_addParameter< bool >(useWeight,
			"useWeight",
			"use weighted median (1) or not (0)",
			true);
	ParameteredObject::_addParameter< int >(radius,
			"radius",
			"radius of the neighborhood",
			10, "int");
	ParameteredObject::_addParameter< T >(sigma_spatial,
			"sigma_spatial",
			"spatial difference weight",
			7, "T");
	ParameteredObject::_addParameter< T >(sigma_color,
			"sigma_color",
			"color difference weight",
			7, "T");
	ParameteredObject::_addParameter< T >(sigma_occ_divergence,
			"sigma_occ_divergence",
			"occlusion divergence weight",
			0.3, "T");
	ParameteredObject::_addParameter< T >(sigma_occ_color,
			"sigma_occ_color",
			"occlusion color difference weight",
			20, "T");

	this->_addInputSlot(penaltyFunction,
			"penaltyFunction",
			"penalty function",
			"PenaltyFunction<T>*");
	this->_addInputSlot(img,
			"img",
			"list of input images",
			"CImgList<T>");
	this->_addInputSlot(motionUV,
		"motionUV",
		"current motion components",
		"CImgList<T>");
}

template <class T>
void EnergyNonLocal<T>::execute() {
	Stencil::Base<T>::execute();
	_lamb = this->lambda();
	_penaltyFunction = this->penaltyFunction();
	_useWeight = useWeight();
	_radius = radius();
	_sigma_spatial = sigma_spatial();
	_sigma_color = sigma_color();
	_sigma_occ_divergence = sigma_occ_divergence();
	_sigma_occ_color = sigma_occ_color();
}

template <class T>
T EnergyNonLocal<T>::getEnergy( int, int xI, int yI, int zI, int )
{
  T pixelEnergy;    //  energy per pixel
  T weight_sum;
  T weight, spatial_weight, color_weight, occlusion_weight;
  T color_diff;

  T du, dv, dxU, dyV, divergence, mot_comp_diff, motX, motY;
  int dxI, dyI, dzI;

  pixelEnergy = 0.0;

  weight_sum = 0.0;
  for (dxI = -_radius; dxI < _radius+1; ++dxI)
  for (dyI = -_radius; dyI < _radius+1; ++dyI)
  for (dzI = -0; dzI < 0+1; ++dzI)
  {
    du = motionUV().atNXYZC( 0, xI,     yI,     zI,     0 )
       - motionUV().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 );
    dv = motionUV().atNXYZC( 1, xI,     yI,     zI,     0 )
       - motionUV().atNXYZC( 1, xI+dxI, yI+dyI, zI+dzI, 0 );

    if (_useWeight) {
      dxU = (motionUV().atNXYZC( 0, xI+dxI+1, yI+dyI,   zI+dzI, 0 )
            -motionUV().atNXYZC( 0, xI+dxI-1, yI+dyI,   zI+dzI, 0 ))/2;
      dyV = (motionUV().atNXYZC( 1, xI+dxI,   yI+dyI+1, zI+dzI, 0 )
            -motionUV().atNXYZC( 1, xI+dxI,   yI+dyI-1, zI+dzI, 0 ))/2;
      divergence = dxU + dyV;
      if (divergence > 0) divergence = 0;

      spatial_weight = _gauss( sqrt(double(dxI*dxI + dyI*dyI)), 0, _sigma_spatial );
      color_diff = img().atNXYZC( 0, xI,     yI,     zI,     0 )
                 - img().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 );
      color_weight = _gauss( color_diff, 0, _sigma_color );

      motX = motionUV().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 );
      motY = motionUV().atNXYZC( 1, xI+dxI, yI+dyI, zI+dzI, 0 );
      mot_comp_diff = img().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 )
                    - img().atNXYZC( 0, xI+dxI+motX, yI+dyI+motY, zI+dzI, 1 );

      occlusion_weight =  _gauss( divergence, 0, _sigma_occ_divergence );
      occlusion_weight *= _gauss( mot_comp_diff, 0, _sigma_occ_color );

      weight = spatial_weight * color_weight * occlusion_weight;
      weight_sum += weight;

      pixelEnergy += weight * _penaltyFunction->getPenalty( fabs(double(du)) );
      pixelEnergy += weight * _penaltyFunction->getPenalty( fabs(double(dv)) );
    } else {
      pixelEnergy += _penaltyFunction->getPenalty( fabs(double(du)) );
      pixelEnergy += _penaltyFunction->getPenalty( fabs(double(dv)) );
      weight_sum += 1.0;
    }
  }
  if (weight_sum) {
    pixelEnergy /= weight_sum;
  } else {
    pixelEnergy = 0.0;
  }

  return T(_lamb * pixelEnergy);
}

template <class T>
T signum( T arg )
{
        return (arg < 0) ? T(-1) : T(1) ;
}

template <class T>
std::vector<T> EnergyNonLocal<T>::getEnergyGradient(
    int, int xI, int yI, int zI, int )
{
  std::vector<T> ret( 2, T(0) );

  //  energy gradients wrt the motion at "current" pixel x_k
  //  for example dE(u(x_k)) / du(x_k)
  T pixelGradientU, pixelGradientV;

  T weight_sum;
  T weight, spatial_weight, color_weight, occlusion_weight;
  T color_diff;

  T du, dv, dxU, dyV, divergence, mot_comp_diff, motX, motY;
  int dxI, dyI, dzI;

  pixelGradientU = 0.0;
  pixelGradientV = 0.0;

  weight_sum    = 0.0;
  for (dxI = -_radius; dxI < _radius+1; ++dxI)
  for (dyI = -_radius; dyI < _radius+1; ++dyI)
  for (dzI = -0; dzI < 0+1; ++dzI)
  {
    du = motionUV().atNXYZC( 0, xI,     yI,     zI,     0 )
       - motionUV().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 );
    dv = motionUV().atNXYZC( 1, xI,     yI,     zI,     0 )
       - motionUV().atNXYZC( 1, xI+dxI, yI+dyI, zI+dzI, 0 );

    if (_useWeight) {
      dxU = (motionUV().atNXYZC( 0, xI+dxI+1, yI+dyI,   zI+dzI, 0 )
            -motionUV().atNXYZC( 0, xI+dxI-1, yI+dyI,   zI+dzI, 0 ))/2;
      dyV = (motionUV().atNXYZC( 1, xI+dxI,   yI+dyI+1, zI+dzI, 0 )
            -motionUV().atNXYZC( 1, xI+dxI,   yI+dyI-1, zI+dzI, 0 ))/2;
      divergence = dxU + dyV;
      if (divergence > 0) divergence = 0;

      spatial_weight = _gauss( sqrt(double(dxI*dxI + dyI*dyI)), 0, _sigma_spatial );
      color_diff = img().atNXYZC( 0, xI,     yI,     zI,     0 )
                 - img().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 );
      color_weight = _gauss( color_diff, 0, _sigma_color );

      motX = motionUV().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 );
      motY = motionUV().atNXYZC( 1, xI+dxI, yI+dyI, zI+dzI, 0 );
      mot_comp_diff = img().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 )
                    - img().atNXYZC( 0, xI+dxI+motX, yI+dyI+motY, zI+dzI, 1 );

      occlusion_weight =  _gauss( divergence, 0, _sigma_occ_divergence );
      occlusion_weight *= _gauss( mot_comp_diff, 0, _sigma_occ_color );

      weight = spatial_weight * color_weight * occlusion_weight;
      weight_sum += weight;

      pixelGradientU += weight * _penaltyFunction->getPenaltyGradient( fabs(double(du)) ) * signum(du);
      pixelGradientV += weight * _penaltyFunction->getPenaltyGradient( fabs(double(dv)) ) * signum(dv);
    } else {
      pixelGradientU += _penaltyFunction->getPenaltyGradient( fabs(double(du)) ) * signum(double(du));
      pixelGradientV += _penaltyFunction->getPenaltyGradient( fabs(double(dv)) ) * signum(double(dv));
      weight_sum += 1.0;
    }
  }
  if (weight_sum) {
    pixelGradientU /= weight_sum;
    pixelGradientV /= weight_sum;
  } else {
      pixelGradientU = 0.0;
      pixelGradientV = 0.0;
  }

  ret[0] = T(_lamb * pixelGradientU);
  ret[1] = T(_lamb * pixelGradientV);

  return ret;
}

template <class T>
std::vector<T> EnergyNonLocal<T>::getEnergyHessian(
    int, int xI, int yI, int zI, int )
{
  std::vector<T> ret( 4, T(0) );

  //  energy gradients wrt the motion at "current" pixel x_k
  //  for example dE(u(x_k)) / du(x_k)
  T energyHessianUU, energyHessianUV, energyHessianVV;

  T weight_sum;
  T weight, spatial_weight, color_weight, occlusion_weight;
  T color_diff;

  T du, dv, dxU, dyV, divergence, mot_comp_diff, motX, motY;
  int dxI, dyI, dzI;

  energyHessianUU = T(0.0);
  energyHessianUV = T(0.0);
  energyHessianVV = T(0.0);

  weight_sum    = 0.0;
  for (dxI = -_radius; dxI < _radius+1; ++dxI)
  for (dyI = -_radius; dyI < _radius+1; ++dyI)
  for (dzI = -0; dzI < 0+1; ++dzI)
  {
    du = motionUV().atNXYZC( 0, xI,     yI,     zI,     0 )
       - motionUV().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 );
    dv = motionUV().atNXYZC( 1, xI,     yI,     zI,     0 )
       - motionUV().atNXYZC( 1, xI+dxI, yI+dyI, zI+dzI, 0 );

    if (_useWeight) {
      dxU = (motionUV().atNXYZC( 0, xI+dxI+1, yI+dyI,   zI+dzI, 0 )
            -motionUV().atNXYZC( 0, xI+dxI-1, yI+dyI,   zI+dzI, 0 ))/2;
      dyV = (motionUV().atNXYZC( 1, xI+dxI,   yI+dyI+1, zI+dzI, 0 )
            -motionUV().atNXYZC( 1, xI+dxI,   yI+dyI-1, zI+dzI, 0 ))/2;
      divergence = dxU + dyV;
      if (divergence > 0) divergence = 0;

      spatial_weight = _gauss( sqrt(double(dxI*dxI + dyI*dyI)), 0, _sigma_spatial );
      color_diff = img().atNXYZC( 0, xI,     yI,     zI,     0 )
                 - img().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 );
      color_weight = _gauss( color_diff, 0, _sigma_color );

      motX = motionUV().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 );
      motY = motionUV().atNXYZC( 1, xI+dxI, yI+dyI, zI+dzI, 0 );
      mot_comp_diff = img().atNXYZC( 0, xI+dxI, yI+dyI, zI+dzI, 0 )
                    - img().atNXYZC( 0, xI+dxI+motX, yI+dyI+motY, zI+dzI, 1 );

      occlusion_weight =  _gauss( divergence, 0, _sigma_occ_divergence );
      occlusion_weight *= _gauss( mot_comp_diff, 0, _sigma_occ_color );

      weight = spatial_weight * color_weight * occlusion_weight;
      weight_sum += weight;

      energyHessianUU += weight * _penaltyFunction->getPenaltyHessian(du);
      energyHessianVV += weight * _penaltyFunction->getPenaltyHessian(dv);
    } else {
      energyHessianUU += _penaltyFunction->getPenaltyHessian(du);
      energyHessianVV += _penaltyFunction->getPenaltyHessian(dv);
      weight_sum += 1.0;
    }
  }
  if (weight_sum) {
    energyHessianUU /= weight_sum;
    energyHessianVV /= weight_sum;
  } else {
    energyHessianUU = T(0.0);
    energyHessianVV = T(0.0);
  }

  ret[0] = T(_lamb * energyHessianUU);
  ret[1] = T(_lamb * energyHessianUV);
  ret[2] = T(_lamb * energyHessianUV);
  ret[3] = T(_lamb * energyHessianVV);

  return ret;
}

template <class T>
int EnergyNonLocal<T>::getEnergyGradientDimensions() { return 2; }

template <typename T>
inline T EnergyNonLocal<T>::_gauss( T x, T mu, T sigma )
{
  return 1/(sqrt(2*M_PI*sigma*sigma))
    * exp(double(-(x - mu)*(x - mu)/(2*sigma*sigma))) ;
}

template <typename T>
inline T EnergyNonLocal<T>::_dgauss( T x, T mu, T sigma )
{
  return (mu - x)/(sigma*sigma*sqrt(2*M_PI*sigma*sigma))
    * exp(-(x - mu)*(x - mu)/(2*sigma*sigma)) ;
}


#endif /* _ENERGYNONLOCAL_HXX_ */

