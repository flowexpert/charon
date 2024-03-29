/*  Copyright (C) 2009 René Steinbrügge

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
/// @file Constant.hxx
/// implements class Constant
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 27.05.2009

#ifndef _BrightnessModels_Constant_HXX_
#define _BrightnessModels_Constant_HXX_

#include "Constant.h"

template<class T>
BrightnessModels::Constant<T>::Constant(const std::string& name) :
	BrightnessModel<T> ("brightnessmodels_constant", name)
{
	_addFunction(BrightnessModels::Constant<T>::getUnknowns);
	_addFunction(BrightnessModels::Constant<T>::compute);

	// nasty quick fix, forces constructor to be compiled into
	// shared object file, without: optimized away (perhaps too simple)
	if (!this) {
		sout << "dummy call, you should never see this" << std::endl;
	}
}

template<class T>
std::set<std::string>& BrightnessModels::Constant<T>::getUnknowns()
{
	return this->unknowns;
}

template<class T>
void BrightnessModels::Constant<T>::compute(
		const Point4D<int>&, const int&,
		std::map<std::string, T>& , T&,
		const std::string&)
{
	// no brightness change
}

#endif
