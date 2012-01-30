/*  Copyright (C) 2010 Stephan Meister

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
/** \file BinOperatorCImg.hxx
 *  Implementation of the parameter class BinOperatorCImg.
 *  \author Stephan Meister
 *  \date 15.01.2012
 */

#ifndef _BINOPERATORCIMG_HXX_
#define _BINOPERATORCIMG_HXX_

#include "BinOperatorCImg.h"
#include <sstream>

using namespace cimg_library ;

template <typename T>
BinOperatorCImg<T>::BinOperatorCImg(const std::string& name) :
		TemplatedParameteredObject<T>("BinOperatorCImg", name, 
			"Perform binary operations on input image<br>"
			"If a operation takes two operands you can either input a single value "
			"or provide them via the second input"
			"<br>available operands are described in the opType parameter"
		),
		_in2(true, false)
{
	ParameteredObject::_addInputSlot(_in1, "in1", "input image 1", "CImgList<T>"); 
	ParameteredObject::_addInputSlot(_in2, "in2", "input image 2", "CImgList<T>"); 
	ParameteredObject::_addParameter(_opType, "opType", "Available operations are:<br>"
		"Passthrough (out is in1)<br>Addition<br>Difference<br>Multiplication<br>Division<br>Power of<br>Bitwise And"
		"<br>Bitwise Or<br>Bitwise Xor<br>Bitwise Not (in2 or withValue is ignored)"
		"<br>Left Bit Shift<br>Right Bit Shift"
		"<br>Mask (Input is set to zero where mask(in2) is zero and kept otherwise,"
		"<br>\tacts as passtrough when withValue is true)"
		"<br>toGray(use only the first spectrum(c) channel of each CImg)"
		"<br>\tThis does not perform a correct RGB->grayscale conversion, you will get only the R channel"
		"<br>toRGB (duplicate first spectrum(c) channel twice)<br>"
		"\tusefull for operations which need RGB images as input.<br>"
		"\tKeep in mind that charon uses a CImgList of width 3 for color images,"
		" not a single CImg with the spectrum(c) dimension set to 3 like this operation does.<br>"
		"\tUse the channelconverter plugin to change the representation",
	"{Passthrough;Addition;Difference;Multiplication;Division;Power of;Bitwise And;Bitwise Or;Bitwise Xor;Bitwise Not;Left Shift;Right Shift;Mask;Log;Exp;to Gray;to RGB}") ;

	ParameteredObject::_addParameter(_withValue, "withValue", 
		"Set second operand to a single value<br>"
		"(assumed true if no second input image is provided)", false) ;
	ParameteredObject::_addParameter(_value, "value", "Second operand if withValue is true", T(0) ) ;

	ParameteredObject::_addOutputSlot(_out, "out", "output image", "CImgList<T>"); 
}

template <typename T>
void BinOperatorCImg<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const CImgList<T>& in1 = _in1() ;

	CImgList<T> dummy ;
	const CImgList<T>& in2 = _in2.connected() ? _in2() : dummy ;

	_out().assign(in1.size()) ;
	CImgList<T>& out = _out() ;

	const std::string& op = _opType() ;

	const T& val = _value() ;

	bool withValue = (!_in2.connected() ||_withValue()) ;
	
	cimglist_for(in1, l)
	{
		const CImg<T>& first = in1(l) ;
		const CImg<T>& second = in2(l) ;

		if(op == "Addition")
			out(l) = (withValue ? first + val : first + second) ;
		else if(op == "Difference")
				out(l) = (withValue ? first - val : first - second) ;
		else if(op == "Multiplication")
				out(l) = (withValue ? first * val : first.get_mul(second)) ;
		else if(op == "Division")
				out(l) = (withValue ? first / val : first.get_div(second)) ;
		else if(op == "Power of")		
			out(l) = (withValue ? first.get_pow(val) : first.get_pow(second)) ;
		else if(op == "Bitwise And")
			out(l) = (withValue ? first & val : first & second) ;
		else if(op == "Bitwise Or")
				out(l) = (withValue ? first | val : first | second) ;
		else if(op == "Bitwise Xor")
			out(l) = (withValue ? first  ^ val : first ^ second) ;
		else if(op == "Bitwise Not")
				out(l) = ~first ;
		else if(op == "Left Shift")
			out(l) = (withValue ? first  << val : first << second) ;
		else if(op == "Right Shift")
			out(l) = (withValue ? first  >> val : first >> second) ;
		else if(op == "Mask")
		{
			if(withValue)
					out(l) = first ;
			else
			{	
				out(l).assign(first) ;
				cimg_forXYZC(first,x,y,z,c)
				{	out(l,x,y,z,c) = (second(x,y,z,c) == T(0.0) ? T(0) : first(x,y,z,c)) ;	}
			}
		}
		else if(op == "Passthrough")
			out(l) = first ;
		else if(op == "to Gray")
			out(l) = first.get_channel(0) ;
		else if(op == "to RGB")
		{
			out(l).assign(first.width(),first.height(),first.depth(),3) ;
			out(l).get_shared_channel(0) = out(l).get_shared_channel(1) = out(l).get_shared_channel(2) = first.get_channel(0) ;
		}
		else if(op == "Exp")
			out(l) = first.get_exp() ;
		else if(op == "Log")
			out(l) = first.get_log() ;
		else
		{
			sout << op << " : Unknown Operation! Defaulting to Passthrough" << std::endl ;
			out(l) = first ;
		} ;
	}
}

#endif /* _BINOPERATORCIMG_HXX_ */

