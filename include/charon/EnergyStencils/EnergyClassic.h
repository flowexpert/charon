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
/** \file EnergyClassic.h
 *  Declaration of the parameter class EnergyClassic.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 30.08.2011
 */

#ifndef _ENERGYCLASSIC_H_
#define _ENERGYCLASSIC_H_

#ifdef _MSC_VER
#ifdef energyclassic_EXPORTS
/// Visual C++ specific code
#define energyclassic_DECLDIR __declspec(dllexport)
#else
#define energyclassic_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define energyclassic_DECLDIR
#endif

#include <charon/EnergyStencil.h>

#include <CImg.h>

/// EnergyStencil for so called classic regularization
/** EnergyStencil for so called classic regularization.
 */
template <typename T>
class energyclassic_DECLDIR EnergyClassic :
public EnergyStencil<T> {
 public:

  /// default constructor
  /// \param name          Instance name
  EnergyClassic(const std::string& name = "");

  /// stencil's main function
  void execute();

  /// function yielding stencil's energy for given parameter vector
  std::vector<T> getEnergy(
                      std::vector<T> parameterVector,
                      int pSize,
                      int pWidth, int pHeight, int pDepth,
                      int pSpectrum );

  /// function yielding stencil's energy gradient for given parameter vector
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
  ~EnergyClassic();
};

#endif // _ENERGYCLASSIC_H_

