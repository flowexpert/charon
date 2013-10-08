/*  Copyright (C) 2012, 2013
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
/** \file WeightedMedian.hxx
 *  Implementation of the parameter class WeightedMedian.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 17.01.2012
 */

#ifndef _WEIGHTEDMEDIAN_HXX_
#define _WEIGHTEDMEDIAN_HXX_

#include "WeightedMedian.h"

#include <charon/CliqueWeight.hxx>

#include <vector>

template<typename T>
WeightedMedian<T>::WeightedMedian(const std::string& name) :
	TemplatedParameteredObject<T>(
			"WeightedMedian", name,
			"This module performs weighted median filtering of images. "
			"It calculates the weighted median for each image window "
			"and writes its result into the window's center position. "
			"Median filtering in general is an approach to L1-optimization "
			"(see e.g. <b>Numerical Methods in Scientific Computing</b>, "
			"vol. 2, by &Aring;ke Bj&ouml;rck, section 8.7.5. "
			"<b>Solving Overdetermined Systems in the lp Sense</b>).<br>"
			"Here, the filtering windows lie within the X and Y dimensions, that is "
			"the filter is being applied <i>separate</i> for each "
			"N, Z, and C dimension to the input data in. "
			"Optionally, a mask (with 0 borders and non-0 regions) can be connected "
			"in order to perform <i>bilateral</i> median filtering. "
			"The parameter list windowRadiusList specifies the window radius to be used "
			"at each level, if the level (which roughly corresponds to a pyramid level) "
			"is connected, otherwise, the first element of this list is being used as "
			"the window radius. "
			"In order to specify the element weights, a clique weight module "
			"(e.g. CliqueWeightUniform to perform uniform median filtering) "
			"has to be connected via its input slot. "
			"The filtering process itself closely resembles the approach proposed "
			"by <i>Li</i> and <i>Osher</i> within their paper <b>A new median formula with applications "
			"to pde-based denoising</b>. "
			"In order to iterate bilaterally over the image window, a flood-fill-like "
			"implementation is being utilized. "
			),
	mask(true,false),
	level(true,false),
	windowRadiusList("7")
{
	this->_addInputSlot(in, "in", "data input", "CImgList<T>");
	this->_addInputSlot(mask, "mask", "mask for bilateral median filtering (optional)", "CImgList<T>");
	this->_addInputSlot(cliqueWeight, "cliqueWeight", "clique weight function input", "CliqueWeight<T>*");
	this->_addInputSlot(level, "level", "current pyramide level (optional)", "uint");
	this->_addOutputSlot(out, "out", "data output", "CImgList<T>");
	this->_addParameter(
			windowRadiusList, "windowRadiusList",
			"radius list of image windows, level selects, otherwise first element is chosen", "T_list");

	ParameteredObject::_setTags("charon-flow;MedianFilters;CImg");
}

struct SNeighbor {
	double value;
	double weight;
};
struct by_value {
	bool operator()( struct SNeighbor const &a,
	                 struct SNeighbor const &b )
	{
		return a.value < b.value ;
	}
};

