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
 *  @file SurfaceAnalysisMinChange.h
 *  @brief definition of SurfaceAnalysisMinChange, child class of SurfaceAnalysis
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 15.09.2009
 */

#ifndef SURFACEANALYSISMINCHANGE_H_
#define SURFACEANALYSISMINCHANGE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef surfaceanalysisminchange_EXPORTS
///Visual C++ specific code
#define surfaceanalysisminchange_DECLDIR __declspec(dllexport)
#else
#define surfaceanalysisminchange_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define surfaceanalysisminchange_DECLDIR
#endif

#include <SurfaceAnalysis.h>

template<typename T>
class surfaceanalysisminchange_DECLDIR SurfaceAnalysisMinChange: public SurfaceAnalysis<
		T>
{
public:
	///standard constructor
	SurfaceAnalysisMinChange(const std::string name);

	void execute();

	/**
	 *  find the minimal change in roi
	 *  simple implementation of finding minimum in a list of values
	 *  @param differences vector of all differences
	 *  @param parameters all possible parameters
	 *  @param [out] vector of IncrementorParameter for best fitting Parameters
	 */
	virtual std::vector<IncrementorParameter<T>*> & findMinChange(
			std::vector<T>& differences, std::vector<std::vector<
					IncrementorParameter<T>*> >& parameters);

};

#endif /* SURFACEANALYSISMINCHANGE_H_ */
