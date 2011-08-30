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
/** @file CGMinimize.h
 *  Conjugate Gradient minimization for differentiable multivariate functions.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  @date 30.08.2011
 */
/**
 *  The minimization function was ported from MATLAB-Code
 *  <a href="http://www.cs.toronto.edu/~hinton/csc2515/assignments/minimize.m">minimize.m</a>, Copyright (C) Carl Edward Rasmussen.
 *  A Python port can be found at
 *  <a href="http://www.cs.toronto.edu/~rfm/code/minimize.py">minimize.py</a>, Copyright (C) Roland Memisevic.
 */

#include <cmath>
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

#include <CImg.h>
using namespace cimg_library;

#include <charon/EnergyStencil.h>

#define CG_DIRECTION_PR

//////////////////////////////////////////////////////////////////////

template <typename T>
vector<T> _scaleVector( const T &skalar, const vector<T> &vektor )
{
  vector<T> out( vektor.size() );
  typename vector<T>::const_iterator vit;
  typename vector<T>::iterator oit;
  for (vit = vektor.begin(), oit = out.begin();
       (vit != vektor.end()) ;
       ++vit, ++oit) {
    *oit = skalar * (*vit);
  }
  return out;
}

template <typename T>
vector<T> _addVectors( const vector<T> &v1, const vector<T> &v2 )
{
  vector<T> out( v1.size() );
  typename vector<T>::const_iterator v1it;
  typename vector<T>::const_iterator v2it;
  typename vector<T>::iterator oit;
  for (v1it = v1.begin(), v2it = v2.begin(), oit = out.begin();
       (v1it != v1.end()) && (v2it != v2.end());
       ++v1it, ++v2it, ++oit) {
    *oit = (*v1it) + (*v2it);
  }
  return out;
}

template <typename T>
T _dotProduct( const vector<T> &v1, const vector<T> &v2 )
{
  T ret = 0;

  typename vector<T>::const_iterator iv1, iv2;
  for (  iv1 =  v1.begin(),   iv2 =  v2.begin();
	 (iv1 != v1.end()) && (iv2 != v2.end());
	 ++iv1,               ++iv2               )
    {
      ret += (*iv1) * (*iv2);
    }

  return ret;
}

template <typename T>
int _anyPredicate( const vector<T> &v, int (*predicateFunction)( T arg ) )
{
  int ret = 0;
  typename vector<T>::const_iterator iv;
  for (  iv =  v.begin();
	 iv != v.end();
	 ++iv) {
    if (predicateFunction(*iv)) {
      ret = 1;
    }
  }
  return ret;
}

template <typename T>
int _isnanPlusIsinf( T x )
{
  return (isnan(x) + isinf(x));
}

template <typename T>
T _addScalars( const T &x, const T &y )
{
  return x + y;
}

template <typename T>
T _scaleScalar( const T lambda, const T x )
{
  return lambda * x;
}

//////////////////////////////////////////////////////////////////////

