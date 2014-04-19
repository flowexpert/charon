/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file Irls.hxx
 *  Implementation of the parameter class Irls.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 06.01.2012
 */

#ifndef _IRLS_HXX_
#define _IRLS_HXX_

#include "Irls.h"

template<typename T>
Irls<T>::Irls(const std::string& name) :
	TemplatedParameteredObject<T>(
			"Irls", name,
			"This module performs the (i)teratively (r)eweighted (l)east "
			"(s)quares approach in order to obtain a resonable (multi-dimensional) "
			"median approximation, motivated by the fact that the median is "
			"non-existant for data of dimension 2 or higher. "
			"Starting with a mean approximation, the algorithm refines that mean iteratively "
			"by reweighting the datapoints, whereas weights depend upon the datapoint's "
			"distances to the current mean approximation, "
			"eventually yielding a median approximation. "
			"Here, the weights result from a normal distribution, parameterized with "
			"sigma_dist. "
			"Additionally, they are multiplied with individual clique weights, "
			"depending on the connected cliqueWeight function. "
			)
{
	this->_addInputSlot(in,    "in",    "image input",  "CImgList<T>");
	this->_addInputSlot(cliqueWeight, "cliqueWeight", "clique weight function input", "CliqueWeight<T>*");
	this->_addOutputSlot(out,  "out",   "image output", "CImgList<T>");
	this->_addParameter(
			windowRadius, "windowRadius",
			"radius r of image windows (size is 2*r+1)",  7u);
	this->_addParameter(
			iterations, "iterations",
			"iteration count (0 => mean filtering)", 10u);
	this->_addParameter(sigma_dist, "sigma_dist",
			"spatial distance weight", T(7));

	ParameteredObject::_setTags("charon-flow;MedianFilters;CImg");
}

template<typename T>
void Irls<T>::execute() {
	const cimg_library::CImgList<T>& i = in();
	CliqueWeight<T> *_cliqueWeight = cliqueWeight();

	cimg_library::CImgList<T>& o = out();
	const int& r = windowRadius();
	const T _sigma_dist = sigma_dist();

	o = i;

	int dim = i.size();
	std::vector<T> med( dim );
	std::vector<T> tmp( dim );

	int _iterations = iterations();

	T weight, weight_sum, dist;

	cimg_forXYZC(o[0],xx,yy,zz,tt) {

		// calculate mean as an initial guess
		for (int d=0; d<dim; ++d) {
			tmp[d] = T(0);
		}
		weight_sum = T(0);
		for (int x=-r; x<r+1; ++x)
		for (int y=-r; y<r+1; ++y)
		{
			weight = _cliqueWeight->getCliqueWeight(
				0, xx, yy, zz, tt, 0, x, y, 0, 0 );
			weight_sum += weight;
			for (int d=0; d<dim; ++d) {
				tmp[d] += weight * i.atNXYZC(d,xx+x,yy+y,zz,tt);
			}
		}
		for (int d=0; d<dim; ++d) {
			med[d] = tmp[d] / weight_sum;
		}

		// perform iteratively reweighted least squares
		// see "Numerical Methods for Scientific Computing Vol. II"
		// by Germund Dahlquist and Åke Björck, section 8.7.5 for details
		for (int iter=0; iter<_iterations; ++iter) {
			for (int d=0; d<dim; ++d) {
				tmp[d] = T(0);
			}
			weight_sum = T(0);
			for (int x=-r; x<r+1; ++x)
			for (int y=-r; y<r+1; ++y)
			{
				dist = T(0);
				for (int d=0; d<dim; ++d) {
					dist += pow( double(i.atNXYZC(d,xx+x,yy+y,zz,tt) - med[d]), double(2) );
				}
				dist = pow( double(dist), double(0.5) );

				weight = _gauss( dist, 0, _sigma_dist );

				weight *= _cliqueWeight->getCliqueWeight(
					0, xx, yy, zz, tt, 0, x, y, 0, 0 );
				weight_sum += weight;
				for (int d=0; d<dim; ++d) {
					tmp[d] += weight * i.atNXYZC(d,xx+x,yy+y,zz,tt);
				}
			}
			for (int d=0; d<dim; ++d) {
				med[d] = tmp[d] / weight_sum;
			}
		}

		// write back
		for (int d=0; d<dim; ++d) {
			o.atNXYZC(d,xx,yy,zz,tt) = med[d];
		}
	}
}

template <typename T>
inline T Irls<T>::_gauss( T x, T mu, T sigma )
{
	return 1/(sqrt(2*M_PI*sigma*sigma))
	  * exp(double(-(x - mu)*(x - mu)/(2*sigma*sigma))) ;
}

#endif /* _IRLS_HXX_ */

