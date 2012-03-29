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
/** \file CliqueWeightUniform.hxx
 *  Implementation of the parameter class CliqueWeightUniform.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 29.03.2012
 */

#ifndef _CLIQUEWEIGHTUNIFORM_HXX_
#define _CLIQUEWEIGHTUNIFORM_HXX_

#include "CliqueWeightUniform.h"

#include <charon/CliqueWeight.hxx>

template <class T>
CliqueWeightUniform<T>::CliqueWeightUniform(const std::string& name) :
	CliqueWeight<T>(
		"CliqueWeightUniform", name,
		"<h2>Implementation of the uniform clique weight."
	)
{
}

template <class T>
void CliqueWeightUniform<T>::execute() {
	CliqueWeight<T>::execute();
}

template <class T>
T CliqueWeightUniform<T>::getCliqueWeight( int, int, int, int, int,
                                           int, int, int, int, int )
{
	return T(1);
}

#endif /* _CLIQUEWEIGHTUNIFORM_HXX_ */

