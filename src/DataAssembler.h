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
/// @file DataAssembler.h
/// defines abstract class DataAssembler
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _DataAssembler_h_
#define _DataAssembler_h_

#include "main.h"
//#include "charon-utils/ParameteredObject.h"

/// abstract class for data assemblers
class DataAssembler  : public ParameteredObject
{
	// Auch zeitliche Nachbarschaft?
protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const = 0;
	
public:
	/// default constructor
	DataAssembler(const std::string& classname,const std::string& name = "") : 
		ParameteredObject(classname,name,"calculates the neighbourhood")
	{
		_addOutputSlot(out,"this","Pointer to itself","DataAssembler*");
		out = this;
	}
	
	/// output slot containing pointer to this class
	OutputSlot<DataAssembler*> out;
	
	/**
	 *        initializes the data assembler
	 * @param x x-coordinate of center
	 * @param y y-coordinate of center
	 * @param z z-coordinate of center
	 */
	virtual void init(int x, int y, int z) = 0;
	
	/// set the assembler to the next pixel
	virtual void next() = 0;
	
	/// check wether last pixel is reached
	virtual bool end() = 0;
	
	/// get x-coordinate of current pixel
	virtual int getX() = 0;
	
	/// get y-coordinate of current pixel
	virtual int getY() = 0;
	
	/// get z-coordinate of current pixel
	virtual int getZ() = 0;
	
	/// get channel of current pixel
	virtual int getV() = 0;
	
	/// returns the number of points of the data assembler
	virtual int count()= 0; // Anzahl der Pixel
};

#endif
