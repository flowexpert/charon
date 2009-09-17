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
/** @file PetscSolver.h
 *  Implementation of class PetscSolver.
 *  This is the PETSc implementation of a solver. It has been designed to run on
 *  multiple instances simultaneously.
 *  @see Solver.h
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#ifndef _PETSCSOLVER_H_
#define _PETSCSOLVER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef petscsolver_EXPORTS
///Visual C++ specific code
#define petscsolver_DECLDIR __declspec(dllexport)
#else
#define petscsolver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define petscsolver_DECLDIR
#endif

#include "Solver.h"
#include <petscksp.h>

///@todo incorporate std::runtime_error and #include <stdexept> instead of throw "string"

template <class T>
class petscsolver_DECLDIR PetscSolver : public Solver<T>
{
	protected:
		PetscInt		*columns;
		PetscScalar		*values;
		
		class PetscMetaStencil : public Solver<T>::MetaStencil
		{
			public:
				PetscMetaStencil(const std::string unknown,const std::vector<Stencil<T>*>& stencils);
				
				/**
				 * Write data form SubStencils into PetscScalar *values and
				 * their indices into PetscInt *columns.
				 * @param[in] unknown current unknown.
				 * @param[in] p current point,
				 * @param[in] unknownSizes sizes of matrix blocks.
				 * @param[out] columns Array of column indices for MatSetValues.
				 * @param[out] values Array of values for MatSteValues.
				 * @return Number of entries.
				 */
				unsigned int update(const std::string unknown,
				                    const Point4D<unsigned int>& p,
				                    const std::map<std::string,Roi<int> >& unknownSizes,
				                    PetscInt* &columns, PetscScalar* &values);
		};
		
		/**
		 * Converts a coordinate in an ROI into a relative index.
		 * @param[in] p Point to convert.
		 * @param[in] dim Dimensions of the ROI that p is in.
		 * @see getVectorIndex()
		 * @see getCoordinate()
		 * @return Relative vector index.
		 */
		unsigned int pointToRelativeIndex(const Point4D<unsigned int> p, const Roi<int> &dim) const;
		
		/**
		 * Converts a relative vector index to a global vector index.
		 * @param[in] i Relative vector index.
		 * @param[in] unknown The unknown in which the relative vector index is.
		 * @param[in] unknownSizes Map of ROIs associated to their unknown.
		 * @see getIndex()
		 * @see getCoordinate()
		 * @return Global vector index.
		 */
		unsigned int relativeIndexToGlobalIndex(const unsigned int i,
		                                        const std::string& unknown,
		                                        const std::map<std::string,Roi<int> >& unknownSizes);
		
		/**
		 * Convert a global vector index to 4-dimensional coordinates and the according unknown.
		 * @param[in] vi Global vector index.
		 * @param[in] unknownSizes Map of ROIs associated to their unknown.
		 * @param[out] unknown Unknown of the ROI in which the point is.
		 * @param[out] p Coordinates of the point.
		 * @see getIndex()
		 * @see getVectorIndex()
		 */
		void globalIndexToPoint(const unsigned int vi,
		                        const std::map<std::string, Roi<int> >& unknownSizes,
		                        std::string& unknown, Point4D<unsigned int>& p);
		
		/**
		 * Convert coordinates to the global index
		 * @param[in] p Current point
		 * @param[in] unknown Current unknown
		 * @param[in] unknownSizes Map of ROIs associated to their unknown
		 * @return global index
		 */
		unsigned int pointToGlobalIndex(const Point4D<unsigned int>& p, const std::string unknown,
		                                const std::map<std::string, Roi<int> >& unknownSizes);
		
		/**
		 * Find the closest real pixel to p (which is a ghost pixel).
		 * @param[in] p Ghost pixel to which the closest real pixel should be found
		 * @return Point4D object containing the coordinates of the closest
		 * boundary pixel of the unexpanded ROI
		 */
		Point4D<unsigned int>& getBoundary(Point4D<unsigned int> &p);
		
	public:	
		PetscSolver(const std::string& name = "");
		
		bool isRankZero();
		
		void execute();
				
		~PetscSolver();
};

#endif // _PETSCSOLVER_H_