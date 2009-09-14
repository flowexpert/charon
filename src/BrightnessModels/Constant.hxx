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
/// @file Constant.hxx
/// implements class Constant
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _BrightnessModels_Constant_HXX_
#define _BrightnessModels_Constant_HXX_

#include "Constant.h"

using namespace std;
using namespace cimg_library;

BrightnessModels::Constant<T>::Constant(const string& name) :
	BrightnessModel<T> ("brightnessmodels_diffusion", name)
{
	_addInputSlot(sequence, "sequence", "Sequence of Images", "CImgList");
}

template<class T>
std::set<std::string>& BrightnessModels::Constant<T>::getUnknowns() const
{
	return this->unknowns;
}

template<class T>
void BrightnessModels::Constant<T>::compute(const int xs, const int ys,
		const int zs, const int t, const int v, std::map<std::string, T>& term,
		T& rhs)
{
	// no brightness change
}

template<class T>
void BrightnessModels::Constant<T>::apply(const Pixel<T> & inPixel,
		const std::vector<Parameter<T>*> & modifier, Pixel<T> & outPixel)
{
	outPixel = inPixel;
	// no brightness change
}

#endif
