/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// \file SteeredDerivative.hxx
/// Implementation of the parameter class SteeredDerivative
/// \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// \date 09.10.2009

#ifndef STEERED_DERIVATIVE_HXX_
#define STEERED_DERIVATIVE_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "SteeredDerivative.h"
#include <cassert>

template <typename T>
SteeredDerivative<T>::SteeredDerivative(const std::string& name) :
		TemplatedParameteredObject<T>("SteeredDerivative", name,
		"calculate derivatives aligned and orthogonal to the local "
		"image structure")
{
	ParameteredObject::_addInputSlot(dX, "dX",
		"derivative in x direction", "CImgList<T>");
	ParameteredObject::_addInputSlot(dY, "dY",
		"derivative in y direction", "CImgList<T>");
	ParameteredObject::_addOutputSlot(dA, "dA",
		"derivative aligned to image structure", "CImgList<T>");
	ParameteredObject::_addOutputSlot(dO, "dO",
		"derivative orthogonal to image structure", "CImgList<T>");
	ParameteredObject::_addInputSlot(eigenVectors, "eigenVectors",
		"Eigenvectors of structure tensor", "CImgList<T>");
}

template <typename T>
void SteeredDerivative<T>::execute() {
	assert(dX().is_sameNXYZC(dY()));
	assert(dX().is_sameXYZ(eigenVectors()));
	assert(eigenVectors().size() >= 2);
	assert(eigenVectors()[0].spectrum() >= dX()[0].spectrum());

	cimg_library::CImg<T> eig0 = eigenVectors()[0].get_crop(0,0,0,0,
		dX()[0].width()-1, dX()[0].height()-1, dX()[0].depth()-1, dX()[0].spectrum()-1);
	cimg_library::CImg<T> eig1 = eigenVectors()[1].get_crop(0,0,0,0,
		dX()[0].width()-1, dX()[0].height()-1, dX()[0].depth()-1, dX()[0].spectrum()-1);
	assert(eig0.is_sameXYZC(dX()[0]));
	assert(eig1.is_sameXYZC(dX()[0]));

	dA().assign(dX().size(), dX()[0].width(), dX()[0].height(),
		dX()[0].depth(), dX()[0].spectrum());
	dO().assign(dX().size(), dX()[0].width(), dX()[0].height(),
		dX()[0].depth(), dX()[0].spectrum());
	cimglist_for(dX(), kk) {
		dO()[kk] = eig0.get_mul(dX()[kk]) + eig1.get_mul(dY()[kk]);
		dA()[kk] = eig0.get_mul(dY()[kk]) - eig1.get_mul(dX()[kk]);
	}
}

#endif // STEERED_DERIVATIVE_HXX_
