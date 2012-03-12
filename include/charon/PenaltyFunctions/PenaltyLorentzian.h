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
/** \file PenaltyLorentzian.h
 *  Declaration of the parameter class PenaltyLorentzian.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 25.01.2012
 */

#ifndef _PENALTYLORENTZIAN_H_
#define _PENALTYLORENTZIAN_H_

#ifdef _MSC_VER
#ifdef penaltylorentzian_EXPORTS
/// Visual C++ specific code
#define penaltylorentzian_DECLDIR __declspec(dllexport)
#else
#define penaltylorentzian_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define penaltylorentzian_DECLDIR
#endif

#include <charon/PenaltyFunction.h>

/// Lorentzian PenaltyFunction
/** Penalty function yielding the Lorentzian penalty.
 *
 *  It yields log( 1 + (x^2 / 2*sigma^2) ) wrt to its argument x
 *  and its gradient (x/sigma^2) / (1 + (x^2 / 2*sigma^2)) .
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class penaltylorentzian_DECLDIR PenaltyLorentzian :
public PenaltyFunction<T> {
 public:
  /// default constructor
  /// \param name          Instance name
  PenaltyLorentzian(const std::string& name = "");

  /// main function
  void execute();

  /// penalty
  T getPenalty( T );

  /// penalty's 1st order derivative (gradient ;-)) wrt its argument
  T getPenaltyGradient( T );

  /// penalty's 2nd order derivative (Hessian ;-)) wrt its argument
  T getPenaltyHessian( T );

  /// parameter sigma
  Parameter< T > sigma;

private:
  /// destructor
  ~PenaltyLorentzian();

  T _lamb;
  T _sigma;
};

#endif // _PENALTYLORENTZIAN_H_

