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
 *  @file SurfaceAnalysisMinChange.hxx
 *  @brief impelmentation of SurfaceAnalysisMinChange, child class of 
 *  SurfaceAnalysis
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 15.09.2009
 */

#ifndef SURFACEANALYSISMINCHANGE_HXX_
#define SURFACEANALYSISMINCHANGE_HXX_

#include "SurfaceAnalysisMinChange.h"
#include <SurfaceAnalysis.hxx>

template<typename T>
SurfaceAnalysisMinChange<T>::SurfaceAnalysisMinChange(const std::string name) :
	SurfaceAnalysis<T>::SurfaceAnalysis(name)
{

}

template<typename T>
std::vector<IncrementorParameter<T>*> & SurfaceAnalysisMinChange<T>::findMinChange(
		std::vector<T>& differences, std::vector<std::vector<
				IncrementorParameter<T>*> >& parameters)
{
	//find minimum
	unsigned int tempMin = 0;
	for (unsigned int i = 1; i != differences.size() - 1; i++)
	{
		if (differences[i] < differences[tempMin])
		{
			tempMin = i;
		}
		if (i == parameters.size())
		{
			break;
		}
	}
	return parameters[tempMin];
}

#endif /* SURFACEANALYSISMINCHANGE_HXX_ */

