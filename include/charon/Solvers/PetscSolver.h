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
 *  Declaration of the parameter class PetscSolver.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *  @date 8.09.2009
 */

#ifndef _PETSCSOLVER_H_
#define _PETSCSOLVER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef petscsolver_EXPORTS
/// dll import/export
#define petscsolver_DECLDIR __declspec(dllexport)
#else
#define petscsolver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// no dll handling on gcc
#define petscsolver_DECLDIR
#endif

#include <petscksp.h>
#include "../Solver.h"

class PetscInit;

/// PETSc implementation of a solver
/**
 *  This solver is based on the
 *  <a href="http://www.mcs.anl.gov/petsc/petsc-as/">PETSc library</a>.
 *  It has been designed to run on multiple instances (machines, cpus)
 *  simultaneously.
 *
 *  \ingroup charon-flow-solvers
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class petscsolver_DECLDIR PetscSolver : public Solver<T>
{
protected:
	/// MetaStencil implementation for usage with PETSc
	class PetscMetaStencil : public Solver<T>::MetaStencil
	{
	public:
		/// default constructor
		PetscMetaStencil(
				const std::string& unknown  /**[in] unknown name*/,
				const std::vector<Stencil<T>*>& stencils
					/**[in] stencils for this unknown*/);

		/// copy constructor
		PetscMetaStencil(const PetscMetaStencil& rhs /**[in] copy source*/);

		/// empty constructor for use in map
		PetscMetaStencil();

		/// update columns and values
		/**
		 *  Write data from SubStencils into PetscScalar* values and
		 *  their indices into PetscInt* columns.
		 *  \param[in]  unknown       current unknown
		 *  \param[in]  p             current point
		 *  @param[in]  unknownSizes  sizes of matrix blocks.
		 *  @param[out] columns       Array of column indices for MatSetValues.
		 *  @param[out] values        Array of values for MatSteValues.
		 *  @return                   Number of entries.
		 */
		unsigned int update(
				const std::string& unknown,
				const Point4D<int>& p,
				const std::map<std::string,const Roi<int>*>& unknownSizes,
				PetscInt*& columns,
				PetscScalar*& values);
	};

	/// Convert ROI coordinate to relative index
	/**
	 *	@see getVectorIndex()
	 *	@see getCoordinate()
	 *	@return                       Relative vector index.
	 */
	inline static unsigned int _pointToRelativeIndex(
			const Point4D<int>& p /**[in] point to convert*/,
			const Roi<int>& dim   /**[in] dimensions of the ROI around p*/);

	/// Convert a relative vector index to a global vector index.
	/**
	 *	@param[in] i                  Relative vector index.
	 *	@param[in] unknown            The unknown in which the relative
	 *	                              vector index is.
	 *	@param[in] unknownSizes       Map of ROIs associated to their unknown.
	 *	@see getIndex()
	 *	@see getCoordinate()
	 *	@return                       Global vector index.
	 */
	inline static unsigned int _relativeIndexToGlobalIndex(
			const unsigned int i,
			const std::string& unknown,
			const std::map<std::string,const Roi<int>* >& unknownSizes);

	/// Convert a global vector index to 4-dimensional coordinates and
	/// the according unknown.
	/**
	 *	For each unknown, there is a vectorized block containing all values
	 *	in the corresponding region of interest. To determine the real
	 *	coordinate of a point given it's vector index in this block, we have
	 *	to determine the unknown block where vi lies in an then calculate
	 *	the position inside the roi by back-transformation from the vectorized
	 *	version (multiple applicaton of modulo). At the end, the global
	 *	position of the roi is corrected (using x/y/z/t-Begin as offset).
	 *	@param[in]  vi                Global vector index.
	 *	@param[in]  unknownSizes      Map of ROIs associated to their unknown.
	 *	@param[out] unknown           Unknown of the ROI in which the point is
	 *	@param[out] p                 Coordinates of the point.
	 *	@see getIndex()
	 *	@see getVectorIndex()
	 */
	inline static void _globalIndexToPoint(
			const unsigned int& vi,
			const std::map<std::string, const Roi<int>* >& unknownSizes,
			std::string& unknown, Point4D<int>& p);

	/// Convert coordinates to the global index
	/**
	 *	@param[in] p                  Current point
	 *	@param[in] unknown            Current unknown
	 *	@param[in] unknownSizes       Map of ROIs associated to their unknown
	 *	@return                       global index
	 */
	inline static unsigned int _pointToGlobalIndex(
			const Point4D<int>& p,
			const std::string& unknown,
			const std::map<std::string, const Roi<int>* >& unknownSizes);

	/// Find the closest real pixel to p (which is a ghost pixel).
	/**
	 *	@param[in] p                  Ghost pixel to which the closest real
	 *	                              pixel should be found
	 *
	 *	@return                       Point4D object containing the
	 *	                              coordinates of the closest boundary
	 *	                              pixel of the unexpanded ROI
	 */
	Point4D<int> _getBoundary(Point4D<int>& p) const;

	/// Add cross terms which are not handled by MetaStencil
	/** \remarks   Make sure that update() has been called to the stencils
	 *             with the same unknown given to this stencil
	 *  \param[in] unknown            currently handled unknown
	 *  \param[in] p                  currently handled point
	 *                                (has to be a real point)
	 *  \param[in] unknownSizes       Map of ROIs associated to their unknown
	 *  \param[out] columns           Array of column indices for MatSetValues.
	 *  \param[out] values            Array of values for MatSteValues.
	 *  \return                       Number of entries.
	 */
	unsigned int _addCrossTerms(
			const std::string& unknown,
			const Point4D<int>& p,
			const std::map<std::string, const Roi<int>*>& unknownSizes,
			PetscInt*& columns,
			PetscScalar*& values) const;

public:
	/// default constructor
	PetscSolver(const std::string& name = "" /**[in] instance name*/);

	/// add hint how many entries to allocate per row
	/// (zero = auto guess)
	Parameter<unsigned int> entriesPerRowHint;

	/// Petsc command line options
	Parameter<std::string> commandLine;

	/// check if process is rank 0
	bool isRankZero();

	/// encapsulated execute function for PETSc calls
	int petscExecute();

	/// default destructor
	virtual ~PetscSolver();

protected:
	/// main function
	virtual void execute();

private:
	/// command line argument counter
	int _argc;
	/// command line argument vector
	char** _argv;
	/// check if initialization has taken place
	static bool _initialized;
};

#endif // _PETSCSOLVER_H_
