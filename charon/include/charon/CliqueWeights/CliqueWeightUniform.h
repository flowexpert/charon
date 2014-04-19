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
/** \file CliqueWeightUniform.h
 *  Declaration of the parameter class CliqueWeightUniform.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 29.03.2012
 */

#ifndef _CLIQUEWEIGHTUNIFORM_H_
#define _CLIQUEWEIGHTUNIFORM_H_

#ifdef _MSC_VER
#ifdef cliqueweightuniform_EXPORTS
/// Visual C++ specific code
#define cliqueweightuniform_DECLDIR __declspec(dllexport)
#else
#define cliqueweightuniform_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define cliqueweightuniform_DECLDIR
#endif

#include <charon/CliqueWeight.h>

/// Uniform clique weighting
/** Uniform clique weighting
 *
 *  e.g. for various weighted median filters.
 */
template <typename T>
class cliqueweightuniform_DECLDIR CliqueWeightUniform :
	public CliqueWeight<T> {
public:
	/// default constructor
	/// \param name          Instance name
	CliqueWeightUniform(const std::string& name = "");

	T getCliqueWeight( int nn, int xx, int yy, int zz, int cc,
	                   int n,  int x,  int y,  int z,  int c );
protected:
	/// main function
	virtual void execute();
};

#endif // _CLIQUEWEIGHTUNIFORM_H_

