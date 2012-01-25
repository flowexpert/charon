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
/** \file PenaltyCharbonnier.h
 *  Declaration of the parameter class PenaltyCharbonnier.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 25.01.2012
 */

#ifndef _PENALTYCHARBONNIER_H_
#define _PENALTYCHARBONNIER_H_

#ifdef _MSC_VER
#ifdef penaltycharbonnier_EXPORTS
/// Visual C++ specific code
#define penaltycharbonnier_DECLDIR __declspec(dllexport)
#else
#define penaltycharbonnier_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define penaltycharbonnier_DECLDIR
#endif

#include <charon/PenaltyFunction.h>

/// Charbonnier PenaltyFunction
/** Penalty function yielding the Charbonnier penalty.
 *
 *  It yields ( x ^ 2 + \eps ^ 2 ) ^ a wrt to its argument x
 *  and its gradient 2 * x * (x^2 + \eps^2) ^ a-1 .
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class penaltycharbonnier_DECLDIR PenaltyCharbonnier :
public PenaltyFunction<T> {
 public:
  /// default constructor
  /// \param name          Instance name
  PenaltyCharbonnier(const std::string& name = "");

  /// main function
  void execute();

  /// penalty function
  T getPenalty( T );

  /// penalty derivative (gradient ;-)) wrt its argument
  T getPenaltyGradient( T );

  /// parameter a
  Parameter< T > a;

  /// parameter epsilon
  Parameter< T > eps;

private:
  /// destructor
  ~PenaltyCharbonnier();

  T _lamb;
  T _a, _eps;
};

#endif // _PENALTYCHARBONNIER_H_

