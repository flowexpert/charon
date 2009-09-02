/*  Copyright (C) 2009 René Steinbrügge

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
/// @file nrange.cpp
/// implements class nRange
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "../main.h"
#include "nRange.h"

using namespace std;
using namespace cimg_library;

DataAssemblers::nRange::nRange(const std::string& name, const int _sizex, const int _sizey, const int _sizez,
			       const int _channels) : DataAssembler("dataassemblers_nrange",name)
{
	_addParameter(sizex,"sizex","size of the Neighbourhood in x-Direction",5,"int");
	_addParameter(sizey,"sizey","size of the Neighbourhood in y-Direction",5,"int");
	_addParameter(sizez,"sizez","size of the Neighbourhood in z-Direction",5,"int");
	_addParameter(channels,"channels","channels of the image",1,"int");
	
	sizex = _sizex;
	sizey = _sizey;
	sizez = _sizez;
	channels = _channels;
}

void DataAssemblers::nRange::init(int _x, int _y, int _z)
{
	i = 0;
	x =_x;
	y =_y;
	z =_z;
}

void DataAssemblers::nRange::next()
{
	i++;
}

bool DataAssemblers::nRange::end()
{
	return (i >= this->count());
}

int DataAssemblers::nRange::getX()
{
	return ((i % (sizex*sizey)) % sizex) - (sizex/2) + x;
}

int DataAssemblers::nRange::getY()
{
	return ((i % (sizex*sizey)) / sizex) - (sizey/2) + y;
}

int DataAssemblers::nRange::getZ()
{
	return ((i / (sizex*sizey)) % sizez) - (sizez/2) + z;
}

int DataAssemblers::nRange::getV()
{
	return ((i / (sizex*sizey)) / sizez);
}

int DataAssemblers::nRange::count()
{
	return sizex*sizey*sizez*channels;
}