template<typename T>
void WeightedMedian<T>::execute() {
	const cimg_library::CImgList<T>& img = in();
	CliqueWeight<T>* _cliqueWeight = cliqueWeight();

	cimg_library::CImgList<T>& o = out();
	o = img;

	int r;
	if (level.connected())
		r = windowRadiusList()[level()];
	else
		r = windowRadiusList()[0];

	int nbhCnt = 4*r*r + 4*r + 1 ;

	//  neighborhood and weights vectors
	std::vector<SNeighbor> neighborhood( nbhCnt );
	typename std::vector<SNeighbor>::const_iterator itNbh;

	T weight_sum, partial_weight_sum;

	int idx;
	cimglist_for(o,nn)
	cimg_forXYZC(o[nn], xx, yy, zz, cc)
	{
		idx = 0;
		if (mask.connected()) { // recursive bilateral median filtering
			SNeighbor neighbor;
			neighborhood.clear();
			const cimg_library::CImgList<T> &_mask = mask();
			int cx = 0, cy = 0;
			std::vector< int > queueX;
			std::vector< int > queueY;
			cimg_library::CImg<bool> visited( 2*r+1, 2*r+1, 1, 1, false );
			queueX.clear();
			queueY.clear();
			queueX.push_back( xx );
			queueY.push_back( yy );
			while (!queueX.empty())
			{
				cx = queueX.back();
				cy = queueY.back();
				queueX.pop_back();
				queueY.pop_back();
				visited.atXYZC( r + cx - xx, r + cy - yy, 0, 0 ) = true;

				neighbor.value = T(img.atNXYZC( nn, cx, cy, zz, cc ));
				neighbor.weight = _cliqueWeight->getCliqueWeight(
					nn, xx, yy, zz, cc, 0, cx-xx, cy-yy, 0, 0 );
				neighborhood.push_back( neighbor );

				// push back north neighbor
				if (!visited.atXYZC( r + cx - xx, r + cy-1 - yy, 0, 0 )
				&&  _mask[0].atXYZC( cx, cy-1, 0, 0 )
				&&  ((cx-xx  )*(cx-xx  ) <= r*r)
				&&  ((cy-1-yy)*(cy-1-yy) <= r*r))
				{
					queueX.push_back( cx );
					queueY.push_back( cy-1 );
				}

				// push back east neighbor
				if (!visited.atXYZC( r + cx+1 - xx, r + cy - yy, 0, 0 )
				&&  _mask[0].atXYZC( cx+1, cy, 0, 0 )
				&&  ((cx+1-xx)*(cx+1-xx) <= r*r)
				&&  ((cy-yy  )*(cy-yy  ) <= r*r))
				{
					queueX.push_back( cx+1 );
					queueY.push_back( cy );
				}

				// push back south neighbor
				if (!visited.atXYZC( r + cx - xx, r + cy+1 - yy, 0, 0 )
				&&  _mask[0].atXYZC( cx, cy+1, 0, 0 )
				&&  ((cx-xx  )*(cx-xx  ) <= r*r)
				&&  ((cy+1-yy)*(cy+1-yy) <= r*r))
				{
					queueX.push_back( cx );
					queueY.push_back( cy+1 );
				}

				// push back west neighbor
				if (!visited.atXYZC( r + cx-1 - xx, r + cy - yy, 0, 0 )
				&&  _mask[0].atXYZC( cx-1, cy, 0, 0 )
				&&  ((cx-1-xx)*(cx-1-xx) <= r*r)
				&&  ((cy-yy  )*(cy-yy  ) <= r*r))
				{
					queueX.push_back( cx-1 );
					queueY.push_back( cy );
				}
			}
		} else {
			for (int x=-r; x<r+1; ++x)
			for (int y=-r; y<r+1; ++y)
			{
				neighborhood[idx].value  = T(img.atNXYZC( nn, xx+x, yy+y, zz, cc ));
				neighborhood[idx].weight = _cliqueWeight->getCliqueWeight(
					nn, xx, yy, zz, cc, 0, x, y, 0, 0 );
				++idx;
			}
		}

		std::sort( neighborhood.begin(), neighborhood.end(), by_value() );

		weight_sum = T(0.0);
		for (  itNbh =  neighborhood.begin();
		       itNbh != neighborhood.end();
		     ++itNbh)
		{
			weight_sum += itNbh->weight ;
		}
		weight_sum /= 2.0;

		partial_weight_sum = T(0.0);
		idx = 0;
		for (  itNbh =  neighborhood.begin();
		       itNbh != neighborhood.end();
		     ++itNbh)
		{
			partial_weight_sum += itNbh->weight ;
			if (partial_weight_sum >= weight_sum) {
				break;
			}
			idx++;
		}

		o( nn, xx, yy, zz, cc ) = neighborhood[idx].value;
	}
}

#endif /* _WEIGHTEDMEDIAN_HXX_ */

