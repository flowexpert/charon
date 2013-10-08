/*  Copyright (C) 2013
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
/** \file ConfidenceMeasures.hxx
 *  Implementation of the parameter class ConfidenceMeasures.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 21.03.2013
 */

#ifndef _CONFIDENCEMEASURES_HXX_
#define _CONFIDENCEMEASURES_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "ConfidenceMeasures.h"

template <typename T>
ConfidenceMeasures<T>::ConfidenceMeasures(const std::string& name) :
		TemplatedParameteredObject<T>(
			"ConfidenceMeasures", name,
			"This module calculates the confidence measures Psi_BOOTG, Psi_BOOTGM and "
			"Psi_BOOTA, as proposed by <i>Jan Kybic</i> and <i>Claudia Nieuwenhuis</i> "
			"within their 2011 paper <b>Bootstrap Optical Flow Confidence "
			"and Uncertainty Measure</b>.<br>"
			"Here, an arbitrary count of (multi-dimensional) optical flow fields can be "
			"connected via the motions multi-input slot. "
			"Additionally, this module yields the mean and variance motion fields. "
		),
		motions(false,true)
{
	ParameteredObject::_addInputSlot(
		motions, "motions",
		"multi-slot for motion-fields",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		mean, "mean",
		"motion mean",
		"CImgList<T>");
	ParameteredObject::_addOutputSlot(
		variance, "variance",
		"motion variance",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		bootG, "bootG",
		"bootG confidence measure",
		"CImgList<T>");
	ParameteredObject::_addOutputSlot(
		bootGM, "bootGM",
		"bootGM confidence measure",
		"CImgList<T>");
	ParameteredObject::_addOutputSlot(
		bootA, "bootA",
		"bootA confidence measure",
		"CImgList<T>");

}

template <typename T>
void ConfidenceMeasures<T>::execute()
{
	// count of connected motion fields
	unsigned int motionsCnt = motions.size();

	// obtain motion field dimensions from first motion field
	unsigned int _size = motions[0].size();
	unsigned int _width = motions[0][0].width();
	unsigned int _height = motions[0][0].height();
	unsigned int _depth = motions[0][0].depth();
	unsigned int _spectrum = motions[0][0].spectrum();

	cimg_library::CImgList<T> motionMean( _size, _width, _height, _depth, _spectrum, T(0.0) );
	cimg_library::CImgList<T> motionVariance( _size, _width, _height, _depth, _spectrum, T(0.0) );

	// compute motion field mean
	for (unsigned int i=0; i<motionsCnt; i++) {
		cimglist_for( motionMean, nn )
		cimg_forXYZC( motionMean[nn], xx, yy, zz, cc )
		{
			motionMean[nn].atXYZC( xx, yy, zz, cc )
			  += motions[i][nn].atXYZC( xx, yy, zz, cc );
		}
	}
	cimglist_for( motionMean, nn )
	cimg_forXYZC( motionMean[nn], xx, yy, zz, cc )
	{
		motionMean[nn].atXYZC( xx, yy, zz, cc ) /= motionsCnt;
	}

	// compute motion field variance
	for (unsigned int i=0; i<motionsCnt; i++) {
		cimglist_for( motionVariance, nn )
		cimg_forXYZC( motionVariance[nn], xx, yy, zz, cc )
		{
			motionVariance[nn].atXYZC( xx, yy, zz, cc )
			  += pow( double(motions[i][nn].atXYZC( xx, yy, zz, cc )
			                -motionMean[nn].atXYZC( xx, yy, zz, cc )), double(2.0) );
		}
	}
	cimglist_for( motionVariance, nn )
	cimg_forXYZC( motionVariance[nn], xx, yy, zz, cc )
	{
		motionVariance[nn].atXYZC( xx, yy, zz, cc ) /= motionsCnt;
	}

	// compute \Psi_{BootG} confidence measure
	T varsum = T(0.0);
	cimg_library::CImgList<T> motionBootG( 1, _width, _height, _depth, _spectrum, T(0.0) );
	cimg_forXYZC( motionBootG[0], xx, yy, zz, cc )
	{
		varsum = T(0.0);
		for (unsigned int nn=0; nn<_size; nn++) {  // for each motion component
			varsum += motionVariance[nn].atXYZC( xx, yy, zz, cc );
		}
		motionBootG[0].atXYZC( xx, yy, zz, cc ) = pow( double(varsum), double(0.5) );
	}

	// compute \Psi_{BootGM} confidence measure
	T tmp = T(0.0);
	T tmp2 = T(0.0);
	cimg_library::CImgList<T> motionBootGM( 1, _width, _height, _depth, _spectrum, T(0.0) );
	cimg_forXYZC( motionBootGM[0], xx, yy, zz, cc )
	{
		tmp2 = T(0.0);
		for (unsigned int i=0; i<motionsCnt; i++) {  // for each motion field
			tmp = T(0.0);
			for (unsigned int nn=0; nn<_size; nn++) {  // for each motion component
				tmp += pow( double(motions[i][nn].atXYZC( xx, yy, zz, cc )
				                  -motionMean[nn].atXYZC( xx, yy, zz, cc )), double(2.0) );
			}
			tmp2 += pow( double(tmp), double(0.5) );
		}
		motionBootGM[0].atXYZC( xx, yy, zz, cc ) = tmp2 / motionsCnt;
	}

	// compute \Psi_{BootA} confidence measure
	// not implemented yet

	// assign output slots
	mean() = motionMean;
	variance() = motionVariance;
	bootG() = motionBootG;
	bootGM() = motionBootGM;
}

#endif /* _CONFIDENCEMEASURES_HXX_ */

