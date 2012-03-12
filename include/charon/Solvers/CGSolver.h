/*  Copyright (C) 2011 Heidelberg Collaboratory for Image Processing

    This file is part of Charon.

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
/** \file CGSolver.h
 *  Declaration of the parameter class CGSolver.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 12.10.2011
 */

#ifndef _CGSOLVER_H_
#define _CGSOLVER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef cgsolver_EXPORTS
/// dll import/export
#define cgsolver_DECLDIR __declspec(dllexport)
#else
#define cgsolver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// no dll handling on gcc
#define cgsolver_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon/CGSolverHelper.h>

#include <charon/Stencil.h>
#include <charon-utils/CImg.h>
#include <vector>

#include <charon-utils/Roi.h>

/// Unconstrained gradient based optimization using nonlinear
/// conjugate gradients
/**
 *  This optimizer is based upon
 *  <a href="http://www.gaussianprocess.org/gpml/code/matlab/util/minimize.m">
 *      minimize.m</a>,
 *  by Carl Edward Rasmussen. See its
 *  <a href="http://www.gaussianprocess.org/gpml/code/matlab/">
 *      Documentation</a>
 *
 *  \ingroup charon-flow-solvers
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class cgsolver_DECLDIR CGSolver : public TemplatedParameteredObject<T>
{
public:
	/// Input slot for Stencils
	InputSlot< Stencil::Base<T>* > stencils;

	/// Input slot for the quantity to be optimized.
	InputSlot< CGSolverHelper<T>* > itHelper;

	/// Output slot for feedback
	OutputSlot< cimg_library::CImgList<T> > result;

	/// length parameter, see "minimize.m" for details
	Parameter< int > length;

	///  flow dimensions
	Parameter< int > flowDimensions;

	///  write intermediate results
	Parameter< bool > writeIntermediateResults;

	///  do not fail line search
	Parameter< bool > doNotFailLineSearch;

	/// default constructor
	CGSolver(const std::string& name = "" /**[in] instance name*/);

	/// minimize function
	void minimize(
		cimg_library::CImgList<T> &_itflow,
		std::vector<T> &X,
		std::vector<T> &startingPoint_X,
		typename std::set<AbstractSlot<Stencil::Base<T>*>*>::const_iterator&
			stencilsBegin,
		typename std::set<AbstractSlot<Stencil::Base<T>*>*>::const_iterator&
			stencilsEnd,
		int length
	);

	/// default destructor
	~CGSolver();

protected:
	/// main function
	void execute();

private:
	/// size parameter of the quantities.
	int _pSize;
	/// width parameter of the quantities.
	int _pWidth;
	/// height parameter of the quantities.
	int _pHeight;
	/// depth parameter of the quantities.
	int  _pDepth;
	/// spectrum parameter of the quantities.
	int _pSpectrum;

	/// linear indexing
	int _linearIndex( int n, int x, int y, int z, int c,
		int pSize, int pWidth, int pHeight, int pDepth, int pSpectrum );

	/// function to convert a std::vector to a CImgList
	cimg_library::CImgList<T> _reshapeFeedback( std::vector<T> v );

	/// pointer to iteration helper
	CGSolverHelper<T> *_help;
};

/// vector scaling
template <typename T> std::vector<T> _scaleVector(
	const T &skalar, const std::vector<T> &vektor);
/// vector addition
template <typename T> std::vector<T> _addVectors(
	const std::vector<T> &v1, const std::vector<T> &v2);
/// vector dot product
template <typename T> T _dotProduct(
	const std::vector<T> &v1, const std::vector<T> &v2);
/// this yields "1", if any of the vector's elements satisfies
/// the given predicateFunction, and "0" otherwise
template <typename T> int _anyPredicate(
	const std::vector<T> &v, int (*predicateFunction)( T arg ));
/// addition of scalars
template <typename T> T _addScalars( const T& x, const T& y );
/// multiplication of scalars
template <typename T> T _scaleScalar( const T lambda, const T x );
/// sum of a vector's elements
template <typename T> T _sumVector( const std::vector<T> &v );

#endif // _CGSOLVER_H_

