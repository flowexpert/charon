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
 *  @file SurfaceAnalysis.hxx
 *  @brief implementation of abstract class SurfaceAnalysis
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef _SURFACEANALYSIS_HXX_
#define _SURFACEANALYSIS_HXX_

#include "SurfaceAnalysis.h"
#include <ParameteredObject.hxx>

template<typename T>
SurfaceAnalysis<T>::SurfaceAnalysis(const std::string& name) :
	TemplatedParameteredObject<T> ("SurfaceAnalysis", name,
			"Computes best fitting Parameters, for the flow"), out(this)
{
	this->_addOutputSlot(out, "out",
			"Computes best fitting Parameters, for the flow",
			"SurfaceAnalysis<T>*");
}

template<typename T>
void SurfaceAnalysis<T>::execute()
{
	ParameteredObject::execute();
}

#endif
