/*  Copyright (C) 2013 Gerald Mwangi

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
/** \file TVFilterVigra.hxx
 *  Implementation of the parameter class TVFilterVigra.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 12.06.2013
 */

#ifndef _TVFILTERVIGRA_HXX_
#define _TVFILTERVIGRA_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "TVFilterVigra.h"
#include <vigra/tv_filter.hxx>

template <typename T>
TVFilterVigra<T>::TVFilterVigra(const std::string& name) :
		TemplatedParameteredObject<T>(
			"TVFilterVigra", name,
			"<h2>Wrapper for the total variation filter in the "
			"VIGRA-Suite</h2><br>"
			"Wrapper for the total variation filter in the VIGRA-Suite"
		)
{
	ParameteredObject::_addInputSlot(
		in, "in",
		"input image",
		"VigraArray5<T>");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"output image",
		"VigraArray5<T>");

	ParameteredObject::_addParameter< double >(
		alpha, "alpha",
		"TV smoothing parameter",
		1, "double");
	ParameteredObject::_addParameter< int >(
		steps, "steps",
		"maximal number of iteration steps",
		10, "int");
	ParameteredObject::_addParameter< double >(
		eps, "eps",
		"The algorithm stops, if the primal-dual gap is below the threshold"
		"eps",
		0, "double");

}

template <typename T>
void TVFilterVigra<T>::execute() {
    vigra::MultiArrayShape<5>::type shape=in().shape();
    typename vigra::MultiArrayShape<2>::type new_shape;
    new_shape[0]=shape[0];
    new_shape[1]=shape[1];
    vigra::MultiArray<2,double> in2D(new_shape,((vigra::MultiArray<5,double>)in()).data());
    vigra::MultiArray<2,double> out2D(new_shape);
    vigra::totalVariationFilter(in2D,out2D,alpha(),steps(),eps());
    out()=vigra::MultiArray<5,T>(shape,((vigra::MultiArray<2,T>)out2D).data());
    //out().reshape(shape,((const vigra::MultiArray<2,T>)out2D));
}

#endif /* _TVFILTERVIGRA_HXX_ */
