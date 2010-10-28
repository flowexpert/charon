/*  This file is part of Charon.

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
/**
 *  @file ObjectiveFunction.hxx
 *  @brief implementation of class ObjectiveFunction
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef OBJECTIVEFUNKTION_HXX_
#define OBJECTIVEFUNKTION_HXX_

#include "ObjectiveFunction.h"
#include "ParameteredObject.hxx"
#include "PixelSelection.h"

template<typename T>
ObjectiveFunction<T>::ObjectiveFunction(const std::string& name,
		const std::string pluginName) :
	TemplatedParameteredObject<T>::TemplatedParameteredObject(pluginName, name,
			"computes the difference between all way the parameters "
				"are computed")
{
	out = this;
	this->_addInputSlot(sequence, "sequence", "Sequence of Images",
			"cimglist<T>");
	this->_addInputSlot(brightnessModel, "brightnessModel",
			"Pointer to BrightnessModel", "BrightnessModel*");
	this->_addInputSlot(motionModel, "motionModel", "Pointer to MotionModel",
			"MotionModel*");
	this->_addInputSlot(interpolator, "interpolator",
			"Pointer to Interpolator", "Interpolator<T>*");
	this->_addOutputSlot(out, "out", "this OutputSlot", "ObjectiveFunction<T>*");
}

#endif /* OBJECTIVEFUNKTION_HXX_ */
