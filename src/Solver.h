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

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef solver_EXPORTS
///Visual C++ specific code
#define solver_DECLDIR __declspec(dllexport)
#else
#define solver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define solver_DECLDIR
#endif

#include <CImg.h>
#include <petscksp.h>
#include <ParameteredObject.h>
#include <Roi.h>
#include "Stencil.h"

template <class T>
class solver_DECLDIR Solver : public TemplatedParameteredObject<T>
{
	private:
		InputSlot< Stencil<T>* > stencils;
		InputSlot< Roi<int>* > roi;
		OutputSlot<cimg_library::CImgList<T>& > out; //Pointer? Reference?
				
		//meta stencil class to combine multiple SubStencils by unknown
		class MetaStencil
		{
			private:
				/**
				 * Vector of pointers to the SubStencils.
				 * The data for the solver will be pulled from here.
				*/
				std::vector< SubStencil<T>* > substencils;
				
				/**
				 * CImg representing the MetaStencil.
				 * This is a dummy to pre-allocate memory and to store the
				 * dimensions of the MetaStencil.
				 * Later, the data of the SubStencils will be merged here.
				 */
				cimg_library::CImg<T> data;
				
				/**
				 * Set of points that belong to this MetaStencil.
				 * @remark The size of the MetaStencil has to be extracted from the pattern.
				 */
				std::set<Point4D<unsigned int> > pattern;
				
				///@todo changed int -> unsigned int - re-check code to avoid hiccups
				//expansions in all 8 directions
				unsigned int left       , right;	//x (left is negative, right is positive)
				unsigned int up         , down;		//y (up is negative, down is positive)
				unsigned int backward   , forward;	//z (backward is negative, forward is positive)
				unsigned int before     , after;	//t (before is negative, after is positive)
				
				Point4D<unsigned int> center;				//coordinates of the center of the meta stencil
			public:
				///default constructor
				MetaStencil(const std::string unknown,const std::vector<Stencil<T>*>& stencils);
								
				///copy constructor
				MetaStencil(const MetaStencil& rhs);
				
				///assignment operator
				virtual MetaStencil& operator=(MetaStencil& rhs);
				
				//the only necessary getter to determine the maximum number of
				//entries.
				virtual std::set<Point4D<unsigned int> >& getPattern();
				
				//Your implementation of the MetaStencil will need a function
				//to gather the data from the different SubStencils and present
				//it in any way you want it to in order to work with your
				//solver.
				//It would be appreciated if you'd name this function 'update'.
				
				/**
				 * Expand the given region of interest to include the necessary ghost nodes.
				 * @param[in] inRoi Region of interest to expand.
				 * @return Epxanded region of interest.
				 */
				virtual Roi<int> expand(const Roi<int>& inRoi);
		};
	public:
		///Default constructor.
		Solver(const std::string& classname, const std::string& name = "");
			
		virtual void execute() =0;
		//this is the one, only and primary function of the solver
		//once every object is connected, update() will bi called recursively
		//so all the calculations will take place in this function, thus in
		//the end, the correct data will be presented in the outputSlot
		//of the solver
		
		virtual ~Solver();
};

#endif // _SOLVER_HXX_