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
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(_in1, "in1", "input image 1", "CImgList<T>"); 
	ParameteredObject::_addInputSlot(_in2, "in2", 
		"input image 2<br>"
		"must be of the same dimension as in1", "CImgList<T>"); 
	ParameteredObject::_addParameter(_opType, "opType",
		"operation type<br>"
		"Available operations are:<ul>"
		"<li>Passthrough (out is in1)</li>"
		"<li>Addition</li>"
		"<li>Difference</li>"
		"<li>Multiplication</li>"
		"<li>Division</li>"
		"<li>Power of</li>"
		"<li>Bitwise And</li>"
		"<li>Bitwise Or</li>"
		"<li>Bitwise Xor</li>"
		"<li>Bitwise Not (in2 or withValue is ignored)</li>"
		"<li>Left Bit Shift</li>"
		"<li>Right Bit Shift</li>"
		"<li>Mask (Input is set to zero where mask(in2) is zero "
		"and kept otherwise, acts as passtrough when withValue is true)</li>"
		"<li>Natural logarithm (in2 or withValue is ignored)</li>"
		"<li>Exponential (in2 or withValue is ignored)</li>"
		"<li>Sine </li>"
		"<li>Cosine </li>"
		"<li>Tangent </li>"
		"<li>Arc Sine </li>"
		"<li>Arc Cosine </li>"
		"<li>Arc Tangent </li>"
		"<li>Arc Tangent2 (2 argument function)</li>"
		"<li>Flow Magnitude</li>"
		"<li>Limit Magnitude (set vector value to m*(v/|v|) "
		"if magnitude is larger than m)</li>"
		"<li>toGray (use only the first spectrum(c) channel of each CImg)</li>"
		"<li>toRGB (duplicate first spectrum(c) channel twice) "
		"This does not perform a correct RGB->grayscale conversion, "
		"you will get only the R channel toRGB.<br>"
		"Usefull for operations which need RGB images as input.<br>"
		"Keep in mind that charon uses a CImgList of width 3 for color images, "
		"not a single CImg with the spectrum(c) dimension set to 3 "
		"like this operation does.<br>"
		"Use the channelconverter plugin to change the representation</li>"
		"</ul>",
		"{Passthrough;Addition;Difference;Multiplication;Division;Power of;"
		"Absolute;Bitwise And;Bitwise Or;Bitwise Xor;Bitwise Not;"
		"Left Shift;Right Shift;Mask;Log;Exp;Sin;Cos;Tan;Asin;Acos;Atan;Atan2;"
		"Flow Magnitude;Limit Magnitude;to Gray;to RGB;Normal Distribution}") ;

	ParameteredObject::_addParameter(
			_withValue, "withValue",
			"Set second operand to a single value<br>"
			"(assumed true if no second input image is provided)", false) ;
	ParameteredObject::_addParameter(_value, "value",
				"Second operand if withValue is true", T(0) ) ;
	ParameteredObject::_addOutputSlot(_out, "out",
				"output image (will have the same dimension as in1)", "CImgList<T>");
}

template <typename T>
void BinOperatorCImg<T>::execute() {
	const CImgList<T>& in1 = _in1() ;

	CImgList<T> dummy ;
	const CImgList<T>& in2 = _in2.connected() ? _in2() : dummy ;

	CImgList<T>& out = _out() ;

	const std::string& op = _opType() ;

	const T& val = _value() ;

	bool withValue = (!_in2.connected() ||_withValue()) ;

	T diff = T(0);
	if (op == "Normal Distribution") {
		out.assign(1u, in1[0].width(), in1[0].height(), in1[0].depth(), in1[0].spectrum(), T(0));
		cimg_forXYZC( out[0], x, y, z, c )
		{
			diff = in1.atNXYZC( 0, x, y, z, c ) - in2.atNXYZC( 0, x, y, z, c ) ;
			out.atNXYZC( 0, x, y, z, c ) = exp(double(-diff*diff/(2*val*val))) ;
		}
	}
	else if (op == "Passthrough") {
		out.assign(in1);
	}
	else if(op == "Flow Magnitude") {
		if (in1.size() < 2) {
			ParameteredObject::raise("in1 is no flow (less than 2 elements)");
		}
		out.assign(1u, in1[0].width(),in1[0].height(),
			in1[0].depth(),in1[0].spectrum(),T(0));
		cimglist_for(in1,kk) {
			out[0] += in1[kk].get_sqr();
		}
		out[0].sqrt();
	}
	else if(op == "Limit Magnitude") {
		out.assign(in1);
		const T& v = _value();
		if (v <= 0) {
			ParameteredObject::raise("magnitude limit has to be positive!");
		}
		// calculate current flow magnitudes
		cimg_library::CImg<T> mag(in1[0].width(),in1[0].height(),
			in1[0].depth(),in1[0].spectrum(),T(0));
		cimglist_for(in1,kk) {
			mag += in1[kk].get_sqr();
		}
		mag.sqrt();
		// apply limit, if magnitude too large
		// (set to max allowed magnitude, keep direction)
		unsigned int num = 0u;
		cimg_forXYZC(mag, xx,yy,zz,tt) {
			const T& cur = mag(xx,yy,zz,tt);
			if(cur > v) {
				num++;
				cimglist_for(out,kk) {
					T& o = out(kk,xx,yy,zz,tt);
					o *= v;
					o /= cur;
				}
			}
		}
		if (num > 0) {
			sout << "(II) \tlimit magnitude applied to "
				<< num << " pixels (" << (100.f*num/mag.size())
				<< "%)." << std::endl;
		}
	}
	else {
		out.assign(in1.size()) ;
		cimglist_for(in1, l) {
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
			else if(op == "Absolute")
				out(l) = first.get_abs() ;
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
			else if(op == "Mask") {
				if(withValue) {
					out(l) = first ;
				}
				else {
					out(l).assign(first) ;
					cimg_forXYZC(first,x,y,z,c) {
						out(l,x,y,z,c) =
							(second(x,y,z,c)==T(0.0) ? T(0) : first(x,y,z,c));
					}
				}
			}
			else if(op == "to Gray")
				out(l) = first.get_channel(0) ;
			else if(op == "to RGB") {
				out(l).assign(first.width(),first.height(),first.depth(),3) ;
				out(l).get_shared_channel(0) =
					out(l).get_shared_channel(1) =
					out(l).get_shared_channel(2) = first.get_channel(0) ;
			}
			else if(op == "Exp")
				out(l) = first.get_exp() ;
			else if(op == "Log")
				out(l) = first.get_log() ;
			else if(op == "Sin")
				out(l) = first.get_sin() ;
			else if(op == "Cos")
				out(l) = first.get_cos() ;
			else if(op == "Tan")
				out(l) = first.get_tan() ;
			else if(op == "Asin")
				out(l) = first.get_asin() ;
			else if(op == "Acos")
				out(l) = first.get_acos() ;
			else if(op == "Atan")
				out(l) = first.get_atan() ;
			else if(op == "Atan2")
				out(l) = first.get_atan2(second) ;
			else {
				ParameteredObject::raise("Unknown Operation: " + op);
			}
		}
	}
}

#endif /* _BINOPERATORCIMG_HXX_ */

