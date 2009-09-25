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
 *  @brief defines the ObjectiveFunctionComparing class, child class of 
 *  ObjectiveFunction
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef OBJECTIVEFUNKTIONCOMPARING_H_
#define OBJECTIVEFUNKTIONCOMPARING_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef objectivefunctioncomparing_EXPORTS
///Visual C++ specific code
#define objectivefunctioncomparing_DECLDIR __declspec(dllexport)
#else
#define objectivefunctioncomparing_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define objectivefunctioncomparing_DECLDIR
#endif

#include <ObjectiveFunction.h>

template<typename T>
class objectivefunctioncomparing_DECLDIR ObjectiveFunctionComparing: public ObjectiveFunction<T>
{
public:
	/// standard constructor
	ObjectiveFunctionComparing(const std::string& name);

	/// standard execute method
	virtual void execute();

	/**
	 *  compares the changed List of Pixels from the apply function
	 *  (BrightnessModel) with the next picture and try to find the right
	 *  position for every pixel.
	 *  @param pixelList list of pixel from PixelSelection
	 *  @param intensity short vector of computed intensity
	 *  @param [out] vallue of T which shows the quadratic distance between the
	 *  range and the computed intensity
	 */
	virtual T compare(const std::vector<Pixel<T>*> & pixelList,
			const std::vector<IncrementorParameter<T>*>& params);
};

#endif /* OBJECTIVEFUNKTIONCOMPARING_H_ */
