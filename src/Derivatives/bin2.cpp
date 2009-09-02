
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
/// @file bin2.cpp
/// implements class bin2
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "bin2.h"

using namespace std;
using namespace cimg_library;

using namespace Derivatives;

bin2::bin2(const string& name) : Derivative("derivatives_bin2",name)
{
	_addOutputSlot(dx,"dx","derivative in x", "CImgList");
	_addOutputSlot(dy,"dy","derivative in y", "CImgList");
	_addOutputSlot(dz,"dz","derivative in z", "CImgList");
	_addOutputSlot(dt,"dt","derivative in t", "CImgList");
}

void bin2::calculateDerivatives()
{
	
	
	//check for 3d
	if (img().is_sameN(1)) // 2D  TODO 2D algorithm same as 3D ?!
	{
		if (dx.connected())
		{
			CImg<> f(3,1,1,1,0.5,0.0,-0.5);
			dx = CImgList<>(img()[0].get_convolve(f));
		}
		if (dy.connected())
		{
			CImg<> f(1,3,1,1,0.5,0.0,-0.5);
			dy = CImgList<>(img()[0].get_convolve(f));
		}
		if (dt.connected())
		{
			CImg<> f(1,1,3,1,0.5,0.0,-0.5);
			dt = CImgList<>(img()[0].get_convolve(f));
		}
	}
	else //3d
	{
		if (dx.connected())
		{
			dx = CImgList<>(img().size);
			CImg<> f(3,1,1,1,0.5,0.0,-0.5);
			for (int t=0; t<img().size; t++)
				dx()[t]=img()[t].get_convolve(f);
			
		}
		if (dy.connected())
		{
			dy = CImgList<>(img().size);
			CImg<> f(1,3,1,1,0.5,0.0,-0.5);
			for (int t=0; t<img().size; t++)
				dy()[t]=img()[t].get_convolve(f);
			
		}
		if (dz.connected())
		{
			dz = CImgList<>(img().size);
			CImg<> f(1,1,3,1,0.5,0.0,-0.5);
			for (int t=0; t<img().size; t++)
				dz()[t]=img()[t].get_convolve(f);
			
		}
		if (dt.connected())
		{
			dt = CImgList<>(img().size,img()[0].width,img()[0].height,img()[0].depth,img()[0].dim);
			for (int t=0; t<img().size; t++)
				cimg_forXYZV(dt()[t],x,y,z,v)
					dt()(t,x,y,z,v) = 0.5f*(img().atNXYZV(t+1,x,y,z,v)-img().atNXYZV(t-1,x,y,z,v));
					
		}
	}
		
	
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
	
	if (dz.connected() && img().is_sameN(1))
	{
		targets = dz.getTargets();
		for(targetIter = targets.begin(); targetIter != targets.end(); targetIter++)
			res.insert(&((*targetIter)->getParent()));
	}
	
	if (dt.connected())
	{
		targets = dt.getTargets();
		for(targetIter = targets.begin(); targetIter != targets.end(); targetIter++)
			res.insert(&((*targetIter)->getParent()));
	}
	
	
	std::set<ParameteredObject*>::const_iterator objIter;
	for(objIter = res.begin(); objIter != res.end(); objIter++)
		(*(objIter))->update();
		
	
	

}

/*void bin2::getX(const cimg_library::CImg<> &img, CImg<>& res)
{
	CImg<> f(3,1,1,1,-0.5,0.0,0.5);
	//return img.get_convolve(f);
	res = img.get_convolve(f);
}

void bin2::getY(const cimg_library::CImg<> &img, CImg<>& res)
{
	CImg<> f(1,3,1,1,-0.5,0.0,0.5);
	//return img.get_convolve(f);
	res = img.get_convolve(f);
}

void bin2::getZ(const cimg_library::CImg<> &img, CImg<>& res)
{
	CImg<> f(1,1,3,1,-0.5,0.0,0.5);
	//return img.get_convolve(f);
	res = img.get_convolve(f);
} */
