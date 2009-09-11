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
 *  @file BlockmatchingLIACS.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 03.09.2009
 */

#ifndef BLOCKMATCHINGLIACS_HXX_
#define BLOCKMATCHINGLIACS_HXX_

#include "BlockMatchingLIACS.h"
#include "BlockMatching.hxx"
#include "ListedPixelSelection.hxx"
#include "IncrementorCountUp.hxx"
#include "ObjectiveFunctionComparing.hxx"

template<typename T>
BlockMatchingLIACS<T>::BlockMatchingLIACS(const std::string& name) :
	BlockMatching<T>::BlockMatching(name)
{

}

template<typename T>
void BlockMatchingLIACS<T>::findFlow()
{
	std::vector<T> pixelProperties;
//	pixelProperties = this->brightnessModel()->apply(
//			this->newPos()->getListOfParams());
	//	pixelProperties = this->motionModel()->apply(this->newPos()->getListOfParams());		///@TODO gibts noch net


}

#endif /* BLOCKMATCHINGLIACS_HXX_ */

