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
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *  @date 8.09.2009
 */

#ifndef _SOLVER_HXX_
#define _SOLVER_HXX_

#include "Solver.h"
#include "Stencil.h"
#include <charon-utils/Roi.h>

template <typename T>
Solver<T>::MetaStencil::MetaStencil(const std::string& unknown,
									const std::vector<Stencil<T>*>& stencils) :
		left(0),		right(0),
		up(0),			down(0),
		backward(0),	forward(0),
		before(0),		after(0)
{
	// Iterate through the stencils and in each one, find the SubStencils
	// to the given unknown. Save the maximum of expansion in each dimension
	// and reposition the center of the meta stencil accordingly

	// stencil iterator
	typename std::vector<Stencil<T>*>::const_iterator sIt;
	for (sIt=stencils.begin() ; sIt != stencils.end() ; sIt++) {
		// SubStencil iterator
		typename std::map<std::string, SubStencil<T> >::const_iterator ssIt;
		ssIt = (*sIt)->get().find(unknown);
		//setting work-variable for better reading
		unsigned int centerx = ssIt->second.center.x;
		unsigned int centery = ssIt->second.center.y;
		unsigned int centerz = ssIt->second.center.z;
		unsigned int centert = ssIt->second.center.t;

		// Measuring the SubStencil that is currently being added
		// to the MetaStencil
		int width    = ssIt->second.pattern.dimx();
		int height   = ssIt->second.pattern.dimy();
		int depth    = ssIt->second.pattern.dimz();
		int duration = ssIt->second.pattern.dimv();
		// remember the re-declaration of the v-dimension of CImg
		// to be the time axis? Yeah, right ;-)

		// positioning of the center of the meta stencil
		//
		// it is important to understand, that the center can
		// onle move in positive directions - no more fancy
		// checks and calculations are needed.
		if (centerx > this->center.x) {this->center.x = centerx;}
		if (centery > this->center.y) {this->center.y = centery;}
		if (centerz > this->center.z) {this->center.z = centerz;}
		if (centert > this->center.t) {this->center.t = centert;}

		// setting the expansions
		//
		// Here's the same principle as with the center:
		// the individual expansions can only grow, not shrink
		// these lines just find the maximum
		// of expansions in all 8 directions
		// over all added SubStencils
		if (centerx            > this->left)     {this->left = centerx;}
		if (width-centerx-1    > this->right)    {this->right = width-centerx-1;}
		if (centery            > this->up)       {this->up = centery;}
		if (height-centery-1   > this->down)     {this->down = height-centery-1;}
		if (centerz            > this->backward) {this->backward = centerz;}
		if (depth-centerz-1    > this->forward)  {this->forward = depth-centerz-1;}
		if (centert            > this->before)   {this->before = centert;}
		if (duration-centert-1 > this->after)    {this->after = duration-centert-1;}

		// push_back the address of the just measured SubStencil
		this->substencils.push_back( &(ssIt->second) );
	}

	// expand the MetaStencil CImg to the appropriate size
	int dimx = left+1+right;
	int dimy = up+1+down;
	int dimz = backward+1+forward;
	int dimt = before+1+after;
	// initializing with 0 is important!
	this->data.assign(dimx, dimy, dimz, dimt, 0);

	// filling the pattern
	for (unsigned int i = 0 ; i < this->substencils.size() ; i++) {
		//saving the offset as Point4D for later convennience
		Point4D<int> offset = Point4D<int>(this->center)
			- Point4D<int>(this->substencils[i]->center);

		//Iterate through all pixels of the SubStencil...
		cimg_forXYZV(substencils[i]->pattern,xc,yc,zc,tc) {
			//...and set the pattern into the
			//MetaStencil (with offset).
			if (this->substencils[i]->pattern(xc,yc,zc,tc)) {
				Point4D<int> p(xc,yc,zc,tc);
				Point4D<int> sum = p+offset;
				assert(sum.x >= 0);
				assert(sum.x < dimx);
				assert(sum.y >= 0);
				assert(sum.y < dimy);
				assert(sum.z >= 0);
				assert(sum.z < dimz);
				assert(sum.t >= 0);
				assert(sum.t < dimt);
				Point4D<unsigned int> usum(sum);
				this->pattern.insert(usum);
			}
		}
	}
}

template <typename T>
Solver<T>::MetaStencil::MetaStencil(const typename Solver<T>::MetaStencil& rhs)
{
	*this = rhs;
}

template <typename T>
Solver<T>::MetaStencil::MetaStencil() {
}

template <typename T>
const Point4D<unsigned int>& Solver<T>::MetaStencil::getCenter() const {
	return center;
}

template <typename T>
typename Solver<T>::MetaStencil& Solver<T>::MetaStencil::operator = (
								const typename Solver<T>::MetaStencil& rhs) {
	if (&rhs == this)
		return *this;

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
	this->center      = rhs.center;

	return *this;
}

template <typename T>
std::set<Point4D<unsigned int> >& Solver<T>::MetaStencil::getPattern() {
	return pattern;
}

template <typename T>
void Solver<T>::MetaStencil::expand(Roi<int>& inRoi) {
	inRoi.xBegin = -int(left);
	inRoi.xEnd   = inRoi.xEnd + right;
	inRoi.yBegin = -int(up);
	inRoi.yEnd   = inRoi.yEnd + down;
	inRoi.zBegin = -int(backward);
	inRoi.zEnd   = inRoi.zEnd + forward;
	inRoi.tBegin = -int(before);
	inRoi.tEnd   = inRoi.tEnd + after;
}

template <typename T>
Solver<T>::Solver(const std::string& classname, const std::string& name) :
		TemplatedParameteredObject<T>(classname,name,"solves the linear system"),
		stencils(false,true)	//make stencil input slot mandatory and multi
{
	this->_addInputSlot(stencils,"stencil","Multi Input slot for stencils","Stencil<T>");
	this->_addInputSlot(roi,"roi","region of interest to work on","Roi<int>");
	this->_addOutputSlot(out,"out","CImgList containing the solution","CImgList<T>");
	out() = result;
}

template <typename T>
Solver<T>::~Solver() {}

#endif // _SOLVER_HXX_
