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
/** @file CGSolver.hxx
 *  Implementation of class CGSolver.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  @date 30.08.2011
 */

#ifndef _CGSOLVER_HXX_
#define _CGSOLVER_HXX_

#include "CGSolver.h"

#include <charon-core/ParameteredObject.hxx>
#include <cmath>
#include <vector>
#include <numeric>

#define CG_DIRECTION_PR

#ifdef _MSC_VER
	#define isnan(x) _isnan(x)
	#define isinf(x) !_finite(x)
#endif

template <typename T>
CGSolver<T>::CGSolver(const std::string& name) : 
		TemplatedParameteredObject<T>("CGSolver", name),
                energyStencils(false,true)
{
	this->_addInputSlot(energyStencils,
	                    "energyStencils",
	                    "energy stencils",
	                    "EnergyStencil<T>*");
	this->_addInputSlot(initialParameters,
	                    "initialParameters",
	                    "initial parameters",
	                    "CImgList<T>");
        this->_addOutputSlot(optimizedParameters,
                             "optimizedParameters",
                             "optimized parameters",
                             "CImgList<T>");
        this->_addParameter(length,
	                    "length",
	                    "see minimize.m for details", 10);
}

template <typename T>
CGSolver<T>::~CGSolver() {
}

template <typename T>
void CGSolver<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	optimizedParameters() = cimg_library::CImgList<T>( initialParameters() );

	_pSize     = initialParameters().size();
	_pWidth    = initialParameters()[0].width();
	_pHeight   = initialParameters()[0].height();
	_pDepth    = initialParameters()[0].depth();
	_pSpectrum = initialParameters()[0].spectrum();

	int vectorSize = _pSize * _pWidth * _pHeight * _pDepth * _pSpectrum;

	std::vector<T> _initialParameterVector( vectorSize, T(0.0) );
	std::vector<T> _optimizedParameterVector( vectorSize, T(0.0) );

	int idx = 0;
	int n, x, y, z, c;
	for (n=0; n<_pSize; ++n)
	for (x=0; x<_pWidth; ++x)
	for (y=0; y<_pHeight; ++y)
	for (z=0; z<_pDepth; ++z)
	for (c=0; c<_pSpectrum; ++c) {
		_initialParameterVector[idx] = T(
		    initialParameters().atNXYZC( n, x, y, z, c ) );
		++idx;
	}

	typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator _energyStencilsBegin;
	typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator _energyStencilsEnd;
	_energyStencilsBegin = this->energyStencils.begin();
	_energyStencilsEnd   = this->energyStencils.end();

	minimize(
	    _optimizedParameterVector,
	    _initialParameterVector,
	    _energyStencilsBegin,
	    _energyStencilsEnd,
            this->length()
        );

	idx = 0;
	for (n=0; n<_pSize; ++n)
	for (x=0; x<_pWidth; ++x)
	for (y=0; y<_pHeight; ++y)
	for (z=0; z<_pDepth; ++z)
	for (c=0; c<_pSpectrum; ++c) {
		optimizedParameters().atNXYZC( n, x, y, z, c ) =
		    _optimizedParameterVector[idx];
		++idx;
	}

}

template <typename T>
std::vector<T> CGSolver<T>::_scaleVector( const T &skalar, const std::vector<T> &vektor )
{
  std::vector<T> out( vektor.size() );
  typename std::vector<T>::const_iterator vit;
  typename std::vector<T>::iterator oit;
  for (vit = vektor.begin(), oit = out.begin();
       (vit != vektor.end()) ;
       ++vit, ++oit) {
    *oit = skalar * (*vit);
  }
  return out;
}

template <typename T>
std::vector<T> CGSolver<T>::_addVectors( const std::vector<T> &v1, const std::vector<T> &v2 )
{
  std::vector<T> out( v1.size() );
  typename std::vector<T>::const_iterator v1it;
  typename std::vector<T>::const_iterator v2it;
  typename std::vector<T>::iterator oit;
  for (v1it = v1.begin(), v2it = v2.begin(), oit = out.begin();
       (v1it != v1.end()) && (v2it != v2.end());
       ++v1it, ++v2it, ++oit) {
    *oit = (*v1it) + (*v2it);
  }
  return out;
}

