/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file Threshold.hxx
 *  Implementation of the parameter class Threshold.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 25.01.2011
 */

#ifndef _THRESHOLD_HXX_
#define _THRESHOLD_HXX_

#include "Threshold.h"
#include <vigra/multi_pointoperators.hxx>

template <typename T>
Threshold<T>::Threshold(const std::string& name) :
	TemplatedParameteredObject<T>("threshold", name,
		"This class uses the vigra threshold command to threshold "
		"images at the given boundaries. If image values are "
		"between boundaries (i.e. within the closed interval "
		"[lower, heigher]) result is set to yesresult, "
		"otherwise to noresult.")
{
	ParameteredObject::_addParameter<T>(
			lower,  "lower",  "lower bound", 0, "T");
	ParameteredObject::_addParameter<T>(
			higher, "higher", "upper bound", 1, "T");
	ParameteredObject::_addParameter<T>(
			noresult,   "noresult",
			"result if out of bounds", 0, "T");
	ParameteredObject::_addParameter<T>(
			yesresult, "yesresult",
			"result if value between boundaries", 1, "T");
	ParameteredObject::_addInputSlot (
			in,     "in",     "image input",  "vigraArray5<T>");
	ParameteredObject::_addOutputSlot(
			out,    "out",    "image output", "vigraArray5<T>");
}

template <typename T>
void Threshold<T>::execute() {
	out().reshape(in().shape());

	vigra::transformMultiArray(
			vigra::srcMultiArrayRange(in()),
			vigra::destMultiArrayRange(out()),
			vigra::Threshold<T,T>(
					lower(),higher(),noresult(),yesresult()));
}

#endif /* _THRESHOLD_HXX_ */
