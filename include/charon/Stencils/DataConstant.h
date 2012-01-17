/*  Copyright (C) 2010 Jens-Malte Gottfried

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
/** \file DataConstant.h
 *  Implementation of class DataConstant.
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de>
 *      Jens-Malte Gottfried </a>
 *  \date 2010-01-18
 */

#ifndef _DATA_CONSTANT_H_
#define _DATA_CONSTANT_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef dataconstant_EXPORTS
///Visual C++ specific code
#define dataconstant_DECLDIR __declspec(dllexport)
#else
#define dataconstant_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define dataconstant_DECLDIR
#endif

#include <charon-utils/Interpolator.h>
#include <charon/Stencil.h>

class Function;

/// Brightness constancy data term
/**
 *  This implements the BC data term from the learning optical flow
 *  paper by Sun et. al.
 *
 *  The data term energy looks like
 *
 *  \f[E_D=\sum_{i,j}\Psi(I_1(i,j)-I_2(i+u_{ij},j+v_{ij}))\f]
 *
 *  It is a general stencil wich is able to model various kinds of constancy
 *  and functions \f$\Psi\f$ for local weighting of the bcce data term.
 *  With preprocessing of the sequence data, other kinds of constancy
 *  like gradient constancy may be realized (providing filtered versions
 *  of the sequence as input to DataConstant).
 *
 *  The stencil equations are derived using
 *  \f$\nabla_u E_D=0,\nabla_v E_D=0\f$:
 *  \f[\nabla_u E_D=\Psi'(I_1(i,j)-I_2(i+u_{ij},j+v_{ij}))
		\cdot(-I_{2,x}(i+u_{ij},j+v_{ij}))\f]
 *  Expanding this by the argument of \f$\Psi'\f$ leads to
	\f[\nabla_u E_D=\frac{\Psi'(I_1(i,j)-I_2(i+u_{ij},j+v_{ij}))
		\cdot(-I_{2,x}(i+u_{ij},j+v_{ij}))}{I_2(i+u_{ij},j+v_{ij})-I_1(i,j)}
		\cdot(I_2(i+u_{ij},j+v_{ij})-I_1(i,j))\f]
 *  Taylor-expansion of the right part is the usual brighness constancy
 *  constraint equation (BCCE):
 *  \f[I_2(i+u_{ij},j+v_{ij})-I_1(i,j)\approx
		I_x(i,j)\Delta u_{ij}+I_y(i,j)\Delta v_{ij}+I_t(i,j)\f]
 *  \f$\nabla_u E_D\f$ is now linearized by holding the first part fixed and
 *  using the BCCE taylor-expansion instead of the right part.
 *  Additionally \f$I_{2,x}(i+u_{ij},j+v_{ij})\f$ may be approximated
 *  by \f$I_x(i,j)\f$:
 *  \f[\nabla_u E_D\approx\frac{\Psi'(-I_t(i,j))\cdot(-I_x(i,j))}{I_t(i,j)}
		\cdot (I_x(i,j)*\Delta u_{ij} +I_y(i,j)\Delta v_{ij}+I_t(i,j))\f]
 *  Usually, \f$\Psi\f$ is symmetric (even), so \f$\Psi'\f$ is antisymmetric
 *  (odd) wich may be used to simplify this further:
 *  \f[\nabla_u E_D\approx\frac{\Psi'(I_t(i,j))}{I_t(i,j)} \cdot
		I_x(i,j)(I_x(i,j)*\Delta u_{ij}+I_y(i,j)\Delta v_{ij}+I_t(i,j))\f]
 *  In the end, this is the usual BCCE stencil locally weighted by
 *  \f[w_{ij}=\frac{\Psi'(I_t(i,j))}{I_t(i,j)}\f]
 *
 *  This weight is used, if the option useDiff2Lin is set to true.
 *  Otherwise, the term of \f$\nabla_u E_D\f$ is linearized direclty by
 *  tailer-expansion in \f$u,v\f$ wich leads to the local weights
 *  \f[w_{ij}=\Psi''(I_t(i,j))\f]
 *  In practice, this turns out to perform worse than the linearization
 *  presented above, but both variants are avaliable for testing.
 *
 *  As one may see above, using \f$I_x,I_y\f$ of the first frame is an
 *  approximation, so optionally, setting useImg2 to true, \f$I_x,I_y\f$
 *  of the second image are used for calculations.
 *
 *  \ingroup charon-stencils
 *  \ingroup charon-learning
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <class T>
class dataconstant_DECLDIR DataConstant : public Stencil<T> {
public:
	/// default constructor
	DataConstant(const std::string& name = "" /**[in] instance name*/);

	/// \name Image derivative inputs
	//  \{
	InputSlot<cimg_library::CImgList<T> > it;
	InputSlot<cimg_library::CImgList<T> > ix;
	InputSlot<cimg_library::CImgList<T> > iy;
	//  \}

	/// initial guess of real flow, no zeros, even in warped mode
	InputSlot<cimg_library::CImgList<T> > flowGuess;

	/// brightness constancy energy function
	InputSlot<Function*> psi;

	/// Calculate flow increments
	Parameter<bool> calcDifferences;
	/// Use derivatives \f$I_x,I_y\f$ of Img2 istead of Img1
	Parameter<bool> useImg2;
	/// use linearization of second derivative
	Parameter<bool> useDiff2Lin;

	/// \copybrief Stencil::updateStencil
	/** \copydetails Stencil::updateStencil */
	virtual void updateStencil(
			const std::string& unknown,
			const Point4D<int>& p=Point4D<int>(), const int& v=0);

	/// Calculate weight map
	/** This calculates the local weight values for each pixel.
	 *  The resulting image contains these values and may be considered
	 *  as a weight map.
	 *  \param seq           ignored, only works on \f$I_t\f$
	 *  \param frame         ignored
	 *  \return              local weights \f$w_{ij}\f$ (see above)
	 */
	virtual cimg_library::CImg<T> apply(
			const cimg_library::CImgList<T>& seq,
			const unsigned int frame) const;

	/// calculate energy \f$E_D\f$ of the whole image (pair)
	virtual double getEnergy() const;

protected:
	/// main function
	virtual void execute();
};

#endif //_DATA_CONSTANT_H_
