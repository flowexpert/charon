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
 *  @file ObjectiveFunctionComparing.hxx
 *  defines the ObjectiveFunctionComparing class
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef OBJECTIVEFUNKTIONCOMPARING_H_
#define OBJECTIVEFUNKTIONCOMPARING_H_

#include "../ObjectiveFunction.h"

template<typename T>
class ObjectiveFunctionComparing: ObjectiveFunction<T>
{
public:
	ObjectiveFunctionComparing(const std::string& name);
	InputSlot<cimg_library::CImg<T>&> image;

	virtual T compare(const std::vector<Pixel<T>*> & pixelList,
			const std::vector<IncrementorParameter<T>*>& params);

};

#endif /* OBJECTIVEFUNKTIONCOMPARING_H_ */
