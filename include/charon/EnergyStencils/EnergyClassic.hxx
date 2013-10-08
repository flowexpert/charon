/*  Copyright (C) 2011, 2012, 2013
                  University of Heidelberg (IWR/HCI)

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
 *  \date 20.12.2011
 */

#ifndef _ENERGYCLASSIC_HXX_
#define _ENERGYCLASSIC_HXX_

#include <charon/Stencil.hxx>

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
	Stencil::Base<T>("EnergyClassic", name,
			"This stencil implements so-called classic regularization. "
			"It can be used directly in connection with a linear solver, "
			"yielding the Euler-Lagrange equations or for energy optimization. "
			"Figurative, the motion at the central position "
			"is being adjusted to the motions within its neighborhood "
			"accordingly to the utilized penalty function.<br>"
			"Currently, a hard-coded 4- or 8-neighborhood can be selected using "
			"the diagonalRegularization switch. "
			"If the optional right-hand-side motionUV is <i>not</i> connected, "
			"the stencil implies a uniform averaging over the whole neighborhood.<br>"
			"An outstanding feature is the support for regularization masks. "
			"If such a mask (indicating borders via 0 and regions via non-0) is connected, "
			"regularization is being performed bilaterally utilizing a flood-fill approach. "
			),
	motionUV(true,false),
	regionMask(true,false),
	matchMask(true,false)
{
	this->_addInputSlot(penaltyFunction,
	                    "penaltyFunction",
	                    "penalty function",
	                    "PenaltyFunction<T>*");

	this->_addInputSlot(motionUV,
	                    "motionUV",
	                    "current motion components",
	                    "CImgList<T>");

	this->_addInputSlot(roi, "roi", "region of interest", "Roi<int>*");

	this->_addInputSlot(regionMask, "regionMask", "mask indicating region separation", "CImgList<T>");
	this->_addInputSlot(matchMask, "matchMask", "mask indicating true (e.g. matched) positions", "CImgList<T>");

	ParameteredObject::_addParameter(
			diagonalRegularization, "diagonalRegularization", "if set, perform additional diagonal regularization");
	ParameteredObject::_addParameter(
			pUnknowns, "unknowns", "List of unknowns");

	ParameteredObject::_setTags("charon-flow;Stencils;CImg");
}

template <class T>
void EnergyClassic<T>::execute() {
	Stencil::Base<T>::execute();
	if (!this->lambdaMask.connected()) {
		_lamb = this->lambda();
	} else {
		_lambdaMask = this->lambdaMask();
		_lamb = T(1.0);
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
		this->_unknowns.insert(*puIt);
	}
}

template <class T>
T EnergyClassic<T>::_energy( int pn, int px, int py, int pz, int pc, T x, T xo )
{
	return _penaltyFunction->getPenalty( pn, px, py, pz, pc, pow(double(x - xo), 2.0) );
}

template <class T>
T EnergyClassic<T>::_energyGradient( int pn, int px, int py, int pz, int pc, T x, T xo )
{
	return _penaltyFunction->getPenaltyGradient( pn, px, py, pz, pc, pow(double(x - xo), 2.0) ) * (x - xo);
}

template <class T>
T EnergyClassic<T>::_energyHessian( int pn, int px, int py, int pz, int pc, T x, T xo )
{
	return _penaltyFunction->getPenaltyGradient( pn, px, py, pz, pc, pow(double(x - xo), 2.0) )
	     + pow(double(x - xo), 2.0) * _penaltyFunction->getPenaltyHessian( pn, px, py, pz, pc, pow(double(x - xo), 2.0) );
}

