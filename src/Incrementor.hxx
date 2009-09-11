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
 *  @file Incrementor.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef INCREMENTOR_HXX_
#define INCREMENTOR_HXX_

#include "Incrementor.h"
#include "ParameteredObject.hxx"

template<typename T>
Incrementor<T>::Incrementor(const std::string& name) :
	TemplatedParameteredObject<T> ("incrementor", name,
			" Increments the Parameters, witch are needed to compute changes "
				"inside the Image "), paramList(false, true), out(this)
{
	this->_addInputSlot(image, "Image", "Image where to Increment", "CImg");
	this->_addInputSlot(paramList, "paramList",
			"list of Parameters for Incrementor", "Incrementor Parameter");
	this->_addOutputSlot(out, "out",
			"Position in Image after Incrementation", "Incrementor<T>*");
}

template<typename T>
void Incrementor<T>::execute()
{
	ParameteredObject::execute();
}

template<typename T>
std::vector<Parameter<T>* >& Incrementor<T>::getListOfParams()
{
	return this->listOfParams;
}

#endif
