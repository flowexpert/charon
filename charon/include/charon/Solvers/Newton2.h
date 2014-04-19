/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file Newton2.h
 *  Declaration of the parameter class Newton2.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 06.02.2012
 */

#ifndef _NEWTON2_H_
#define _NEWTON2_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef newton2_EXPORTS
/// dll import/export
#define newton2_DECLDIR __declspec(dllexport)
#else
#define newton2_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// no dll handling on gcc
#define newton2_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

#include <charon/Stencil.h>
#include <charon-utils/CImg.h>
#include <charon-utils/Roi.h>

/// Second order Newton method
/** This solver is based upon the second order Newton method.
 *  Hessian Stencils are needed.
 *  \f[x_{n+1} = x_n-\gamma\left[\mathbf{H} E\right]^{-1}\cdot\nabla E\f]
 *
 *  \ingroup charon-flow-solvers
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class newton2_DECLDIR Newton2 : public TemplatedParameteredObject<T>
{
public:
	/// Input slot for Stencils
	InputSlot< Stencil::Base<T>* > stencils;

	/// Input slot for region of interest
	InputSlot< Roi<int>* > roi;

	/// Output slot for feedback
	OutputSlot< cimg_library::CImgList<T> > result;

	/// Output slot for residual
	OutputSlot< cimg_library::CImgList<T> > residual;

	/// default constructor
	Newton2(const std::string& name = "" /**[in] instance name*/);

	/// main function
	void execute();
};

#endif // _NEWTON2_H_