template <typename T>
T CGSolver<T>::_dotProduct( const std::vector<T> &v1, const std::vector<T> &v2 )
{
  T ret = 0;

  typename std::vector<T>::const_iterator iv1, iv2;
  for (  iv1 =  v1.begin(),   iv2 =  v2.begin();
         (iv1 != v1.end()) && (iv2 != v2.end());
         ++iv1,               ++iv2               )
    {
      ret += (*iv1) * (*iv2);
    }

  return ret;
}

template <typename T>
int _isnanPlusIsinf( T x )
{
  return (isnan(x) + isinf(x));
}

template <typename T>
int CGSolver<T>::_anyPredicate( const std::vector<T> &v, int (*predicateFunction)( T arg ) )
{
  int ret = 0;
  typename std::vector<T>::const_iterator iv;
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
T CGSolver<T>::_addScalars( const T &x, const T &y )
{
  return x + y;
}

template <typename T>
T CGSolver<T>::_scaleScalar( const T lambda, const T x )
{
  return lambda * x;
}

template <typename T>
T CGSolver<T>::_sumVector( const std::vector<T> &v )
{
  T ret = T(0.0);
  typename std::vector<T>::const_iterator iv;
  for (  iv =  v.begin();
         iv != v.end();
         ++iv) {
    ret += (*iv);
  }
  return ret;
}

template <typename T>
void CGSolver<T>::minimize(
	      std::vector<T> &X,
	      std::vector<T> &startingPoint_X,
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
  T REALMIN = 1e-16f;
  T INT = 0.01f;
  T EXT = 3.0f;
  int const MAX = 20;
  int const RATIO = 10;
  T SIG = 0.01f;
  T RHO = SIG/2;
 
  //////////////////////////////////////////////////////////////////////

  int i = 0;
  int ls_failed = 0;

  std::vector<T> vec_f0( energySize, T(0.0) );
  std::vector<T> df0( energyGradientSize, T(0.0) );
  for (sIt = stencilsBegin; sIt != stencilsEnd; ++sIt) {
    is = (*((InputSlot<EnergyStencil<T>*>*)*sIt))();
    vec_f0 = _addVectors( vec_f0, is->getEnergy( X, _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
    df0    = _addVectors( df0, is->getEnergyGradient( X, _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
  }
  T f0 = _sumVector( vec_f0 );
//  T f0 = std::accumulate( vec_f0.begin(), vec_f0.end(), T(0) );

  i = i + (length<0);

  //////////     s = -df0
  std::vector<T> s = _scaleVector(T(-1.0), df0);

  //////////     d0 = -s' * s     //////////
  T d0 = _scaleScalar( T(-1.0), _dotProduct( s, s ) );

  T x3 = red/(T(1.0)-d0);
//  std::cout << "[1] x3 = " << x3 << std::endl;

//////////     PART 1


  std::vector<T> X0;
  T F0;
  std::vector<T> dF0;
  int M;
  while (i < std::abs(length)) {

    i = i + (length>0);  //  i increments if length > 0

    X0 = X;    F0 = f0; dF0 = df0;

    if (length > 0) { 
      M = MAX;
    } else {
      M = std::min( MAX, -length-i );
    }

    T A;
    T B;
    T x1, x2, x4;
    T d1, d2, d3, d4;
    T f1, f2, f3, f4;
    std::vector<T> df1, df2, df3, df4;
    std::vector<T> vec_f3;

    int success;
    while (1) {
      x2 = 0.0;  f2 = f0;  d2 = d0;  df2 = df0;  f3 = f0;  df3 = df0;
      success = 0;

      while ((!success) && (M > 0)) {
	try {
	  M = M-1; i = i + (length<0);

//          std::cout << "[2] x3 = " << x3 << std::endl;

	  df3 = std::vector<T>( energyGradientSize, T(0.0) );
	  vec_f3 = std::vector<T>( energySize, T(0.0) );
          for (sIt = stencilsBegin; sIt != stencilsEnd; ++sIt) {
            is = (*((InputSlot<EnergyStencil<T>*>*)*sIt))();
	    vec_f3 = _addVectors( vec_f3, is->getEnergy( _addVectors( X, _scaleVector( x3, s ) ), _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
	    df3    = _addVectors( df3, is->getEnergyGradient( _addVectors( X, _scaleVector( x3, s ) ), _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
          }
          f3 = _sumVector( vec_f3 );
//          f3 = std::accumulate( vec_f3.begin(), vec_f3.end(), T(0.0) );

	  if (isnan(f3) || isinf(f3)
	      || _anyPredicate( df3, _isnanPlusIsinf<T> ) ) {
	    throw;
	  }
	  success = 1;
	} catch(...) {
	  x3 = T(0.5) * (x2+x3);
	}
      }

//      std::cout << "(f3 < F0) = " << f3 << " " << F0 << std::endl;  //  (!!) this does not work for CGSolver<float>
      if (f3 < F0) {
//        std::cout << "[3] x3 = " << x3 << std::endl;
	X0 = _addVectors( X, _scaleVector( x3, s ) );
	F0 = f3;
	dF0 = df3;
      }

      //  d3 = df3'*s;
      d3 = _dotProduct( df3, s );
//      std::cout << "O df3[1000]    = " << df3[1000] << "    ";
//      std::cout << "O s[1000]      = " << s[1000] << std::endl;
//      std::cout << "O d3 = df3 * s = " << d3 << std::endl;

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
	 / (B + sqrt(double(B*B - A*d1*(x2-x1))));

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
             std::abs(d3) > T(-1.0) * SIG*d0
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
	x3 = x2 + (sqrt(double(B*B - A*d2*(x4-x2)*(x4-x2))) - B) / A;
      }

      if (isnan(x3) || isinf(x3)) {
	x3 = T(0.5) * (x2+x4);
      }

      x3 = std::max(std::min(x3, x4-INT*(x4-x2)), x2+INT*(x4-x2));

      df3 = std::vector<T>( energyGradientSize, T(0.0) );
      vec_f3 = std::vector<T>( energySize, T(0.0) );
      for (sIt = stencilsBegin; sIt != stencilsEnd; ++sIt) {
        is = (*((InputSlot<EnergyStencil<T>*>*)*sIt))();
	vec_f3 = _addVectors( vec_f3, is->getEnergy( _addVectors( X, _scaleVector( x3, s ) ), _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
	df3    = _addVectors( df3, is->getEnergyGradient( _addVectors( X, _scaleVector( x3, s ) ), _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum ) );
      }
      f3 = _sumVector( vec_f3 );
//      f3 = std::accumulate( vec_f3.begin(), vec_f3.end(), T(0.0) );

      if (f3 < F0) {
	X0 = _addVectors( X, _scaleVector( x3, s ) );
	F0 = f3;
	dF0 = df3;
      }

      M = M-1; i = i + (length<0);

      d3 = _dotProduct( df3, s );

    }  //  while


//////////     PART 3

//    std::cout << "abs(d3) < -SIG*d0 ? " << (std::abs(d3) < -SIG*d0) << std::endl << std::endl;
//    std::cout << "d3      = " << d3 << std::endl;
//    std::cout << "abs(d3) = " << std::abs(d3) << std::endl;
//    std::cout << "SIG     = " << SIG << std::endl;
//    std::cout << "d0      = " << d0 << std::endl;
//    std::cout << "-SIG*d0 = " << -SIG*d0 << std::endl;



    if (
	   std::abs(d3) < -SIG*d0
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

      x3 = x3 * std::min( T(RATIO), _scaleScalar( 1/(d0-REALMIN), d3 ) );

      ls_failed = 0;
    } else {
      X = X0; f0 = F0; df0 = dF0;

      if (ls_failed || i > std::abs(length)) break;

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

#endif // _CGSOLVER_HXX_

