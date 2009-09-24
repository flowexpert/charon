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
 *  Channel Converter for CImg.
 *  Converts v dimension to t and vice versa.
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
	//the input CImgList has all the frames in different CImg objects in the list
	//what we want is a CImgList that splits the sequence by channel into seperate
	//CImg objects.
	out().assign(in()[0].dimv(),in()[0].dimx(),in()[0].dimy(),in()[0].dimz(),in().size);
	for(unsigned int t=0; t<in().size; t++) {
		for(int v=0; v<in()[t].dimv(); v++) {
			for(int z=0; z<in()[t].dimz(); z++) {
				for(int y=0; y<in()[t].dimy(); y++) {
					for(int x=0; x<in()[t].dimx(); x++) {
						out()(v,x,y,z,t) = in()(t,x,y,z,v);
					}
				}
			}
		}
	}
}

template <class T>
ChannelConverter<T>::~ChannelConverter() {}

#endif