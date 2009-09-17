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
/** @file Stencil.h
 *  Implementation of class Stencil.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#ifndef _STENCIL_H_
#define _STENCIL_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef stencil_EXPORTS
///Visual C++ specific code
#define stencil_DECLDIR __declspec(dllexport)
#else
#define stencil_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define stencil_DECLDIR
#endif

#include <CImg.h>
#include <ParameteredObject.h>
#include "Point4D.h"
#include "SubStencil.h"
#include <string>
#include <map>
#include <set>

/*
==============================================================
General nomenclatur for stencils, metastentils and SubStencils
==============================================================

This is the class Stencil, which represents a collection of masks or set of
weights or pattern for different unknowns - or maybe no unknowns. Each mask is a
SubStencil. So there is a SubStencil for every unknown a stencil knows - or
just one SubStencil in general if the stencil has no unknowns.
In the stencil class, these SubStencils are grouped by method - so there is one
set of SubStencils in the L2Norm stencil and another set of
masks in the GBCCE Stencil etc.
In the class Solver, there is a nested class MetaStencil, which regroups the
SubStencils of the attached stencils by unknown.
SubStencils are a class of their own that contain their pattern
(for the solver), the actual data (which is put there by the updateStencil
method of their stencil) and their center, represented by a Point4D.
Roundup:
Stencil:      Set of SubStencils grouped by method
MetaStencil:  Set of SubStencils grouped by unknown
SubStencil:   Mask for one unknown or the general mask if no unknowns are used
*/

/**
 * This is the base class for Stencils.
 * It manages the communication between the solver and the different
 * motion- and brightnesmodels. Or it can present a non-variant stencil
 * to the solver.
 */
template <class T>
class stencil_DECLDIR Stencil : public TemplatedParameteredObject<T>
{
	protected:
		///Set of the names of unknowns on which this stencil works.
		std::set<std::string> unknowns;
		
		/**
		 * Map to store and manage all the SubStencils.
		 * @remarks
		 * -   This member must never be erased as long as the stencil exists
		 * 	   because this would break the pointers of following objects
		 */
		std::map<std::string, SubStencil<T> > substencils;

		/**
		 * Map to store and manage all the right hand sides.
		 * @remarks
		 * -   This member must never be erased as long as the stencil exists
		 * 	   because this would break the pointers of following objects
		 */
		std::map<std::string, T> rhs;

	public:
		///Lambda coefficient of the stencil.
		Parameter<T> lambda;
		
		///Output slot containing the this-pointer of the object		
		OutputSlot<Stencil<T>*> out;

		///default constructor
		Stencil(const std::string& classname, const std::string& name = "");
		
		/**
		 * Updates the stencil to contain the information to the given coordinate.
		 * The data will be placed in the member data of the individual SubStencils.
		 * @param[in] x x coordinate.
		 * @param[in] y y coordinate.
		 * @param[in] z z coordinate.
		 * @param[in] t t coordinate.
		 * @param[in] v v coordinate
		 */
		virtual void updateStencil(const unsigned int x, const unsigned int y,
                                   const unsigned int z=0, const unsigned int t=0,
                                   const unsigned int v=0);
		
		/**
		 * Getter function for the SubStencils of the stencil.
		 * @return reference to the map of SubStencils.
		 */
		std::map<std::string, SubStencil<T> >& get() const;
		
		std::map<std::string, T>& getRhs() const;
				
		virtual cimg_library::CImgList<T>& apply(const cimg_library::CImgList<T>& seq,
		                                         const unsigned int frame) const;
						
		/**
		 * Getter functions for the unknowns of the stencil.
		 * @return reference to the set that include the unknowns as strings.
		 */
		virtual const std::set<std::string>& getUnknowns() const;
		
		/**
		 * Default destructor.
		 */
		virtual ~Stencil();
};

#endif //_stencil_H_