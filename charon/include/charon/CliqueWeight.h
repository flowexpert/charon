/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

    This file is part of Charon.

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
/** @file CliqueWeight.h
 *  Declaration of the parameter class CliqueWeight.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *
 *  @date 29.03.2012
 */

#ifndef _CLIQUEWEIGHT_H_
#define _CLIQUEWEIGHT_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef cliqueweight_EXPORTS
///Visual C++ specific code
#define cliqueweight_DECLDIR __declspec(dllexport)
#else
#define cliqueweight_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define cliqueweight_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Base class for CliqueWeights
/** This calculates weights for cliques.
 */
template <class T>
class cliqueweight_DECLDIR CliqueWeight : public TemplatedParameteredObject<T> {
public:
	/// Output slot containing the this-pointer of the object
	OutputSlot<CliqueWeight<T>*> out;

	/// default constructor
	CliqueWeight(const std::string& classname /**[in] class name*/,
		const std::string& name /**[in] instance name*/,
		const std::string& doc /**[in] stencil documentation*/);

	/// function yielding clique weight
	virtual T getCliqueWeight( int nn, int xx, int yy, int zz, int cc,
	                           int n,  int x,  int y,  int z,  int c ) = 0;

protected:
	/// \implements ParameteredObject::execute()
	virtual void execute();
};

#endif

