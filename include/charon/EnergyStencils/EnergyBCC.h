/*  Copyright (C) 2011 Michael Baron

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
/** \file EnergyBCC.h
 *  Declaration of the parameter class EnergyBCC.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 30.08.2011
 */

#ifndef _ENERGYBCC_H_
#define _ENERGYBCC_H_

#ifdef _MSC_VER
#ifdef energybcc_EXPORTS
/// Visual C++ specific code
#define energybcc_DECLDIR __declspec(dllexport)
#else
#define energybcc_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define energybcc_DECLDIR
#endif

#include <charon/EnergyStencil.h>

#include <CImg.h>

/// EnergyStencil for Brightness Constancy Constraint
/** EnergyStencil for Brightness Constancy Constraint.
 *
 *  It implements the energy ( I_x * u + I_y * v + I_t )^2
 *  and its gradients wrt u/v
 *    2 * I_x * ( I_x * u + I_y * v + I_t )
 *    2 * I_y * ( I_x * u + I_y * v + I_t )
 */
template <typename T>
class energybcc_DECLDIR EnergyBCC :
public EnergyStencil<T> {
 public:
  /// Input slot for image derivative wrt x
  InputSlot< cimg_library::CImgList<T> > img_dx;  //  I_x
  /// Input slot for image derivative wrt y
  InputSlot< cimg_library::CImgList<T> > img_dy;  //  I_y
  /// Input slot for image derivative wrt t
  InputSlot< cimg_library::CImgList<T> > img_dt;  //  I_t

  /// default constructor
  /// \param name          Instance name
  EnergyBCC(const std::string& name = "");

  /// stencil's main function
  void execute();

  /// stencil's energy function
  std::vector<T> getEnergy(
                      std::vector<T> parameterVector,
                      int pSize,
                      int pWidth, int pHeight, int pDepth,
                      int pSpectrum );

  /// stencil's energy gradient function
  std::vector<T> getEnergyGradient(
                      std::vector<T> parameterVector,
                      int pSize,
                      int pWidth, int pHeight, int pDepth,
                      int pSpectrum );

private:
  /// linear index conversion
  int _linearIndex( int n, int x, int y, int z, int c,
                    int pSize, int pWidth, int pHeight, int pDepth, int pSpectrum );

  /// destructor
  ~EnergyBCC();
};

#endif // _ENERGYBCC_H_

