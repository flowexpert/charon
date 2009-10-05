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
/// @file Exponential.cpp
/// implements class Exponential
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _BrightnessModels_Exponential_hxx_
#define _BrightnessModels_Exponential_hxx_

#include "Exponential.h"
#include "Pixel.hxx"

template<class T>
std::set<std::string>& BrightnessModels::Exponential<T>::getUnknowns()
{
	this->unknowns.insert("k");
	return this->unknowns;
}

template<class T>
void BrightnessModels::Exponential<T>::compute(const int xs, const int ys,
		const int zs, const int t, const int v, std::map<std::string, T>& term,
		T& rhs)
{
	if (sequence().size == 1) // 2D
		term["k"] += -(sequence()(0, xs, ys, t, v));
	else
		// 3D
		term["k"] += -(sequence()(t, xs, ys, zs, v));
}

template<class T>
BrightnessModels::Exponential<T>::Exponential(const string& name) :
	BrightnessModel<T> ("brightnessmodels_exponential", name), functor()
{
	//	brightnessFunctor = &functor;
	_addInputSlot(sequence, "sequence", "Sequence of Images", "CImgList");
	this->setFunctorParams(0.02);
}

template<class T>
void BrightnessModels::Exponential<T>::setFunctorParams(float k)
{
	this->functor.k = k;
}

template<class T>
void BrightnessModels::Exponential<T>::Functor::operator ()(cimg_library::CImg<
		T> & sequence) const
{
	//cout << "!!functor!!" << endl;
	for (unsigned int t = 1; t < sequence.depth; t++)
	{
		//cout << exp(-(this->k)*t) << endl;
		cimg_forXY(sequence,x,y)
			{
				sequence(x, y, t) *= exp(-(this->k) * t);
			}
	}
}

template<class T>
void BrightnessModels::Exponential<T>::Functor::get3d(
		cimg_library::CImgList<T> &sequence) const
{
	//cout << "!!functor!!" << endl;
	for (unsigned int t = 1; t < sequence.size; t++)
	{
		//cout << exp(-(this->k)*t) << endl;
		cimg_forXYZ(sequence(t),x,y,z)
				{
					sequence(t, x, y, z) *= exp(-(this->k) * t);
				}
	}
}

template<class T>
void BrightnessModels::Exponential<T>::apply(const Pixel<T> & inPixel,
		const std::vector<IncrementorParameter<T>*> & modifier,
		Pixel<T> & outPixel)
{
	outPixel = inPixel;
	///@TODO implement me
}

#endif
