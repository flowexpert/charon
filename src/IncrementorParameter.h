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
 *  @file IncrementorParameter.h
 *  @brief declaration of class IncrementorParameter
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 31.08.2009
 */
#ifndef INCREMENTORPARAMETER_H_
#define INCREMENTORPARAMETER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef incrementorparameter_EXPORTS
///Visual C++ specific code
#define incrementorparameter_DECLDIR __declspec(dllexport)
#else
#define incrementorparameter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define incrementorparameter_DECLDIR
#endif

#include <ParameteredObject.h>

/// IncrementorParameter saves one parameter to work in Incrementor or 
/// Brightnessmodels and MotionModels
template<typename T>
class incrementorparameter_DECLDIR IncrementorParameter: public TemplatedParameteredObject<
		T>
{
public: //also public to avoid getters and setters due to template-dll-problem
	std::string name;/// @param parameterName name of parameter
	Parameter<T> min; /// @param min minimum of parameter
	Parameter<T> max; /// @param max maximum of parameter
	Parameter<T> stepSize; /// @param step stepsize of parameter
	Parameter<T> current; /// @param current current pos of parameter

public:
	/// stadard constructor
	IncrementorParameter(const std::string& name = "");
	/// this outputslot
	OutputSlot<IncrementorParameter<T>*> out;
};

#endif /* INCREMENTORPARAMETER_H_ */