template <typename T>
void CGSolver<T>::minimize(
	      vector<T> &X,
	      vector<T> &startingPoint_X,
              typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator &stencilsBegin,
              typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator &stencilsEnd,
              int length
	      )
{
  int energySize = _pSize * _pWidth*_pHeight*_pDepth*_pSpectrum;
  int energyGradientSize = energySize;

  typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator sIt;
  EnergyStencil<T> *is;

  X = startingPoint_X;

  T red = 1.0;

  T REALMIN = 1e-16;
  T INT = 0.01;
  T EXT = 3.0;
  int const MAX = 20;
  int const RATIO = 10;
  T SIG = 0.01;
  T RHO = SIG/2;

  //////////////////////////////////////////////////////////////////////

  int i = 0;
  int ls_failed = 0;

  vector<T> vec_f0( energySize, T(0.0) );
  vector<T> df0( energyGradientSize, T(0.0) );
  for (sIt = stencilsBegin; sIt != stencilsEnd; ++sIt) {
    is = (*((InputSlot<EnergyStencil<T>*>*)*sIt))();
    vec_f0 = _addVectors( vec_f0, is->getEnergy( X, _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
    df0    = _addVectors( df0, is->getEnergyGradient( X, _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
  }
  T f0 = std::accumulate( vec_f0.begin(), vec_f0.end(), T(0) );

  i = i + (length<0);

  //////////     s = -df0
  vector<T> s = _scaleVector(T(-1.0), df0);

  //////////     d0 = -s' * s     //////////
  T d0 = _scaleScalar( T(-1.0), _dotProduct( s, s ) );

  T x3 = red/(T(1.0)-d0);


//////////     PART 1


  vector<T> X0;
  T F0;
  vector<T> dF0;
  int M;
  while (i < abs(length)) {

    i = i + (length>0);  //  i increments if length > 0

    X0 = X;    F0 = f0; dF0 = df0;

    if (length > 0) { 
      M = MAX;
    } else {
      M = min( MAX, -length-i );
    }

    T A;
    T B;
    T x1, x2, x4;
    T d1, d2, d3, d4;
    T f1, f2, f3, f4;
    vector<T> df1, df2, df3, df4;
    vector<T> vec_f3;

    int success;
    while (1) {
      x2 = 0.0;  f2 = f0;  d2 = d0;  df2 = df0;  f3 = f0;  df3 = df0;
      success = 0;

      while ((!success) && (M > 0)) {
	try {
	  M = M-1; i = i + (length<0);

	  df3 = vector<T>( energyGradientSize, T(0.0) );
	  vec_f3 = vector<T>( energySize, T(0.0) );
          for (sIt = stencilsBegin; sIt != stencilsEnd; ++sIt) {
            is = (*((InputSlot<EnergyStencil<T>*>*)*sIt))();
	    vec_f3 = _addVectors( vec_f3, is->getEnergy( _addVectors( X, _scaleVector( x3, s ) ), _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
	    df3    = _addVectors( df3, is->getEnergyGradient( _addVectors( X, _scaleVector( x3, s ) ), _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
          }
          f3 = std::accumulate( vec_f3.begin(), vec_f3.end(), T(0.0) );

	  if (isnan(f3) || isinf(f3)
	      || _anyPredicate( df3, _isnanPlusIsinf ) ) {
	    throw;
	  }
	  success = 1;
	} catch(...) {
	  x3 = T(0.5) * (x2+x3);
	}
      }

      if (f3 < F0) {
	X0 = _addVectors( X, _scaleVector( x3, s ) );
	F0 = f3;
	dF0 = df3;
      }

      //  d3 = df3'*s;
      d3 = _dotProduct( df3, s );

      if (   (d3 > SIG*d0)
	  || (f3 > f0 + x3*RHO*d0)
	  || (M == 0)) {
	break;
      }

      x1 = x2; f1 = f2; d1 = d2; df1 = df2;
      x2 = x3; f2 = f3; d2 = d3; df2 = df3;

      // A = 6 * (f1-f2) + 3 * (d2+d1)   * (x2-x1);
      A = T(6.0) * (f1-f2)
	+ T(3.0) * (d2+d1) * (x2-x1);

      // B = 3 * (f2-f1) -     (2*d1+d2) * (x2-x1);
      B = T(3.0) * (f2-f1)
        - (T(2.0) * d1+d2) * (x2-x1);

      x3 = x1-d1*((x2-x1)*(x2-x1))
	 / (B + sqrt(B*B - A*d1*(x2-x1)));

      //      if (!isreal(x3) || _isnan(x3) || _isinf(x3) || (x3 < 0)) {

      // if (!_isreal(x3))
      if (B*B - A*d1*(x2-x1) < 0) {
	x3 = x2 * EXT;
	continue;
      }

      if ( isnan(x3) || isinf(x3) || (x3 < T(0.0))) {
	x3 = x2 * EXT;
      } else if (x3 > x2 * EXT) {
	x3 = x2 * EXT;
      } else if (x3 < x2 + INT*(x2-x1)) {
	x3 = x2 + INT * (x2-x1);
      }
    }  //  while


//////////     PART 2


    while (
          (
             abs(d3) > T(-1.0) * SIG*d0
	  ||       f3 > f0 + x3*RHO*d0
          )
	  &&
          (
             M > 0
          )
          ) {

      if ( (d3 > T(0.0)) || (f3 > f0 + x3*RHO*d0) ) {
	x4 = x3; f4 = f3; d4 = d3; df4 = df3;
      } else {
	x2 = x3; f2 = f3; d2 = d3; df2 = df3;
      }

      if (f4 > f0) {
	x3 = x2 - (T(0.5)*d2*(x4-x2)*(x4-x2)) / (f4-f2-d2*(x4-x2));
      } else {
	// A = 6 * (f2-f4) / (x4-x2) + 3 * (d4+d2);
	A = T(6.0) * (f2-f4) / (x4-x2)
	  + T(3.0) * (d4+d2);
	// B = 3 * (f4-f2) - (2*d2+d4) * (x4 - x2);
	B = T(3.0) * (f4-f2)
	  - (T(2.0) * d2+d4) * (x4 - x2);
	x3 = x2 + (sqrt(B*B - A*d2*(x4-x2)*(x4-x2)) - B) / A;
      }

      if (isnan(x3) || isinf(x3)) {
	x3 = T(0.5) * (x2+x4);
      }

      x3 = max(min(x3, x4-INT*(x4-x2)), x2+INT*(x4-x2));

      df3 = vector<T>( energyGradientSize, T(0.0) );
      vec_f3 = vector<T>( energySize, T(0.0) );
      for (sIt = stencilsBegin; sIt != stencilsEnd; ++sIt) {
        is = (*((InputSlot<EnergyStencil<T>*>*)*sIt))();
	vec_f3 = _addVectors( vec_f3, is->getEnergy( _addVectors( X, _scaleVector( x3, s ) ), _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
	df3    = _addVectors( df3, is->getEnergyGradient( _addVectors( X, _scaleVector( x3, s ) ), _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
      }
      f3 = std::accumulate( vec_f3.begin(), vec_f3.end(), T(0.0) );

      if (f3 < F0) {
	X0 = _addVectors( X, _scaleVector( x3, s ) );
	F0 = f3;
	dF0 = df3;
      }

      M = M-1; i = i + (length<0);

      d3 = _dotProduct( df3, s );
    }  //  while


//////////     PART 3


    if (
	   abs(d3) < -SIG*d0
       &&
           f3 < f0+x3*RHO*d0
       ) {
      X = _addVectors( X, _scaleVector( x3, s ) );
	
      f0 = f3;


// Fletcher-Reeves
#ifdef CG_DIRECTION_FR
      s = _addVectors(
            _scaleVector(
              _dotProduct(df3, df3) / _dotProduct(df0, df0),
              s
            ),
            _scaleVector( T(-1.0), df3 )
          );
#endif

// Polack-Ribiere
#ifdef CG_DIRECTION_PR
      s = _addVectors(
	    _scaleVector(
	      _addScalars(
	        _dotProduct(df3, df3),
	        _scaleScalar( T(-1.0),
	                      _dotProduct(df0, df3)
                )
              )
	      /
              _dotProduct(df0, df0),
              s
            ),
	    _scaleVector( T(-1.0), df3 )
          );
#endif

      df0 = df3;

      d3 = d0; d0 = _dotProduct( df0, s );

      if (d0 > T(0.0) ) {
	s  = _scaleVector(T(-1.0), df0);
	d0 = T(-1.0) * _dotProduct(s,s);
      }

      x3 = x3 * min( T(RATIO), _scaleScalar( 1/(d0-REALMIN), d3 ) );

      ls_failed = 0;
    } else {
      X = X0; f0 = F0; df0 = dF0;

      if (ls_failed || i > abs(length)) break;

      s = _scaleVector(T(-1.0), df0);
      d0 = T(-1.0) * _dotProduct(s,s);

      x3 = 1/(T(1.0)-d0);
      ls_failed = 1;
    }

    sout << "(II) CGSolver iteration, energy = ";
    sout << i << ", " << f3 << std::endl;

  }  //  while (i < abs(length))

  sout << "(II) CGSolver " << i << " iterations performed" << std::endl;

}
