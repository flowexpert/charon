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
/// @file MovingIllumination.hxx
/// implements class MovingIllumination
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _BRIGHTNESSMODELS_MOVINGILLUMINATION_HXX_
#define _BRIGHTNESSMODELS_MOVINGILLUMINATION_HXX_

#include "MovingIllumination.h"

using namespace std;
using namespace cimg_library;
using namespace BrightnessModels;

template<class T>
std::set<std::string>& BrightnessModels::MovingIllumination<T>::getUnknowns()
{
	this->unknowns.insert("a1");
	this->unknowns.insert("a2");
	return this->unknowns;
}

template<class T>
void BrightnessModels::MovingIllumination<T>::compute(const int xs,
		const int ys, const int zs, const int t, const int v, std::map<
				std::string, T>& term, T& rhs)
{
	//	assert(term.width == 2);
	term["a1"] += 1.0;
	term["a2"] += t;
}

template<class T>
BrightnessModels::MovingIllumination<T>::MovingIllumination(const string& name) :
	BrightnessModel<T> ("brightnessmodels_movingillumination", name), functor()
{
	//	brightnessFunctor = &functor;
	_addInputSlot(sequence, "sequence", "Sequence of Images", "CImgList");
	this->setFunctorParams(-0.003, -0.001);
}

template<class T>
void BrightnessModels::MovingIllumination<T>::setFunctorParams(float a1,
		float a2)
{
	this->functor.a1 = a1;
	this->functor.a2 = a2;
}

template<class T>
void BrightnessModels::MovingIllumination<T>::Functor::operator ()(
		cimg_library::CImg<T>& sequence) const
{
	for (unsigned int t = 0; t < sequence.depth; t++)
	{
		cimg_forXY(sequence,x,y)
			{
				sequence(x, y, t) *= exp(-(cimg::sqr(x - 100 - 0.4 * t)
						+ cimg::sqr(y - 100 - 0.5 * t)) / 10000.0);
			}
	}
}

template<class T>
void BrightnessModels::MovingIllumination<T>::apply(
		const Pixel<T> & inPixel, const std::vector<Parameter<T>*> & modifier,
		Pixel<T> & outPixel)
{
	outPixel = inPixel;
}

#endif