template <class T>
T EnergyClassic<T>::getEnergy( int, int x, int y, int z, int )
{
	if (this->lambdaMask.connected())
		_lamb = _lambdaMask.atNXYZC( 0, x, y, z, 0 );

	T motionUC = motionUV().atNXYZC( 0, x,   y,   z, 0 );
	T motionUN = motionUV().atNXYZC( 0, x,   y-1, z, 0 );
	T motionUE = motionUV().atNXYZC( 0, x+1, y,   z, 0 );
	T motionUS = motionUV().atNXYZC( 0, x,   y+1, z, 0 );
	T motionUW = motionUV().atNXYZC( 0, x-1, y,   z, 0 );
	T motionVC = motionUV().atNXYZC( 1, x,   y,   z, 0 );
	T motionVN = motionUV().atNXYZC( 1, x,   y-1, z, 0 );
	T motionVE = motionUV().atNXYZC( 1, x+1, y,   z, 0 );
	T motionVS = motionUV().atNXYZC( 1, x,   y+1, z, 0 );
	T motionVW = motionUV().atNXYZC( 1, x-1, y,   z, 0 );

	T energy =  _energy( -1, x, y, z, -1, motionUC, motionUN )
	         +  _energy( -1, x, y, z, -1, motionUC, motionUE )
	         +  _energy( -1, x, y, z, -1, motionUC, motionUS )
	         +  _energy( -1, x, y, z, -1, motionUC, motionUW );
        energy   += _energy( -1, x, y, z, -1, motionVC, motionVN )
                 +  _energy( -1, x, y, z, -1, motionVC, motionVE )
                 +  _energy( -1, x, y, z, -1, motionVC, motionVS )
                 +  _energy( -1, x, y, z, -1, motionVC, motionVW );

	return T(this->_lamb * energy);
}

template <class T>
std::vector<T> EnergyClassic<T>::getEnergyGradient(
  int, int x, int y, int z, int )
{
	if (this->lambdaMask.connected())
		_lamb = _lambdaMask.atNXYZC( 0, x, y, z, 0 );

        T motionUC = motionUV().atNXYZC( 0, x,   y,   z, 0 );
        T motionUN = motionUV().atNXYZC( 0, x,   y-1, z, 0 );
        T motionUE = motionUV().atNXYZC( 0, x+1, y,   z, 0 );
        T motionUS = motionUV().atNXYZC( 0, x,   y+1, z, 0 );
        T motionUW = motionUV().atNXYZC( 0, x-1, y,   z, 0 );
        T motionVC = motionUV().atNXYZC( 1, x,   y,   z, 0 );
        T motionVN = motionUV().atNXYZC( 1, x,   y-1, z, 0 );
        T motionVE = motionUV().atNXYZC( 1, x+1, y,   z, 0 );
        T motionVS = motionUV().atNXYZC( 1, x,   y+1, z, 0 );
        T motionVW = motionUV().atNXYZC( 1, x-1, y,   z, 0 );

        T energyGradientU = _energyGradient( -1, x, y, z, -1, motionUC, motionUN )
                          + _energyGradient( -1, x, y, z, -1, motionUC, motionUE )
                          + _energyGradient( -1, x, y, z, -1, motionUC, motionUS )
                          + _energyGradient( -1, x, y, z, -1, motionUC, motionUW );
        T energyGradientV = _energyGradient( -1, x, y, z, -1, motionVC, motionVN )
                          + _energyGradient( -1, x, y, z, -1, motionVC, motionVE )
                          + _energyGradient( -1, x, y, z, -1, motionVC, motionVS )
                          + _energyGradient( -1, x, y, z, -1, motionVC, motionVW );

	std::vector<T> ret( 2, T(0.0) );
	ret[0] = T(this->_lamb * energyGradientU);
	ret[1] = T(this->_lamb * energyGradientV);
	return ret;
}

