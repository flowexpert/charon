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
/** @file Solver.h
 *  Declaration of the parameter class Solver.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *  @date 8.09.2009
 */

#ifndef _SOLVER_H_
#define _SOLVER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef solver_EXPORTS
/// Visual C++ specific code
#define solver_DECLDIR __declspec(dllexport)
#else
#define solver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define solver_DECLDIR
#endif

#include <charon-utils/CImg.h>
#include <charon-core/ParameteredObject.h>
#include <charon-utils/Roi.h>
#include "Stencil.h"
/// Abstract base class for solvers.
/**
 *  This is the abstract base class of a solver form which all solver
 *  implementations should be derived. It contains the nested class MetaStencil,
 *  which is used to group multiple SubStencils.
 *
 *  \ingroup charon-flow-solvers
 */
template <typename T>
class solver_DECLDIR Solver : public TemplatedParameteredObject<T>
{
protected:
	/// meta stencil class to combine multiple SubStencils by unknown
	/**
	 * Your implementation of the MetaStencil will need a function
	 * to gather the data from the different SubStencils and present
	 * it in any way you want it to in order to work with your
	 * solver.
	 * It would be appreciated if you'd name this function 'update'.
	 */
	class MetaStencil
	{
	protected:
		/// Vector of pointers to the SubStencils
		/// (the data for the solver will be pulled from here)
		std::vector<const SubStencil<T>* > substencils;

		/// CImg representing the MetaStencil
		/// (SubStencils data will be merged here)
		cimg_library::CImg<T> data;

		/// Set of points that belong to this MetaStencil
		/// (the size of the MetaStencil has to be extracted from here)
		std::set<Point4D<int> > pattern;

		/// \name expansions in all 8 directions
		//@{
		int left       /** x negative*/, right;    ///< x positive
		int up         /** y negative*/, down;     ///< y positive
		int backward   /** z negative*/, forward;  ///< z positive
		int before     /** t negative*/, after;    ///< t positive
		//@}

		/// coordinates of the center of the meta stencil
		Point4D<int> center;

	public:
		/// default constructor
		/** \param[in] unknown		name of the unknown for this stencil group
		 *  \param[in] stencils		list of stencils to group
		 */
		MetaStencil(
				const std::string& unknown,
				const std::vector<Stencil::Mask<T>*>& stencils);

		/// copy constructor
		MetaStencil(const MetaStencil& rhs /**< [in] copy source*/);

		/// empty constructor for use in map
		MetaStencil();

		/// assignment operator
		/** \param[in] rhs copy source
		 *  \returns assigned object
		 */
		virtual MetaStencil& operator=(const MetaStencil& rhs);

		/// getter for MetaStencil::center
		const Point4D<int>& getCenter() const;

		/// getter for MetaStencil::pattern
		/** The only necessary getter to determine the maximum number of
		 *  entries.
		 *  \returns pattern
		 */
		virtual std::set<Point4D<int> >& getPattern();

		/// Expand the given region of interest to include the necessary
		/// ghost nodes.
		/** @param[in,out] inRoi  Region of interest to expand
		 *      (Will be set to the epxanded region of interest)
		 */
		virtual void expand(Roi<int>& inRoi) const;
	};

public:
	/// pointers, that the solver will use
	InputSlot< Stencil::Base<T>* > stencils;

	/// region of interes for the solver to work on
	InputSlot< Roi<int>* > roi;

	/// result
	OutputSlot<cimg_library::CImgList<T> > out;

	/// default constructor
	Solver(
		const std::string& classname,	///< [in] class name
		const std::string& name = ""	///< [in] instance name
	);

	/// default destructor
	virtual ~Solver();
};

#endif // _SOLVER_H_
