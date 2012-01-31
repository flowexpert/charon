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
/** \file EnergyCoupling.h
 *  Declaration of the parameter class EnergyCoupling.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 24.11.2011
 */

#ifndef _ENERGYCOUPLING_H_
#define _ENERGYCOUPLING_H_

#ifdef _MSC_VER
#ifdef energycoupling_EXPORTS
/// Visual C++ specific code
#define energycoupling_DECLDIR __declspec(dllexport)
#else
#define energycoupling_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define energycoupling_DECLDIR
#endif

#include <charon/AbstractStencil.h>

#include <CImg.h>
#include <vector>

/// EnergyStencil for coupling two flow fields.
/** EnergyStencil for the coupling term from
 *  "Large Displacement Optical Flow Without Warping"
 *  (Steinbr"ucker, Pock, Cremers ; ICCV 2009)
 *
 *  It implements the energy (1/2T) * ( u - v )^2
 *  and its gradients wrt both components of the
 *  first flow field u.
 *
 *  The temperature is chosen wrt the current iteration (tempLevel)
 *  and is hardcoded.
 *
 *  \ingroup charon-stencils
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class energycoupling_DECLDIR EnergyCoupling :
	public virtual AbstractStencil::Base<T>,
	public AbstractStencil::Energy<T>,
	public AbstractStencil::EnergyGradient<T>
{
 public:
  /// Input slot for first flow field
  InputSlot< cimg_library::CImgList<T> > firstMotionUV;
  /// Input slot for second flow field
  InputSlot< cimg_library::CImgList<T> > secondMotionUV;

  /// Input slot for current iteration number.
//  InputSlot< unsigned int > tempLevel;
  Parameter< T > temp;

  /// default constructor
  /// \param name          Instance name
  EnergyCoupling(const std::string& name = "");

  /// stencil's main function
  void execute();

  /// stencil's energy function
  T getEnergy( int nI, int xI, int yI, int zI, int cI );

  /// stencil's energy gradient function
  std::vector<T> getEnergyGradient( int nI, int xI, int yI, int zI, int cI );

  /// stencil's count of gradient components
  int getEnergyGradientDimensions();

private:
  /// destructor
  ~EnergyCoupling();

  T _lamb;
};

#endif // _ENERGYCOUPLING_H_

