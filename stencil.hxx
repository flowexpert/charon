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
/** @file stencil.hxx
 *  Implementation of class Stencil.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 21.08.2009
 */


#ifndef _stencil_HXX_
#define _stencil_HXX_

#include "CImg.h"
#include "ParameteredObject.h"
#include "Parameter.h"
#include <vector>
#include <string>
#include <map>
#include <set>

/*
==============================================================
General nomenclatur for stencils, metastentils and substencils
==============================================================

This is the class Stencil, which represents a collection of masks or set of
weights or pattern for different unknowns - or maybe no unknowns. Each mask is a
substencil. So there is a substencil for every unknown a stencil knows - or
just one substencil in general if the stencil has no unknowns.
In the stencil class, these substencils are grouped by method - so there is one
set of substencils in the L2Norm stencil and another set of
masks in the GBCCE Stencil etc.
In the class Solver, there is a nested class MetaStencil, which regroups the
substencils of the attached stencils by unknown.
Roundup:
Stencil:      Set of substencils grouped by method
MetaStencil:  Set of substencils grouped by unknown
SubStencil:   Mask for one unknown or the general mask if no unknowns are used
*/

/**
 * This is the base class for Stencils. It manages the communication between the
 * solver and the different motion- and brightnesmodels. Or it can present a
 * non-variant stencil to the solver.
 */
template <class T>
class Stencil : public ParameteredObject
{
	protected:
		///Lambda coefficient of the stencil.
		Parameter<T> lambda;
		
		///Set of the names of unknowns on which this stencil works.
		std::set<std::string> unknowns;
		
		/**
		 * Map to store and manage all the substencils.
		 * @remarks
		 * -   This member must never be erased as long as the stencil exists
		 * 	   because this would break the pointers of following objects
		 */
		std::map<std::string, cimg_library::CImg<T> > substencils;

		/**
		 * Map to store and manage all the right hand sides.
		 * @remarks
		 * -   This member must never be erased as long as the stencil exists
		 * 	   because this would break the pointers of following objects
		 */
		std::map<std::string, T>& rhs;
		
		///Coordinates of the center in the order x, y, z, t.
		int center[4];
		
		OutputSlot<Stencil*> out;

		///default constructor
		Stencil(const std::string& classname, const std::string& name = "") : 
			ParameteredObject(classname,name,
			"discretizes partial differential equation terms or defines derivatives filters for images"),
			out(this) {
				_addOutputSlot(out,"this","Pointer to itself","Stencil<T>*");
				_addParameter(lambda,"lambda","weight of the pde term",1);
		}
	
	public:
		//updates the stencil to contain the information to the given coordinate				
		virtual void updateStencil(
			const unsigned int x,
			const unsigned int y,
			const unsigned int z=0,
			const unsigned int t=0,
			const unsigned int v=0);
			
		//Fills the stencils with 1s for the solver to grasp the structure of
		//the stencil.
		virtual void getStructure() =0;
		
		std::map<std::string, cimg_library::CImg<T> >& get() const
			{return substencils;}
				
		virtual cimg_library::CImgList<T>& apply(
			const cimg_library::CImgList<T>& seq,
			const unsigned int frame) const;
													
		int getCenterX() const {return center[0];}
		int getCenterY() const {return center[1];}
		int getCenterZ() const {return center[2];}
		int getCenterT() const {return center[3];}

		//Getter for the names of unknowns, overload this in your implementation
		//if you have unknowns
		virtual const std::set<std::string>& getUnknowns() const {return unknowns;}
				
		virtual ~Stencil();
}

#endif //_stencil_HXX_