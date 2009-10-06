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
				"convert dim v <-> dim t") {
	this->_addInputSlot(in,"input","sequence to convert","CImgList<T>");
	this->_addOutputSlot(out,"output","converted sequence","CImgList<T>");
}

template <class T>
void ChannelConverter<T>::execute() {
	ParameteredObject::execute();
	out().assign(in()[0].dimv(),in()[0].dimx(),in()[0].dimy(),in()[0].dimz(),in().size);
	cimglist_for(in(), t) 
		cimg_forXYZV(in()[t], x,y,z,v)
			out()(v,x,y,z,t) = in()(t,x,y,z,v);
}

template <class T>
ChannelConverter<T>::~ChannelConverter() {}

#endif