template <class T>
std::vector<T> EnergyClassic<T>::getEnergyHessian(
  int, int x, int y, int z, int )
{
	if (this->lambdaMask.connected())
		_lamb = _lambdaMask.atNXYZC( 0, x, y, z, 0 );

	T motionUC = motionUV().atNXYZC( 0, x,   y,   z, 0 );
	T motionUN = motionUV().atNXYZC( 0, x,   y-1, z, 0 );
	T motionUE = motionUV().atNXYZC( 0, x+1, y,   z, 0 );
	T motionUS = motionUV().atNXYZC( 0, x,   y+1, z, 0 );
	T motionUW = motionUV().atNXYZC( 0, x-1, y,   z, 0 );
	T motionVC = motionUV().atNXYZC( 1, x,   y,   z, 0 );
	T motionVN = motionUV().atNXYZC( 1, x,   y-1, z, 0 );
	T motionVE = motionUV().atNXYZC( 1, x+1, y,   z, 0 );
	T motionVS = motionUV().atNXYZC( 1, x,   y+1, z, 0 );
	T motionVW = motionUV().atNXYZC( 1, x-1, y,   z, 0 );

	T energyHessianUU = _energyHessian( -1, x, y, z, -1, motionUC, motionUN )
	                  + _energyHessian( -1, x, y, z, -1, motionUC, motionUE )
	                  + _energyHessian( -1, x, y, z, -1, motionUC, motionUS )
	                  + _energyHessian( -1, x, y, z, -1, motionUC, motionUW );
	T energyHessianUV = T(0.0);
	T energyHessianVV = _energyHessian( -1, x, y, z, -1, motionVC, motionVN )
	                  + _energyHessian( -1, x, y, z, -1, motionVC, motionVE )
	                  + _energyHessian( -1, x, y, z, -1, motionVC, motionVS )
	                  + _energyHessian( -1, x, y, z, -1, motionVC, motionVW );

	std::vector<T> ret( 4, T(0.0) );
	ret[0] = T(this->_lamb * energyHessianUU);
	ret[1] = T(this->_lamb * energyHessianUV);
	ret[2] = T(this->_lamb * energyHessianUV);
	ret[3] = T(this->_lamb * energyHessianVV);

	return ret;
}

template <class T>
int EnergyClassic<T>::getEnergyGradientDimensions() { return 2; }

