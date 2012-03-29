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
	const int& r = windowRadius();
	o = img;

	int nbhCnt = 4*r*r + 4*r + 1 ;

	//  neighborhood and weights vectors
	std::vector<SNeighborhood> neighborhood( nbhCnt );
	typename std::vector<SNeighborhood>::const_iterator itNbh;

	T weight_sum;
	int offset;
	int medianIdx;

	int idx;
	cimglist_for(o,nn)
	cimg_forXYZC(o[nn], xx, yy, zz, cc)
	{
		idx = 0;
		for (int x=-r; x<r+1; ++x)
		for (int y=-r; y<r+1; ++y)
		{
			neighborhood[idx].value  = T(img.atNXYZC( nn, xx+x, yy+y, zz, cc ));
			neighborhood[idx].weight = T(inweight.atNXYZC( nn, xx+x, yy+y, zz, cc ));
			++idx;
// (!!) TODO (!!)
//			weightFunction->getCliqueWeight( nn, xx, yy, zz, cc, 0, x, y, 0, 0 );  //  (!!)
// (!!) VERY IMPORTANT (!!)
//			This clique weight depends on current window center (nn, xx, yy, zz, cc) (!!)
//			AND window position (n, x, y, z, c) (!!)
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

		for (  itNbh =  neighborhood.begin();
		       itNbh != neighborhood.end();
		     ++itNbh)
		{
			if (weight_sum > 0) {
				++offset;
			} else {
				--offset;
			}
			weight_sum -= (2 * itNbh->weight) ;
		}

		medianIdx = 2*r*r + 2*r + (offset/2);
		o( nn, xx, yy, zz, cc ) = neighborhood[medianIdx].value;
	}
}

#endif /* _WEIGHTEDMEDIAN_HXX_ */

