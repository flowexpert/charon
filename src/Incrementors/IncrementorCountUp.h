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
 *  @file IncrementorCountUp.h
 *  @brief declaration of class IncrementorCountUp, child class of Incrementor
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef INCREMENTORCOUNTUP_H_
#define INCREMENTORCOUNTUP_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef incrementorcountup_EXPORTS
///Visual C++ specific code
#define incrementorcountup_DECLDIR __declspec(dllexport)
#else
#define incrementorcountup_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define incrementorcountup_DECLDIR
#endif

#include <Incrementor.h>

template<typename T>
class incrementorcountup_DECLDIR IncrementorCountUp: public Incrementor<T>
{
public:
	/// standart constructor
	IncrementorCountUp(const std::string& name);

	virtual void execute();

	/**
	 *  computes the next Step
	 *  - compute return vallue true for no doStep possible, false doStep is 
	 *    possible
	 *  - compute step for all parameters
	 */
	virtual bool doStep();
};

#endif /* INCREMENTORCOUNTUP_H_ */
