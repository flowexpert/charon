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
 *  @file SurfaceAnalysis.h
 *  definition of absract class SurfaceAnalysis
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef SURFACEANALYSIS_H_
#define SURFACEANALYSIS_H_

#include "ParameteredObject.h"
#include "IncrementorParameter.h"
#include <vector>

template<typename T>
class SurfaceAnalysis: TemplatedParameteredObject<T>
{
public:
	/// standard constructor
	SurfaceAnalysis(const std::string& name);
	OutputSlot<SurfaceAnalysis<T>*> out;
	///standard execute from ParameterdObject
	void execute();
	/**
	 *  find the minimal change in roi
	 *  @param differences vector of all differences
	 *  @param parameters all possible parameters
	 *  @param [out] vector of IncrementorParameter for best fitting Parameters
	 */
	virtual std::vector<IncrementorParameter<T>*> & findMinChange(
			std::vector<T>& differences, std::vector<std::vector<
					IncrementorParameter<T>*> >& parameters) =0;
};

#endif /* SURFACEANALYSIS_H_ */
