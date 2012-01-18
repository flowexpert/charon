/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file WeightedMedian.hxx
 *  Implementation of the parameter class WeightedMedian.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 17.01.2012
 */

#ifndef _WEIGHTEDMEDIAN_HXX_
#define _WEIGHTEDMEDIAN_HXX_

#include "WeightedMedian.h"

#define EPS_INVERSE 1e9

template<typename T>
WeightedMedian<T>::WeightedMedian(const std::string& name) :
	TemplatedParameteredObject<T>(
			"WeightedMedian", name,
			"calculates weighted median of image windows<br><br>"
			"Use image windows of given size, calculate weighted median within this "
			"window and use the result as new value for the window center "
			"pixel. This eliminates outliers and makes "
			"e.g. flow estimation more robust. "
			"This uses the approach described by Li and Osher "
			"(A new median formula with applications to PDE based denoising) "
			"equation 3.13, whereas lambda -> 0."  )
{
	this->_addInputSlot(in,    "in",    "image input",  "CImgList<T>");
	this->_addInputSlot(inWeight, "inWeight", "weight input", "CImgList<T>");
	this->_addOutputSlot(out,  "out",   "image output", "CImgList<T>");
	this->_addParameter(
			windowRadius, "windowRadius",
			"radius r of image windows (size is 2*r+1)",  7u);
}

struct SNeighborhood {
	double value;
	double weight;
};
struct by_value {
	bool operator()( struct SNeighborhood const &a,
	                 struct SNeighborhood const &b )
	{
		return a.value < b.value ;
	}
};

template<typename T>
void WeightedMedian<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const cimg_library::CImgList<T>& img = in();
	const cimg_library::CImgList<T>& inweight = inWeight();

	cimg_library::CImgList<T>& o = out();
	const unsigned int& r = windowRadius();
	o = img;

	unsigned int imgSize = img.size();
	unsigned int imgWidth = img[0].width();
	unsigned int imgHeight = img[0].height();
	unsigned int imgDepth = img[0].depth();
	unsigned int imgSpectrum = img[0].spectrum();

	unsigned int noOfElements = 4*r*r + 4*r + 1 ;

	//  neighborhood and weights vectors
	std::vector<SNeighborhood> neighborhood( noOfElements );
	typename std::vector<SNeighborhood>::const_iterator itNbh;

	T weight_sum;
	int offset;
	unsigned int medianIdx;

	unsigned int idx;
	for (unsigned int z=0; z<imgDepth; ++z)
	for (unsigned int t=0; t<imgSpectrum; ++t)
	for (unsigned int v=0; v<imgSize; ++v)
	for (unsigned int i=0; i<imgWidth; ++i)
	for (unsigned int j=0; j<imgHeight; ++j)
	{

		idx = 0;
		for (unsigned int di=0; di<(2*r+1); ++di)
		for (unsigned int dj=0; dj<(2*r+1); ++dj)
		{
			neighborhood[idx].value  = T(img.atNXYZC( v, i+di-r, j+dj-r, z, t ));
			neighborhood[idx].weight = T(inweight.atNXYZC( v, i+di-r, j+dj-r, z, t ));
			++idx;
		}

		std::sort( neighborhood.begin(), neighborhood.end(), by_value() );

		weight_sum = T(0);
		for (  itNbh =  neighborhood.begin();
		       itNbh != neighborhood.end();
		     ++itNbh)
		{
			weight_sum += itNbh->weight ;
		}

		offset = 0;
		if (weight_sum > 0) {
			++offset;
		} else {
			--offset;
		}

		for (  itNbh =  neighborhood.begin();
		       itNbh != neighborhood.end();
		     ++itNbh)
		{
			weight_sum -= (2 * itNbh->weight) ;
			if (weight_sum > 0) {
				++offset;
			} else {
				--offset;
			}
		}

		medianIdx = 2*r*r + 2*r + (offset/2);
		o( v, i, j, z, t ) = neighborhood[medianIdx].value;
	}
}

#endif /* _WEIGHTEDMEDIAN_HXX_ */

