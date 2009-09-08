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
 *  implementations should be derived. It contains the nested class Metastencil,
 *  which is used to group multiple substencils.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#ifndef _SOLVER_HXX_
#define _SOLVER_HXX_

#include "CImg.h"
#include "petscksp.h"
#include "ParameteredObject.h"
#include "Roi.h"
#include "Stencil.hxx"

template <class T>
class Solver : public ParameteredObject
{
	private:
		InpurSlot< Stencil<T>* > stencils;
		InputSlot< Roi<int>* > roi;
		OutputSlot<cimg_library::CImgList<T>& > out;
				
		//meta stencil class to combine multiple substencils by unknown
		class MetaStencil
		{
			private:
				/**
				 * Vector of pointers to the substencils.
				 * The data for the solver will be pulled from here.
				*/
				std::vector< Substencil<T>* > substencils;
				
				/**
				 * CImg representing the metastencil.
				 * This is a dummy to pre-allocate memory and to store the
				 * dimensions of the metastencil.
				 * Later, the data of the substencils will be merged here.
				 */
				cimg_library::CImg<T> data;
				
				/**
				 * Set of points that belong to this metastencil.
				 * @remark The size of the metastencil has to be extracted from the pattern.
				 */
				std::set<Point4D> pattern;
				
				///@todo changed int -> unsigned int - re-check code to avoid hiccups
				//expansions in all 8 directions
				unsigned int left=0     , right=0;	//x (left is negative, right is positive)
				unsigned int up=0       , down=0;	//y (up is negative, down is positive)
				unsigned int backward=0 , forward=0;	//z (backward is negative, forward is positive)
				unsigned int before=0   , after=0;	//t (before is negative, after is positive)
				
				Point4D center;				//coordinates of the center of the meta stencil
			public:
				///default constructor
				MetaStencil(const std::string unknown,const std::vector<Stencil<T>*>& stencils) {
					//Iterate through the stencils and in each one, find the substencils
					//to the given unknown. Save the maximum of expansion in each dimension
					//and reposition the center of the meta stencil accordingly
					std::vector<Stencil<T>*>::iterator sIt;		//stencil iterator
					for (sIt=stencils.begin() ; sIt != stencils.end() ; sIt++) {
						std::map<std::string, Substencil<T> >::iterator ssIt;	//Substencil iterator
						ssIt = sIt->get().find(unknown);
						//setting work-variable for better reading
						int centerx = ssIt->second->center.x;
						int centery = ssIt->second->center.y;
						int centerz = ssIt->second->center.z;
						int centert = ssIt->second->center.t;
						
						//Measuring the substencil that is currently being added
						//to the metastencil
						int width    = ssIt->second->pattern.dimx();
						int height   = ssIt->second->pattern.dimy();
						int depth    = ssIt->second->pattern.dimz();
						int duration = ssIt->second->pattern.dimv();
						//remember the re-declaration of the v-dimension of CImg
						//to be the time axis? Yeah, right ;-)
						
						//positioning of the center of the meta stencil
						//it is important to understand, that the center can
						//onle "wander" in positive directions - no more fancy
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
						//over all added substencils
						if (centerx            > this->left)     {this->left = centerx;}
						if (width-centerx-1    > this->right)    {this->right = width-centerx-1;}
						if (centery            > this->up)       {this->up = centery;}
						if (height-centery-1   > this->down)     {this->down = height-centery-1;}
						if (centerz            > this->backward) {this->backward = centerz;}
						if (depth-centerz-1    > this->forward)  {this->forward = depth-centerz-1;}
						if (centert            > this->before)   {this->before = centert;}
						if (duration-centert-1 > this->after)    {this->after = duration-centert-1;}
												
						//push_back the address of the just measured substencil
						this->substencils.push_back( &(*ssIt) );
					}
					
					//expand the metastencil CImg to the appropriate size
					int dimx=left+1+right;
					int dimy=up+1+down;
					int dimz=backward+1+forward;
					int dimt=before+1+after;
					data.assign(dimx, dimy, dimz, dimt, 0);
				}
								
				///copy constructor
				Metastencil(const Metastencil<T>& rhs) {
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
				
				///assignment operator
				MetaStencil<T>& operator=(Metastencil<T>& rhs) {
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
				std::set<Point4D>& getPattern() {return pattern;}
				
				//Your implementation of the MetaStencil will need a function
				//to gather the data from the different substencils and present
				//it in any way you want it to in order to work with your
				//solver.
				//It would be appreciated if you'd name this function 'update'.
				
				/**
				 * Expand the given region of interest to include the necessary ghost nodes.
				 * @param[in] inRoi Region of interest to expand.
				 * @return Epxanded region of interest.
				 */
				Roi<int> expand(const Roi<int>& inRoi) {
					int t  = -up;
					int l  = -left;
					int bo = inRoi.getHeight() + down;
					int r  = inRoi.getWidth() + right;
					int f  = -backward;
					int ba = inRoi.getDepth() + forward;
					int be = -before;
					int af = inRoi.getDuration() + after;
					
					Roi<int> roi(t, l, bo, r, f, ba, be, af, "");
					return roi;
				}
		};
	public:
		///Default constructor.
		Solver(const std::string& classname, const std::string& name = "") : 
			ParameteredObject(classname,name,"solves the linear system"),
			stencils(false,true)	//make stencil input slot mandatory and multi
		{
			_addInputSlot(stencils,"stencil","Multi Input slot for stencils","stencil<T>*");
			_addInputSlot(roi,"roi","region of interest to work on","Roi<int>*");
			_addOutputSlot(out,"out","CImgList containing the solution","CImgList");
		}
			
		virtual void update() =0;
		//this is the one, only and primary function of the solver
		//once every object is connected, update() will bi called recursively
		//so all the calculations will take place in this function, thus in
		//the end, the correct data will be presented in the outputSlot
		//of the solver
		
		virtual ~Solver();
};

#endif // _SOLVER_HXX_