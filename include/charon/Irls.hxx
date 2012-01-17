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
/** \file Irls.hxx
 *  Implementation of the parameter class Irls.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 06.01.2012
 */

#ifndef _IRLS_HXX_
#define _IRLS_HXX_

#include "Irls.h"

#define EPS_INVERSE 1e9

template<typename T>
Irls<T>::Irls(const std::string& name) :
	TemplatedParameteredObject<T>(
			"Irls", name,
			"calculate median of image windows<br><br>"
			"Use image windows of given size, calculate median within this "
			"window and use the result as new value for the window center "
			"pixel. This eliminates outliers and makes "
			"e.g. flow estimation more robust. "
			"This uses the IRLS approach.")
{
	this->_addInputSlot(in,    "in",    "image input",  "CImgList<T>");
	this->_addInputSlot(inWeight, "inWeight", "weight input", "CImgList<T>");
	this->_addOutputSlot(out,  "out",   "image output", "CImgList<T>");
	this->_addParameter(
			windowRadius, "windowRadius",
			"radius r of image windows (size is 2*r+1)",  7u);
	this->_addParameter(
			iterations, "iterations",
			"iteration count (0 => mean filtering)", 10u);
}

template<typename T>
void Irls<T>::execute() {
	const cimg_library::CImgList<T>& i = in();
	const cimg_library::CImgList<T>& inweight = inWeight();

	cimg_library::CImgList<T>& o = out();
	const unsigned int& r = windowRadius();
	o = i;

	int dim = i.size();
	std::vector<cimg_library::CImg<T> > window( dim );
	std::vector<T> med( dim );
	std::vector<T> tmp( dim );

	int _iterations = iterations();

	T weight, weight_sum, dist;

	cimg_forXYZC(o[0],xx,yy,zz,tt) {
		for (int d=0; d<dim; ++d) {
			window[d] = i[d].get_crop(xx-r,yy-r,zz,tt,xx+r,yy+r,zz,tt);
		}

		// calculate mean as an initial guess
		for (int d=0; d<dim; ++d) {
			med[d] = T(0);
			weight_sum = T(0);
			for (unsigned int i=0; i<2*r+1; ++i)
			for (unsigned int j=0; j<2*r+1; ++j)
			{
				weight_sum += inweight[0](i,j,zz,tt);
				med[d] += (inweight[0](i,j,zz,tt) *
				           window[d]( i, j, zz, tt ));
			}
			med[d] /= weight_sum;
		}

		// perform iteratively weighted least squares
		// see "Numerical Methods for Scientific Computing Vol. II"
		// by Germund Dahlquist and Åke Björck, section 8.7.5 for details
		for (int i=0; i<_iterations; ++i) {
			weight_sum = T(0);
			for (int d=0; d<dim; ++d) {
				tmp[d] = T(0);
			}
			for (unsigned int i=0; i<2*r+1; ++i)
			for (unsigned int j=0; j<2*r+1; ++j)
			{
				dist = T(0);
				for (int d=0; d<dim; ++d) {
					dist += pow( window[d]( i, j, zz, tt ) - med[d], 2 );
				}
				dist = pow( dist, 0.5 );

				if (dist != 0) {
					weight = 1.0 / dist;
				} else {
					weight = EPS_INVERSE;
				}

				weight *= inweight[0]( i, j, zz, tt );

				weight_sum += weight;
				for (int d=0; d<dim; ++d) {
					tmp[d] += weight * window[d]( i, j, zz, tt );
				}
			}
			for (int d=0; d<dim; ++d) {
				med[d] = tmp[d] / weight_sum;
			}
		}

		// write back
		for (int d=0; d<dim; ++d) {
			o(d,xx,yy,zz,tt) = med[d];
		}
	}
}

#endif /* _IRLS_HXX_ */

