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
/// @file Sobel.cpp
/// implements class Sobel
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "Sobel.h"

using namespace std;
using namespace cimg_library;

using namespace Derivatives;

Sobel::Sobel(const string& name) : Derivative("derivatives_sobel",name), filterx(3,3,1,1,\
						 3.0/32,	0.0,	 -3.0/32, \
						10.0/32,	0.0,	-10.0/32, \
						 3.0/32,	0.0,	 -3.0/32)
{
	filtery = filterx.get_transpose();
	//filter.print("filter");
	_addOutputSlot(dx,"dx","derivative in x", "CImgList");
	_addOutputSlot(dy,"dy","derivative in y", "CImgList");
}

void Sobel::calculateDerivatives()
{

	if (dx.connected())
	{
		dx = CImgList<>(img().size);
		for (int t=0; t<img().size; t++)
			dx()[t]=img()[t].get_convolve(filterx);
		//dx().print("dx");
		
	}
	if (dy.connected())
	{
		dy = CImgList<>(img().size);
		for (int t=0; t<img().size; t++)
			dy()[t]=img()[t].get_convolve(filtery);
		//dy().print("dy");
	}
	
	cout << "jo" << endl;
	
	// Update of all objects at the outputsslots
	std::set<ParameteredObject*> res;
	//std::map<std::string, Slot*>::const_iterator slotIter;
	std::set<Slot*> targets;
	std::set<Slot*>::const_iterator targetIter;
	
	if (dx.connected())
	{
		targets = dx.getTargets();
		for(targetIter = targets.begin(); targetIter != targets.end(); targetIter++)
			res.insert(&((*targetIter)->getParent()));
	}
	if (dy.connected())
	{
		targets = dy.getTargets();
		for(targetIter = targets.begin(); targetIter != targets.end(); targetIter++)
			res.insert(&((*targetIter)->getParent()));
	}
	
	std::set<ParameteredObject*>::const_iterator objIter;
	for(objIter = res.begin(); objIter != res.end(); objIter++)
		(*(objIter))->update();
		
}
