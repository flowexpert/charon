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
/** @file ChannelConverter.hxx
 *  Implementation of template class ChannelConverter.
 *  
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */
#ifndef _CHANNELCONVERTER_HXX_
#define _CHANNELCONVERTER_HXX_

#include "ChannelConverter.h"
#include <ParameteredObject.hxx>

template <class T>
ChannelConverter<T>::ChannelConverter(const std::string& name) : 
	TemplatedParameteredObject<T>("ChannelConverter",name,
		"Channel Converter for CImgLists<br><br>"
		"Permutes image dimensions.<br>"
		"Select permutation using the parameter <i>scheme</i>.<br>"
		"Per default converts v dimension to t and vice versa.")
{
	ParameteredObject::_addInputSlot(in,"input",
		"sequence to convert","CImgList<T>");
	ParameteredObject::_addOutputSlot(out,"output",
		"converted sequence","CImgList<T>");
	ParameteredObject::_addParameter<std::string>(scheme, "scheme",
		"Conversion scheme.<br><br>"
		"<i>xyzvt</i> or <i>01234</i> leaves dimensions unpermuted.<br>"
		"Exchange dimensions as needed, e.g. the default <i>xyztv</i> "
		"will permute the 4th CImg dimension (v) and the list element "
		"dimension (t).<br>"
		"Having a flow field with time in the z-dimension and flow "
		"components in the v-dimension, you have to select <i>xytzv</i> "
		"or <i>01423</i>.<br>"
		"This does also work to convert 2D image sequences having their "
		"time axis in the z-dimension "
		"(monochrome or color channels in v-dim).",
		"xyztv", "string");
}

template <typename T>
inline unsigned int ChannelConverter<T>::_select(
	unsigned int x, unsigned int y, unsigned int z,
	unsigned int v, unsigned int t, char sel) const
{
	switch (sel) {
	case 'x':
	case 'X':
	case '0':
		return x;
		break;
	case 'y':
	case 'Y':
	case '1':
		return y;
		break;
	case 'z':
	case 'Z':
	case '2':
		return z;
		break;
	case 'v':
	case 'V':
	case '3':
		return v;
		break;
	case 't':
	case 'T':
	case '4':
		return t;
		break;
	default:
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "invalid dimension given: " << sel;
		throw std::runtime_error(msg.str().c_str());
	}
}

template <class T>
void ChannelConverter<T>::execute() {
	// check permutation scheme
	const std::string& scheme_ = scheme() ;
	
	if(scheme_.size() != 5) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "lenght of scheme string invalid ";
		msg << "(length=" << scheme().size() << ")\n\t";
		msg << "you have to specify 5 dimensions!";
	}
	std::string missing;
	if(StringTool::toLowerCase(scheme_).find('x') == std::string::npos &&
		scheme_.find('0') == std::string::npos)
		missing.push_back('x');
	if(StringTool::toLowerCase(scheme_).find('y') == std::string::npos &&
		scheme_.find('1') == std::string::npos)
		missing.push_back('y');
	if(StringTool::toLowerCase(scheme_).find('z') == std::string::npos &&
		scheme_.find('2') == std::string::npos)
		missing.push_back('z');
	if(StringTool::toLowerCase(scheme_).find('t') == std::string::npos &&
		scheme_.find('3') == std::string::npos)
		missing.push_back('t');
	if(StringTool::toLowerCase(scheme_).find('v') == std::string::npos &&
		scheme_.find('4') == std::string::npos)
		missing.push_back('v');
	if(missing.size()) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "Some dimensions are missing!\n\t";
		msg << "missing: " << missing;
	}

	cimg_library::CImgList<T>& out_ = out() ;
	const cimg_library::CImgList<T>& in_ = in() ;

	out_.assign(
		_select(in_[0].width(),in_[0].height(),
				in_[0].depth(),in_[0].spectrum(),in_.size(), scheme_[4]),
		_select(in_[0].width(),in_[0].height(),
				in_[0].depth(),in_[0].spectrum(),in_.size(), scheme_[0]),
		_select(in_[0].width(),in_[0].height(),
				in_[0].depth(),in_[0].spectrum(),in_.size(), scheme_[1]),
		_select(in_[0].width(),in_[0].height(),
				in_[0].depth(),in_[0].spectrum(),in_.size(), scheme_[2]),
		_select(in_[0].width(),in_[0].height(),
				in_[0].depth(),in_[0].spectrum(),in_.size(), scheme_[3])
	);

	
	cimglist_for(in_, t) 
		cimg_forXYZC(in_[t],x,y,z,v)
			out_(
				_select(x,y,z,v,t,scheme_[4]),
				_select(x,y,z,v,t,scheme_[0]),
				_select(x,y,z,v,t,scheme_[1]),
				_select(x,y,z,v,t,scheme_[2]),
				_select(x,y,z,v,t,scheme_[3])) = in_(t,x,y,z,v);
}

#endif
