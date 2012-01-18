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
/** @file Rfmc.hxx
 *  Implementation of class Rfmc.
 *  @author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  @date 28.10.2010
 */

#ifndef _RFMC_HXX_
#define _RFMC_HXX_

#include "../Stencil.hxx"
#include "Rfmc.h"

template <class T>
Rfmc<T>::Rfmc(const std::string& name) :
	Stencil<T>(
		"Rfmc", name,
		"Stencil modeling the range flow motion constraint equation<br>"
	),
	mask(true,false) // optional
{
	ParameteredObject::_addParameter(
			channelSelect, "channelSelect",
			"select which channel represents depth data<br>"
			"(useful if using combined greyvalue and depth images)",
			"uint");
	ParameteredObject::_addInputSlot(
			zx, "zx", "derivative of the depth data wrt x", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			zy, "zy", "derivative of the depth data wrt y", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			zt, "zt", "derivative of the depth data wrt t", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			mask, "mask",
			"Mask to be able to omit some parts of the image",
			"CImgList<T>");
}

template <class T>
void Rfmc<T>::execute() {
	Stencil<T>::execute();

	// set up unknowns: 3 components of the range flow vector
	this->_unknowns.clear();
	this->_unknowns.insert("a1");
	this->_unknowns.insert("a2");
	this->_unknowns.insert("a3");

	// expand stencil-map to appropriate size and fill with dummy values
	std::set<std::string>::iterator uIt;
	for(uIt=this->_unknowns.begin();uIt!=this->_unknowns.end();uIt++) {
		Point4D<int> center;
		SubStencil<T> entry(1,1,1,1,center);
		entry.pattern(0,0) = 1;
		this->_subStencils[*uIt] = entry;
	}
}

template <class T>
void Rfmc<T>::updateStencil(
		const std::string& unknown,
		const Point4D<int>& p,
		const int&) {
	this->_rhs = 0;
	const T& l = this->lambda();
	assert(p.z==0); // 2D only

	if(!mask.connected() || mask().atNXYZC(channelSelect,p.x,p.y,p.z,p.t)>0) {
		// query derivative values at the given coordinates
		const T& zX = zx().atNXYZC(channelSelect,p.x,p.y,p.z,p.t);
		const T& zY = zy().atNXYZC(channelSelect,p.x,p.y,p.z,p.t);
		const T& zT = zt().atNXYZC(channelSelect,p.x,p.y,p.z,p.t);

		// term values
		T tu, tv, tw, r;

		// compute term regarding the given unknown
		if (unknown == "a1") {
			tu =  zX * zX;
			tv =  zX * zY;
			tw =  zX;
			r  = -zX * zT;
		} else if (unknown == "a2") {
			tu =  zY * zX;
			tv =  zY * zY;
			tw =  zY;
			r  = -zY * zT;
		} else if (unknown == "a3") {
			tu =      zX;
			tv =      zY;
			tw = T(1);
			r  =     -zT;
		} else {
			std::ostringstream msg;
			msg << __FILE__ << ":" << __LINE__ << "\n\t";
			msg << "Invalid unknown given: " << unknown;
			throw std::runtime_error(msg.str());
		}
		this->_subStencils["a1"].data(0) = tu * l;
		this->_subStencils["a2"].data(0) = tv * l;
		this->_subStencils["a3"].data(0) = tw * l;
		this->_rhs = r * l;
	}
	else {
		this->_subStencils["a1"].data(0) = 0;
		this->_subStencils["a2"].data(0) = 0;
		this->_subStencils["a3"].data(0) = 0;
		this->_rhs = 0;
	}
}


//not yet implemented
template <class T>
cimg_library::CImg<T> Rfmc<T>::apply(
		const cimg_library::CImgList<T>& seq,
		const unsigned int frame) const {
	return seq[frame];
}

template <class T>
Rfmc<T>::~Rfmc() {
}

#endif //_RFMC_HXX_
