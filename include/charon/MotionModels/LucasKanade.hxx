/*  Copyright (C) 2012 Helen Morrison

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
/// @file LucasKanade.hxx
/// implements class LucasKanade
/// @author <a href="mailto:helen.morrison@iwr.uni-heidelberg.de">
///		Helen Morrison</a>
/// @date 13.04.2012

#ifndef _MotionModels_LucasKanade_hxx_
#define _MotionModels_LucasKanade_hxx_

#include "LucasKanade.h"
#include <charon-utils/Convolution.h>
template<class T>
std::set<std::string>& MotionModels::LucasKanade<T>::getUnknowns()
{
	this->unknowns.insert("a1");
	this->unknowns.insert("a2");
	return this->unknowns;
}

template<class T>
void MotionModels::LucasKanade<T>::compute(
		const Point4D<int>& p, const int& v,
		std::map<std::string, T>& term, T& rhs,
		const std::string& unknown)
{
	if (unknown.length()) {
		if (unknown == "a1"){
			term["a1"] += this->IxIx()(v, p.x, p.y, p.z, p.t);
			term["a2"] += this->IxIy()(v, p.x, p.y, p.z, p.t);
			rhs -= this->IxIt()(v, p.x, p.y, p.z, p.t);
		}
		else if (unknown == "a2"){
			term["a1"] += this->IxIy()(v, p.x, p.y, p.z, p.t);
			term["a2"] += this->IyIy()(v, p.x, p.y, p.z, p.t);
			rhs -= this->IyIt()(v, p.x, p.y, p.z, p.t);
		}
		else {
			std::ostringstream msg;
			msg << __FILE__ << ":" << __LINE__ << std::endl;
			msg << "\tInvalid Unknown given!" << std::endl;
			msg << "\tGiven unknown: \"" << unknown << "\"";
			throw std::out_of_range(msg.str().c_str());
		}
	}

}

template<class T>
MotionModels::LucasKanade<T>::LucasKanade(const std::string& name) :
		MotionModel<T>::MotionModel("motionmodels_lucaskanade", name)
{
	_addInputSlot(IxIx, "IxIx", "<I_x * I_x>", "CImgList<T>");
	_addInputSlot(IxIy, "IxIy", "<I_x * I_y>", "CImgList<T>");
	_addInputSlot(IyIy, "IyIy", "<I_y * I_y>", "CImgList<T>");
	_addInputSlot(IxIt, "IxIt", "<I_x * I_t>", "CImgList<T>");
	_addInputSlot(IyIt, "IyIt", "<I_y * I_t>", "CImgList<T>");
}
#endif
