/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file BinOperator.h
 *  Declaration of the parameter class BinOperator.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 25.01.2011
 */

#ifndef BIN_OPERATOR_H
#define BIN_OPERATOR_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef binoperator_EXPORTS
///Visual C++ specific code
#define binoperator_DECLDIR __declspec(dllexport)
#else
#define binoperator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define binoperator_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <vigra/multi_array.hxx>

/// simple class to apply binary operators
/** Apply simple binary operators pointwise to the given input data.
 *  Result is given as output. Shapes of the input has to be the same.
 *  Which operator is applied may be selected using the operation parameter.
 */
template <typename T>
class binoperator_DECLDIR BinOperator : public TemplatedParameteredObject<T>
{
public:
	/// first data input
	InputSlot< vigra::MultiArrayView<5,T> >  in1;
	/// second data input
	InputSlot< vigra::MultiArrayView<5,T> >  in2;
	/// data output
	OutputSlot< vigra::MultiArray<5,T> > out;

	/// Operation selector
	/** Possible values:
	 *  - add: \f$o=i_1+i_2\f$
	 *  - sub: \f$o=i_1-i_2\f$
	 *  - mul: \f$o=i_1*i_2\f$
	 *  - or:  \f$o=i_1\vee i_2\f$
	 *  - and: \f$o=i_1\wedge i_2\f$
	 *  - less: \f$o=i_1 < i_2\f$
	 *  - eq: \f$o=i_1 < i_2\f$
	 */
	Parameter<std::string> operation;

	/// create a new BinOperator object
	/// @param name             Object name
	BinOperator(const std::string& name = "");

	/// \implements ParameteredObject::execute
	virtual void execute ();
};

#endif // BIN_OPERATOR_H
