/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file BinOperator.hxx
 *  Implementation of the parameter class BinOperator.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 25.01.2011
 */

#ifndef _BIN_OPERATOR_HXX_
#define _BIN_OPERATOR_HXX_

#include "BinOperator.h"
#include <vigra/multi_pointoperators.hxx>
#include <vigra/functorexpression.hxx>

template <typename T>
BinOperator<T>::BinOperator(const std::string& name) :
	TemplatedParameteredObject<T>("binoperator", name,
		"simple class to apply binary operators<br><br>"
		"Apply simple binary operators pointwise to the given input data. "
		"Result is given as output. Shapes of the input has to be the same. "
		"Which operator is applied may be selected using the operation "
		"parameter.")
{
	ParameteredObject::_setTags("charon-utils;Vigra") ;
	
	ParameteredObject::_addParameter<std::string>(
			operation, "operation",
			"Operation selector ("
			"Possible values: add,sub,mul,or,and,less,eq)",
			"add", "{add;sub;mul;or;and;less;eq}");
	ParameteredObject::_addInputSlot (
			in1, "in1", "first data input", "vigraArray5<T>");
	ParameteredObject::_addInputSlot (
			in2, "in2", "second data input", "vigraArray5<T>");
	ParameteredObject::_addOutputSlot(
			out, "out", "data output", "vigraArray5<T>");
}

template <typename T>
void BinOperator<T>::execute() {
	const std::string& op = operation();
	const vigra::MultiArrayView<5,T>& i1 = in1();
	const vigra::MultiArrayView<5,T>& i2 = in2();
	vigra::MultiArray<5,T>& o = out();

	vigra_precondition(
			i1.shape() == i2.shape(),
			"shapes of i1 and i2 do not match");
	o.reshape(i1.shape());

	if(op == "add")
		vigra::combineTwoMultiArrays(
				vigra::srcMultiArrayRange(i1),
				vigra::srcMultiArrayRange(i2),
				vigra::destMultiArrayRange(o),
				vigra::functor::Arg1()+vigra::functor::Arg2()
		);
	else if (op == "sub")
		vigra::combineTwoMultiArrays(
				vigra::srcMultiArrayRange(i1),
				vigra::srcMultiArrayRange(i2),
				vigra::destMultiArrayRange(o),
				vigra::functor::Arg1()-vigra::functor::Arg2()
		);
	else if (op == "mul")
		vigra::combineTwoMultiArrays(
				vigra::srcMultiArrayRange(i1),
				vigra::srcMultiArrayRange(i2),
				vigra::destMultiArrayRange(o),
				vigra::functor::Arg1()*vigra::functor::Arg2()
		);
	else if (op == "or")
		vigra::combineTwoMultiArrays(
				vigra::srcMultiArrayRange(i1),
				vigra::srcMultiArrayRange(i2),
				vigra::destMultiArrayRange(o),
				vigra::functor::Arg1() || vigra::functor::Arg2()
		);
	else if (op == "and")
		vigra::combineTwoMultiArrays(
				vigra::srcMultiArrayRange(i1),
				vigra::srcMultiArrayRange(i2),
				vigra::destMultiArrayRange(o),
				vigra::functor::Arg1() && vigra::functor::Arg2()
		);
	else if (op == "less")
		vigra::combineTwoMultiArrays(
				vigra::srcMultiArrayRange(i1),
				vigra::srcMultiArrayRange(i2),
				vigra::destMultiArrayRange(o),
				vigra::functor::Arg1() < vigra::functor::Arg2()
		);
	else if (op == "eq")
		vigra::combineTwoMultiArrays(
				vigra::srcMultiArrayRange(i1),
				vigra::srcMultiArrayRange(i2),
				vigra::destMultiArrayRange(o),
				vigra::functor::Arg1() == vigra::functor::Arg2()
		);
	else
		vigra_fail("invalid operation value!");
}

#endif /* _BIN_OPERATOR_HXX_ */
