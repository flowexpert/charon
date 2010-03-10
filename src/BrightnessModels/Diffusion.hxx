/*  Copyright (C) 2009 René Steinbrügge

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
/// @file Diffusion.cpp
/// implements class Diffusion
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _Diffusion_hxx_
#define _Diffusion_hxx_

#include "Diffusion.h"
#include <vector>

template<typename T>
BrightnessModels::Diffusion<T>::Diffusion(const std::string& name) :
	BrightnessModel<T> ("brightnessmodels_diffusion", name)
{
	this->_addInputSlot(dxx, "dxx", "2nd derivative in x-direction", "CImgList");
	this->_addInputSlot(dyy, "dyy", "2nd derivative in y-direction", "CImgList");
	this->_addInputSlot(dzz, "dzz", "2nd derivative in z-direction", "CImgList");

	this->brightnessFunctor = &functor;

	this->setFunctorParams(1.5);
}

template<typename T>
std::set<std::string>& BrightnessModels::Diffusion<T>::getUnknowns()
{
	this->unknowns.insert("d");
	return this->unknowns;
}

template<class T>
void BrightnessModels::Diffusion<T>::compute(const int xs, const int ys,
		const int zs, const int t, const int v, std::map<std::string, T>& term,
		T& rhs)
{
	//assert(term.width == 1);
	if (this->dxx().size == 1) { // 2D
		term["d"] += this->dxx()(v, xs, ys, 0, t) +
		             this->dyy()(v, xs, ys, 0, t);
	} else {
		// 3D
		term["d"] += this->dxx()(v, xs, ys, zs, t) +
		             this->dyy()(v, xs, ys, zs, t) +
		             this->dzz()(v, xs, ys, zs, t);
	}
}


template<class T>
void BrightnessModels::Diffusion<T>::computeEnergy(
				const int xs, const int ys, const int zs, const int t, const int v,
				const cimg_library::CImgList<T>& parameterList, double& energy)
 {
 }

template<class T>
void BrightnessModels::Diffusion<T>::Functor::operator()(cimg_library::CImg<T> &sequence) const {
	for (unsigned int t = 1; t < sequence.depth; t++)
		sequence.draw_image(0, 0, t, sequence.get_slice(t).convolve(
				Functions::gauss_filter(sqrt(2 * (this->d) * t))));
}

template<class T>
void BrightnessModels::Diffusion<T>::Functor::get3d(
		cimg_library::CImgList<T> &sequence) const
{
	for (unsigned int t = 1; t < sequence.size; t++)
	{
		cout << t << endl;
		sequence(t).convolve(Functions::gauss_filter3d(
				sqrt(2.0 * (this->d) * t)));
	}
}

template<class T>
void BrightnessModels::Diffusion<T>::setFunctorParams(float d)
{
	this->functor.d = d;
}

template<class T>
void BrightnessModels::Diffusion<T>::apply(const Pixel<T> & inPixel,
		const std::vector<IncrementorParameter<T>*> & modifier,
		Pixel<T> & outPixel) {
	outPixel = inPixel;
	///@TODO implement me
}

#endif
