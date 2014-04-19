/*  Copyright (C) 2012 Gerald Mwangi

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file WhileGroupTestReader.cpp
 *  Implementation of parameter class WhileGroupTestReader.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 27.11.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "WhileGroupTestReader.h"

WhileGroupTestReader::WhileGroupTestReader(const std::string& name) :
		ParameteredObject(
			"WhileGroupTestReader", name,
			"<h2>Test module input module for whilegroup</h2><br>"
			"Test module input module for whilegroup"
		)
{

	ParameteredObject::_addInputSlot(
		in1, "in1",
		"in1",
		"int");
	ParameteredObject::_addInputSlot(
		in2, "in2",
		"in2",
		"float");

	ParameteredObject::_addParameter<bool>(
		loopmode, "loopmode",
		"loopmode",false,
		"bool");
	ParameteredObject::_addParameter<int>(
		maxIt, "maxIt",
		"maxIt",11,
		"int");
	_iter=0;

}

void WhileGroupTestReader::execute() {
	if(getName()=="ExternReader"||getName()=="ExternReaderLoop")
	{
		if(in1!=20||in2!=0.33f)
		{
		std::stringstream msg;
		msg<<"Incorrect Values in "<<getName()<<" in1 "<<in1()<<" in2 "<<in2()<<" iter "<<_iter;
		raise(msg.str());
		}
	}
	else if(loopmode)
	{

		//0th iter values of extoutgen, else   (intreader,intreader2).in1=intoutgen.out1 and (intreader,intreader2).in2=extoutgen.out2
		if(((_iter>0)&&(in1!=20||in2!=5.5f))||(_iter==0&&(in1!=10||in2!=5.5f)))
		{
			std::stringstream msg;
			msg<<"Incorrect Values in Loopmode! in1 "<<in1()<<" in2 "<<in2()<<" iter "<<_iter;
			raise(msg.str());
		}
	}
	else
	{
		if(in1!=10||in2!=5.5f)
		{
			std::stringstream msg;
			msg<<"Incorrect Values in non loopmode! in1 "<<in1()<<" in2 "<<in2()<<" iter "<<_iter;
			raise(msg.str());
		}
	}
	//Did we shoot over the maximum?
	if(_iter>maxIt)
	raise("Too many iterations!");
	_iter++;
}

// the following functions are needed
// for class WhileGroupTestReader to work as a charon plugin.
extern "C" whilegrouptestreader_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new WhileGroupTestReader(name);
}

extern "C" whilegrouptestreader_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" whilegrouptestreader_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
