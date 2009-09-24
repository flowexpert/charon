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
/// @file nRange.h
/// defines class DataAssemblers::nRange
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _DataAssemblers_nRange_h
#define _DataAssemblers_nRange_h

#include "../DataAssembler.h"

/// namespace for data assemblers
namespace DataAssemblers
{


/// data assembler for a cuboidal neighborhood
class nRange : public DataAssembler
{
protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const
		{return new nRange(name);}	
	
private:
	int i,x,y,z;

public:
	//size=Breite des Quadrats
	/**
	 *        default constructor
	 */
	nRange(const std::string& name = "", const int sizex=5, const int sizey=5, const int sizez=5, const int channels=1);
	
	/// @name sizes
	/// @brief initial point is always in the center
	//@{
	/// x-direction
	Parameter<int> sizex;
	/// y-direction
	Parameter<int>sizey;
	/// z-direction
	Parameter<int> sizez;
	//@}
	
	/// number of channels
	Parameter<int> channels;
	
	void init(int x, int y, int z);
	void next();
	bool end();
	int getX();
	int getY();
	int getZ();
	int getV();
	int count();
};

}

#endif
