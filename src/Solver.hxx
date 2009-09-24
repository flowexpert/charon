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
/** @file Solver.hxx
 *  Implementation of class Solver.
 *  This is the abstract base class of a solver form which all solver
 *  implementations should be derived. It contains the nested class MetaStencil,
 *  which is used to group multiple SubStencils.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#ifndef _SOLVER_HXX_
#define _SOLVER_HXX_

#include "Solver.h"
#include <Stencil.hxx>
#include <Roi.hxx>

///default constructor
template <class T>
Solver<T>::MetaStencil::MetaStencil(const std::string unknown,const std::vector<Stencil<T>*>& stencils) :
		left(0),right(0),up(0),down(0),backward(0),forward(0),before(0),after(0) {
	//Iterate through the stencils and in each one, find the SubStencils
	//to the given unknown. Save the maximum of expansion in each dimension
	//and reposition the center of the meta stencil accordingly
	typename std::vector<Stencil<T>*>::const_iterator sIt;		//stencil iterator
	for (sIt=stencils.begin() ; sIt != stencils.end() ; sIt++) {
		typename std::map<std::string, SubStencil<T> >::const_iterator ssIt;	//SubStencil iterator
		ssIt = (*sIt)->get().find(unknown);
		//setting work-variable for better reading
		unsigned int centerx = ssIt->second.center.x;
		unsigned int centery = ssIt->second.center.y;
		unsigned int centerz = ssIt->second.center.z;
		unsigned int centert = ssIt->second.center.t;
		
		//Measuring the SubStencil that is currently being added
		//to the MetaStencil
		int width    = ssIt->second.pattern.dimx();
		int height   = ssIt->second.pattern.dimy();
		int depth    = ssIt->second.pattern.dimz();
		int duration = ssIt->second.pattern.dimv();
		//remember the re-declaration of the v-dimension of CImg
		//to be the time axis? Yeah, right ;-)
		
		//positioning of the center of the meta stencil
		//it is important to understand, that the center can
		//onle move in positive directions - no more fancy
		//checks and calculations are needed.
		if (centerx > this->center.x) {this->center.x = centerx;}
		if (centery > this->center.y) {this->center.y = centery;}
		if (centerz > this->center.z) {this->center.z = centerz;}
		if (centert > this->center.t) {this->center.t = centert;}
		
		//setting the expansions
		//Here's the same principle as with the center:
		//the individual expansions can only grow, not shrink
		//these lines just find the maximum
		//of expansions in all 8 directions
		//over all added SubStencils
		if (centerx            > this->left)     {this->left = centerx;}
		if (width-centerx-1    > this->right)    {this->right = width-centerx-1;}
		if (centery            > this->up)       {this->up = centery;}
		if (height-centery-1   > this->down)     {this->down = height-centery-1;}
		if (centerz            > this->backward) {this->backward = centerz;}
		if (depth-centerz-1    > this->forward)  {this->forward = depth-centerz-1;}
		if (centert            > this->before)   {this->before = centert;}
		if (duration-centert-1 > this->after)    {this->after = duration-centert-1;}
								
		//push_back the address of the just measured SubStencil
		this->substencils.push_back( &(ssIt->second) );
	}
	
	//expand the MetaStencil CImg to the appropriate size
	int dimx=left+1+right;
	int dimy=up+1+down;
	int dimz=backward+1+forward;
	int dimt=before+1+after;
	this->data.assign(dimx, dimy, dimz, dimt, 0);	//initializing with 0 is important!
	
	//filling the pattern
	for (unsigned int i = 0 ; i < this->substencils.size() ; i++) {
		//saving the offset as Point4D for later convennience
		Point4D<unsigned int> offset = this->center - this->substencils[i]->center;
		
		//Iterate through all pixels of the SubStencil...
		for (int tc=0 ; tc < this->substencils[i]->pattern.dimv() ; tc++) {
			for (int zc=0 ; zc < this->substencils[i]->pattern.dimz() ; zc++) {
				for (int yc=0 ; yc < this->substencils[i]->pattern.dimy() ; yc++) {
					for (int xc=0 ; xc < this->substencils[i]->pattern.dimx() ; xc++) {
						//...and set the pattern into the
						//MetaStencil (with offset).
						if (this->substencils[i]->pattern(xc,yc,zc,tc)) {
							Point4D<unsigned int> p(xc,yc,zc,tc);
							this->pattern.insert(p+offset);
						}
					}
				}
			}
		}
	}
}
				
///copy constructor
template <class T>
Solver<T>::MetaStencil::MetaStencil(const Solver<T>::MetaStencil& rhs) {
	this->substencils = rhs.substencils;
	this->data        = rhs.data;
	this->pattern     = rhs.pattern;
	this->left        = rhs.left;
	this->right       = rhs.right;
	this->up          = rhs.up;
	this->down        = rhs.down;
	this->backward    = rhs.backward;
	this->forward     = rhs.forward;
	this->before      = rhs.before;
	this->after       = rhs.after;
}

template <class T>
Solver<T>::MetaStencil::MetaStencil() {}
		
///assignment operator
template <class T>
///@todo why is 'typename' necessary here?
typename Solver<T>::MetaStencil& Solver<T>::MetaStencil::operator=(Solver<T>::MetaStencil& rhs) {
	if (&rhs == this) {return *this;}
	
	this->substencils = rhs.substencils;
	this->data        = rhs.data;
	this->pattern     = rhs.pattern;
	this->left        = rhs.left;
	this->right       = rhs.right;
	this->up          = rhs.up;
	this->down        = rhs.down;
	this->backward    = rhs.backward;
	this->forward     = rhs.forward;
	this->before      = rhs.before;
	this->after       = rhs.after;
	
	return *this;
}

//the only necessary getter to determine the maximum number of
//entries.
template <class T>
std::set<Point4D<unsigned int> >& Solver<T>::MetaStencil::getPattern() {return pattern;}

template <class T>		
void Solver<T>::MetaStencil::expand(Roi<int>& inRoi) {
	inRoi.top    = -(this->up);
	inRoi.left   = -(this->left);
	inRoi.bottom = inRoi.bottom + this->down;
	inRoi.right  = inRoi.right + this->right;
	inRoi.front  = -(this->backward);
	inRoi.back   = inRoi.back + this->forward;
	inRoi.before = -(this->before);
	inRoi.after  = inRoi.after + this->after;
}

template <class T>
Solver<T>::Solver(const std::string& classname, const std::string& name) : 
		TemplatedParameteredObject<T>(classname,name,"solves the linear system"),
		stencils(false,true)	//make stencil input slot mandatory and multi
{
	this->_addInputSlot(stencils,"stencil","Multi Input slot for stencils","stencil<T>*");
	this->_addInputSlot(roi,"roi","region of interest to work on","Roi<int>*");
	this->_addOutputSlot(out,"out","CImgList containing the solution","CImgList");
	out() = &result;
}

template <class T>
Solver<T>::~Solver() {}

#endif // _SOLVER_HXX_