template <class T>
void EnergyClassic<T>::updateStencil(
		const std::string& unknown,
		const Point4D<int>& p, const int&)
{
	if (this->lambdaMask.connected())
		_lamb = _lambdaMask.atNXYZC( 0, p.x, p.y, p.z, 0 );

	// motion component in neighborhood
	T motionN, motionE, motionS, motionW;
	T motionNE, motionSE, motionSW, motionNW;
	T motionC = T(0);

	// penalty in neighborhood
	T pCN, pCE, pCS, pCW;
	T pCNE, pCSE, pCSW, pCNW;
	T pSum = T(0.0);
	T pSumMask = T(0.0);
	T motionSum = T(0.0);
	T motionCenterSum = T(0.0);

	int motionConnected = motionUV.connected();
	int regionMaskConnected = regionMask.connected();
	int matchMaskConnected = matchMask.connected();

	// fill region mask with given regularization mask
	bool regionMaskC = true, regionMaskN = true, regionMaskE = true, regionMaskS = true, regionMaskW = true;
	bool regionMaskNE = true, regionMaskSE = true, regionMaskSW = true, regionMaskNW = true;
	if (regionMaskConnected) {
		regionMaskC  &= (bool)regionMask()[0].atXYZC(p.x,   p.y,   0, 0);
		regionMaskN  &= (bool)regionMask()[0].atXYZC(p.x,   p.y-1, 0, 0);
		regionMaskNE &= (bool)regionMask()[0].atXYZC(p.x+1, p.y-1, 0, 0);
		regionMaskE  &= (bool)regionMask()[0].atXYZC(p.x+1, p.y,   0, 0);
		regionMaskSE &= (bool)regionMask()[0].atXYZC(p.x+1, p.y+1, 0, 0);
		regionMaskS  &= (bool)regionMask()[0].atXYZC(p.x,   p.y+1, 0, 0);
		regionMaskSW &= (bool)regionMask()[0].atXYZC(p.x-1, p.y+1, 0, 0);
		regionMaskW  &= (bool)regionMask()[0].atXYZC(p.x-1, p.y,   0, 0);
		regionMaskNW &= (bool)regionMask()[0].atXYZC(p.x-1, p.y-1, 0, 0);
	}

	// get match mask
	bool matchMaskC = false;
	if (matchMaskConnected) {
		matchMaskC = (bool)matchMask()[0].atXY( p.x, p.y );
	}

	// fill stencil with masks
	for(unsigned int i=0; i< this->pUnknowns.size() ; i++) {
		SubStencil<T> entry;
		if (motionConnected) {
			motionC  = motionUV().atNXYZC( i, p.x,   p.y,   p.z, 0 );
			motionN  = motionUV().atNXYZC( i, p.x,   p.y-1, p.z, 0 );
			motionNE = motionUV().atNXYZC( i, p.x+1, p.y-1, p.z, 0 );
			motionE  = motionUV().atNXYZC( i, p.x+1, p.y,   p.z, 0 );
			motionSE = motionUV().atNXYZC( i, p.x+1, p.y+1, p.z, 0 );
			motionS  = motionUV().atNXYZC( i, p.x,   p.y+1, p.z, 0 );
			motionSW = motionUV().atNXYZC( i, p.x-1, p.y+1, p.z, 0 );
			motionW  = motionUV().atNXYZC( i, p.x-1, p.y,   p.z, 0 );
			motionNW = motionUV().atNXYZC( i, p.x-1, p.y-1, p.z, 0 );
			pCN  = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1,
			                                                   pow(double(motionC - motionN), 2.0) );
			pCNE = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1,
			                                                   pow(double(motionC - motionNE), 2.0) );
			pCE  = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1,
			                                                   pow(double(motionC - motionE), 2.0) );
			pCSE = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1,
			                                                   pow(double(motionC - motionSE), 2.0) );
			pCS  = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1,
			                                                   pow(double(motionC - motionS), 2.0) );
			pCSW = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1,
			                                                   pow(double(motionC - motionSW), 2.0) );
			pCW  = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1,
			                                                   pow(double(motionC - motionW), 2.0) );
			pCNW = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1,
			                                                   pow(double(motionC - motionNW), 2.0) );
		} else {
			pCN  = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, 0.0 );
			pCNE = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, 0.0 );
			pCE  = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, 0.0 );
			pCSE = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, 0.0 );
			pCS  = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, 0.0 );
			pCSW = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, 0.0 );
			pCW  = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, 0.0 );
			pCNW = _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, 0.0 );
		}
		// weighting of diagonal cliques
		if (diagonalRegularization()) {
			T w = pow(double(2.0), -0.5);
			pCNE *= w;
			pCSE *= w;
			pCSW *= w;
			pCNW *= w;
		} else {
			pCNE = T(0.0);
			pCSE = T(0.0);
			pCSW = T(0.0);
			pCNW = T(0.0);
		}

		_dataMask.assign(3,3,1,1);
		_patternMask.assign(3,3,1,1);
		_center = Point4D<int>(1,1,0,0);
		_dataMask.fill(    0, 0, 0,     0, 0, 0,     0, 0, 0 );
		if (pUnknowns[i] == unknown) {
			_patternMask.fill( 1, 1, 1,     1, 1, 1,     1, 1, 1 );
		} else {
			_patternMask.fill( 0, 0, 0,     0, 0, 0,     0, 0, 0 );
		}
		this->_rhs = T(0.0);

		if (pUnknowns[i] == unknown)
		{
			// if true pixel value, e.g. from sparse match, then do not regularize
			if (motionConnected && matchMaskConnected && matchMaskC) {
				_dataMask.fill( 0, 0, 0,
				                0,
				                _lamb*_penaltyFunction->getPenaltyGradient( -1, p.x, p.y, p.z, -1, 0.0 ),
						0,
				                0, 0, 0 );
				this->_rhs = _lamb * motionC;
			} else {
				pSum =  T(0.0);
				pSum += (regionMaskN  ? pCN  : T(0.0));
				pSum += (regionMaskNE ? pCNE : T(0.0));
				pSum += (regionMaskE  ? pCE  : T(0.0));
				pSum += (regionMaskSE ? pCSE : T(0.0));
				pSum += (regionMaskS  ? pCS  : T(0.0));
				pSum += (regionMaskSW ? pCSW : T(0.0));
				pSum += (regionMaskW  ? pCW  : T(0.0));
				pSum += (regionMaskNW ? pCNW : T(0.0));
				pSumMask = pSum;

				if (pSumMask) {
					_dataMask.fill( (regionMaskNW ? -pCNW : T(0.0)), (regionMaskN ? -pCN : T(0.0)), (regionMaskNE ? -pCNE : T(0.0)),
					                (regionMaskW  ? -pCW  : T(0.0)), pSum,                          (regionMaskE  ? -pCE  : T(0.0)),
					                (regionMaskSW ? -pCSW : T(0.0)), (regionMaskS ? -pCS : T(0.0)), (regionMaskSE ? -pCSE : T(0.0))  );
				} else {  //  if all neighbors are out masked then inpainting
					pSum =  pCN + pCNE + pCE + pCSE + pCS + pCSW + pCW + pCNW;

					_dataMask.fill( -pCNW, -pCN,  -pCNE,
					                -pCW,   pSum, -pCE,
					                -pCSW, -pCS,  -pCSE );
				}

				if (motionConnected) {
					if (pSumMask) {
						motionSum =  T(0.0);
						motionSum += (regionMaskN  ? motionN  * pCN  : T(0.0));
						motionSum += (regionMaskNE ? motionNE * pCNE : T(0.0));
						motionSum += (regionMaskE  ? motionE  * pCE  : T(0.0));
						motionSum += (regionMaskSE ? motionSE * pCSE : T(0.0));
						motionSum += (regionMaskS  ? motionS  * pCS  : T(0.0));
						motionSum += (regionMaskSW ? motionSW * pCSW : T(0.0));
						motionSum += (regionMaskW  ? motionW  * pCW  : T(0.0));
						motionSum += (regionMaskNW ? motionNW * pCNW : T(0.0));
						motionCenterSum =  T(0.0);
						motionCenterSum += (regionMaskN  ? motionC * pCN  : T(0.0));
						motionCenterSum += (regionMaskNE ? motionC * pCNE : T(0.0));
						motionCenterSum += (regionMaskE  ? motionC * pCE  : T(0.0));
						motionCenterSum += (regionMaskSE ? motionC * pCSE : T(0.0));
						motionCenterSum += (regionMaskS  ? motionC * pCS  : T(0.0));
						motionCenterSum += (regionMaskSW ? motionC * pCSW : T(0.0));
						motionCenterSum += (regionMaskW  ? motionC * pCW  : T(0.0));
						motionCenterSum += (regionMaskNW ? motionC * pCNW : T(0.0));
					} else {
	                                        motionSum =  T(0.0);
	                                        motionSum += motionN  * pCN;
						motionSum += motionNE * pCNE;
	                                        motionSum += motionE  * pCE;
						motionSum += motionSE * pCSE;
	                                        motionSum += motionS  * pCS;
						motionSum += motionSW * pCSW;
	                                        motionSum += motionW  * pCW;
						motionSum += motionNW * pCNW;
	                                        motionCenterSum =  T(0.0);
	                                        motionCenterSum += motionC * pCN;
						motionCenterSum += motionC * pCNE;
	                                        motionCenterSum += motionC * pCE;
						motionCenterSum += motionC * pCSE;
	                                        motionCenterSum += motionC * pCS;
						motionCenterSum += motionC * pCSW;
                                	        motionCenterSum += motionC * pCW;
						motionCenterSum += motionC * pCNW;
					}
					this->_rhs = motionSum - motionCenterSum;
				}
			}
		}

		entry.data = _dataMask;
		entry.pattern = _patternMask;
		entry.center = _center;
		this->_subStencils[pUnknowns[i]] = entry;
	}
}

template <class T>
cimg_library::CImg<T> EnergyClassic<T>::apply(
		const cimg_library::CImgList<T>& seq,
		const unsigned int frame) const
{
	return seq[frame].get_convolve(_dataMask);
}

#endif /* _ENERGYCLASSIC_HXX_ */

