/*  Copyright (C) 2013 Gerald Mwangi

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file TVFilterVigra.h
 *  Declaration of the parameter class TVFilterVigra.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 12.06.2013
 */

#ifndef _TVFILTERVIGRA_H_
#define _TVFILTERVIGRA_H_

#ifdef _MSC_VER
#ifdef tvfiltervigra_EXPORTS
/// Visual C++ specific code
#define tvfiltervigra_DECLDIR __declspec(dllexport)
#else
#define tvfiltervigra_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define tvfiltervigra_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>

/// Wrapper for the total variation filter in the VIGRA-Suite
/** Wrapper for the total variation filter in the VIGRA-Suite
 */
template <typename T>
class tvfiltervigra_DECLDIR TVFilterVigra :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	TVFilterVigra(const std::string& name = "");

	/// input image
	InputSlot< vigra::MultiArray<5,T> > in;

	/// output image
	OutputSlot< vigra::MultiArray<5,T> > out;

	/// TV smoothing parameter
	Parameter< double > alpha;
	/// maximal number of iteration steps
	Parameter< int > steps;
	/// The algorithm stops, if the primal-dual gap is below the threshold eps
	Parameter< double > eps;


protected:
	/// Update object.
	virtual void execute();
};

#endif // _TVFILTERVIGRA_H_
