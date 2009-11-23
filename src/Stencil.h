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

#include <charon-utils/CImg.h>
#include <charon-core/ParameteredObject.h>
#include "Point4D.h"
#include "Substencil.h"
#include <string>
#include <map>
#include <set>

/**
 * This is the base class for Stencils.
 * It manages the communication between the solver and the different
 * motion- and brightnesmodels. Or it can present a non-variant stencil
 * to the solver.
 *
 * <b>General nomenclatur for stencils, metastentils and SubStencils</b>
 *
 * This is the class Stencil, which represents a collection of masks or set of
 * weights or pattern for different unknowns - or maybe no unknowns. Each mask is a
 * SubStencil. So there is a SubStencil for every unknown a stencil knows - or
 * just one SubStencil in general if the stencil has no unknowns.
 * In the stencil class, these SubStencils are grouped by method - so there is one
 * set of SubStencils in the L2Norm stencil and another set of
 * masks in the GBCCE Stencil etc.
 * In the class Solver, there is a nested class MetaStencil, which regroups the
 * SubStencils of the attached stencils by unknown.
 * SubStencils are a class of their own that contain their pattern
 * (for the solver), the actual data (which is put there by the updateStencil
 * method of their stencil) and their center, represented by a Point4D.
 * 
 * Roundup:
 * - Stencil:      Set of SubStencils grouped by method
 * - MetaStencil:  Set of SubStencils grouped by unknown
 * - SubStencil:   Mask for one unknown or the general mask
 *                 if no unknowns are used
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
		 *     because this would break the pointers of following objects
		 */
		std::map<std::string, SubStencil<T> > substencils;

		/**
		 * Map to store and manage all the right hand sides.
		 * @remarks
		 * -   This member must never be erased as long as the stencil exists
		 *     because this would break the pointers of following objects
		 */
		std::map<std::string, T> rhs;

	public:
		/// Lambda coefficient of the stencil.
		Parameter<T> lambda;
		
		/// Output slot containing the this-pointer of the object		
		OutputSlot<Stencil<T>*> out;

		/// default constructor
		Stencil(const std::string& classname, const std::string& name = "");

		/// update stencil
		/**
		 *  Updates the stencil to contain the information to the given
		 *  coordinate. The data will be placed in the member data of the
		 *  individual SubStencils. Usually, there are as many equations
		 *  per unknown in the linear equation system as there are pixels
		 *  in the considered roi. To be able to get all these equations,
		 *  you have to provide the current unknown and the position in
		 *  the roi. If there is only one equation or you are implementing
		 *  a static stencil, discard the unneeded coordinates and set the
		 *  substencils for all unknowns that differ from the given one to
		 *  zero.
		 *  \param[in] unknown    query substencils for this unknown,
		 *                        e.g. the Euler-Lagrange equation after
		 *                        deriving wrt the given unknown.
		 *  @param[in] x,y,z,t,v  coordinates
		 */
		virtual void updateStencil(
				const std::string& unknown,
				const unsigned int x=0,
				const unsigned int y=0,
				const unsigned int z=0,
				const unsigned int t=0,
				const unsigned int v=0) = 0;

		/**
		 * Getter function for the SubStencils of the stencil.
		 * @return reference to the map of SubStencils.
		 */
		const std::map<std::string, SubStencil<T> >& get() const;

		/**
		 * Getter function for the right hand side.
		 * @return a map containing all right hand sides, associated to unknowns.
		 */
		const std::map<std::string, T>& getRhs() const;

		/// apply stencil to a sequence
		/** \param seq           sequence to apply stencil on
		 *  \param frame         frame to apply stencil on
		 *  \return              processed version of the input sequence
		 */
		virtual cimg_library::CImg<T> apply(
				const cimg_library::CImgList<T>& seq,
				const unsigned int frame) const = 0;

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
