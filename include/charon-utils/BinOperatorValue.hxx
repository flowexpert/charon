/*  Copyright (C) 2012 Gerald Mwangi

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
/** \file BinOperatorValue.hxx
 *  Implementation of the parameter class BinOperatorValue.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 27.03.2012
 */

#ifndef _BINOPERATORVALUE_HXX_
#define _BINOPERATORVALUE_HXX_


#include "BinOperatorValue.h"

template <typename T>
BinOperatorValue<T>::BinOperatorValue(const std::string& name) :
		TemplatedParameteredObject<T>(
			"BinOperatorValue", name,
			"<h2>Binary operators for numbers</h2><br>"
			"Binary operators for numbers"
		)
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(
		value1, "value1",
		"value1",
		"T");
	ParameteredObject::_addInputSlot(
		value2, "value2",
		"value2",
		"T");

	ParameteredObject::_addOutputSlot(
		res, "result",
		"result",
		"T");
	ParameteredObject::_addParameter<std::string>(
			operation, "operation",
			"Operation selector ("
			"Possible values: add,sub,mul,div,or,and,more,less,eq)",
            "add", "{add;sub;mul;div;or;and;more;less;eq;mod}");

//	ParameteredObject::_addParameter(
//		operation, "operation",
//		"Operation selector (",
//		    "Possible values: add,sub,mul,div,or,and,more,less,eq)",
//		    "add", "{add;sub;mul;div;or;and;more;less;eq}");



}

template <typename T>
void BinOperatorValue<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();


	std::string op=operation();
	if(op=="add")
	    res=value1+value2;
	else if(op=="sub")
	    res=value1-value2;
	else if(op=="mul")
	    res=value1*value2;
	else if(op=="div")
	    res=value1/value2;
	else if(op=="or")
	    res=value1||value2;
	else if(op=="and")
	    res=value1&&value2;
	else if(op=="more")
	    res=value1>value2;
	else if(op=="less")
	    res=value1<value2;
	else if(op=="eq")
	    res=value1==value2;
    else if(op=="mod")
        res=((int)value1())%((int)value2());

}

#endif /* _BINOPERATORVALUE_HXX_ */
