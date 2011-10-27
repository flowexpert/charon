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
 *  @date 26.10.2011
 */

#ifndef _CGSOLVER_HXX_
#define _CGSOLVER_HXX_

#include "CGSolver.h"

#include <charon-core/ParameteredObject.hxx>
#include <charon/IteratorHelper.hxx>

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
	this->_addInputSlot(itHelper,
	                    "itHelper",
	                    "iterator helper",
	                    "IteratorHelper<T>*");
        this->_addOutputSlot(result,
                             "result",
                             "result",
                             "CImgList<T>");
        this->_addParameter(flowDimensions,
                            "flowDimensions",
                            "flow dimensions", 2);
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

	_help = itHelper();
	_help->reset();  //  (!!)
	cimg_library::CImgList<T> & _itflow = _help->flow();

	_pSize     = _itflow.size();
	_pWidth    = _itflow[0].width();
	_pHeight   = _itflow[0].height();
	_pDepth    = _itflow[0].depth();
	_pSpectrum = _itflow[0].spectrum();

/*
	int vectorSize = _pSize * _pWidth * _pHeight * _pDepth * _pSpectrum;

	std::vector<T> _initialParameterVector( vectorSize, T(0.0) );
	std::vector<T> _optimizedParameterVector( vectorSize, T(0.0) );
*/

//        int energySize = _pWidth * _pHeight * _pDepth * _pSpectrum;
//        int energyGradientSize = _pSize * energySize;
        int energySize = _pSize * _pWidth * _pHeight * _pDepth * _pSpectrum;
        int energyGradientSize = energySize;

        std::vector<T> _initialParameterVector( energySize, T(0.0) );
        std::vector<T> _optimizedParameterVector( energyGradientSize, T(0.0) );

	int idx = 0;
	int n, x, y, z, c;
	for (n=0; n<_pSize; ++n)
	for (x=0; x<_pWidth; ++x)
	for (y=0; y<_pHeight; ++y)
	for (z=0; z<_pDepth; ++z)
	for (c=0; c<_pSpectrum; ++c) {
		_initialParameterVector[idx] = T(
		    _itflow.atNXYZC( n, x, y, z, c ) );
		++idx;
	}

	typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator _energyStencilsBegin;
	typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator _energyStencilsEnd;
	_energyStencilsBegin = this->energyStencils.begin();
	_energyStencilsEnd   = this->energyStencils.end();

	minimize(
	    _itflow,
	    _optimizedParameterVector,
	    _initialParameterVector,
	    _energyStencilsBegin,
	    _energyStencilsEnd,
            this->length()
        );

	_itflow.assign( _reshapeFeedback( _optimizedParameterVector ) );

	result() = _itflow;
}

template <typename T>
cimg_library::CImgList<T> CGSolver<T>::_reshapeFeedback( std::vector<T> v )
{
	cimg_library::CImgList<T> ret( _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum );
        int idx = 0;

	int n, x, y, z, c;
        for (n=0; n<_pSize; ++n)
        for (x=0; x<_pWidth; ++x)
        for (y=0; y<_pHeight; ++y)
        for (z=0; z<_pDepth; ++z)
        for (c=0; c<_pSpectrum; ++c) {
                ret.atNXYZC( n, x, y, z, c ) = v.at(idx);
                ++idx;
        }
	return ret;
}

template <typename T>
std::vector<T> _scaleVector( const T &skalar, const std::vector<T> &vektor )
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
std::vector<T> _addVectors( const std::vector<T> &v1, const std::vector<T> &v2 )
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
T _dotProduct( const std::vector<T> &v1, const std::vector<T> &v2 )
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
int _anyPredicate( const std::vector<T> &v, int (*predicateFunction)( T arg ) )
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
T _addScalars( const T &x, const T &y )
{
  return x + y;
}

template <typename T>
T _scaleScalar( const T lambda, const T x )
{
  return lambda * x;
}

template <typename T>
T _sumVector( const std::vector<T> &v )
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

template <class T>
int CGSolver<T>::_linearIndex( int n, int x, int y, int z, int c,
                               int pSize, int pWidth, int pHeight, int pDepth, int pSpectrum )
{
        int ret;

        if (n < 0) n = 0; if (n > pSize-1) n = pSize-1;
        if (x < 0) x = 0; if (x > pWidth-1) x = pWidth-1;
        if (y < 0) y = 0; if (y > pHeight-1) y = pHeight-1;
        if (z < 0) z = 0; if (z > pDepth-1) z = pDepth-1;
        if (c < 0) c = 0; if (c > pSpectrum-1) c = pSpectrum-1;

        ret = c
            + z * pSpectrum
            + y * pSpectrum * pDepth
            + x * pSpectrum * pDepth * pHeight
            + n * pSpectrum * pDepth * pHeight * pWidth ;

        return ret;
}

template <typename T>
void CGSolver<T>::minimize(
              cimg_library::CImgList<T> &_itflow,
	      std::vector<T> &X,
	      std::vector<T> &startingPoint_X,
              typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator &stencilsBegin,
              typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator &stencilsEnd,
              int length
	      )
{
  int xI, yI, zI;
  typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator sIt;
  EnergyStencil<T> *is;

  int energySize = _pSize * _pWidth*_pHeight*_pDepth*_pSpectrum;
  int energyGradientSize = energySize;

//  std::cout << "_pSize     = " << _pSize << std::endl;
//  std::cout << "_pWidth    = " << _pWidth << std::endl;
//  std::cout << "_pHeight   = " << _pHeight << std::endl;
//  std::cout << "_pDepth    = " << _pDepth << std::endl;
//  std::cout << "_pSpectrum = " << _pSpectrum << std::endl;
//  std::cout << "flowDimensions = " << flowDimensions() << std::endl;

//  int energySize = _pWidth*_pHeight*_pDepth*_pSpectrum;
//  int energyGradientSize = _pSize * energySize;

  X = startingPoint_X;
  _itflow.assign( _reshapeFeedback( X ) );

  T red = 1;
  T REALMIN = 1e-16;
  T INT = 0.01;
  T EXT = 3.0;
  int const MAX = 20;
  int const RATIO = 10;
  T SIG = 0.01;
  T RHO = SIG/2;
 
  //////////////////////////////////////////////////////////////////////

  int i = 0; int gc = 0;
  int ls_failed = 0;

//  std::cout << "(II) CGSolver energyGradientSize = ";
//  std::cout << energyGradientSize << std::endl;

  T energy;
  std::vector<T> energyGradient( energyGradientSize, T(0.0) );
  std::vector<T> localGradient( flowDimensions(), T(0.0) );

//  std::cout << "(II) CGSolver energyGradient.size() = ";
//  std::cout << energyGradient.size() << std::endl;
//  std::cout << "(II) CGSolver energyGradient::max_size() = ";
//  std::cout << energyGradient.max_size() << std::endl;

  T f0 = T(0.0);
  std::vector<T> df0( energyGradientSize, T(0.0) );

//  std::cout << "_itflow.atNXYZC( 0, 100, 100, 0, 0 ) = ";
//  std::cout << _itflow.atNXYZC( 0, 100, 100, 0, 0 ) << std::endl;
  _itflow.assign( _reshapeFeedback( X ) );
//  std::cout << "_itflow.atNXYZC( 0, 100, 100, 0, 0 ) = ";
//  std::cout << _itflow.atNXYZC( 0, 100, 100, 0, 0 ) << std::endl;

  for (sIt = stencilsBegin; sIt != stencilsEnd; ++sIt) {
    is = (*((InputSlot<EnergyStencil<T>*>*)*sIt))();

    energy = T(0.0);
    for (xI = 0; xI < _pWidth; ++xI)
    for (yI = 0; yI < _pHeight; ++yI)
    for (zI = 0; zI < _pDepth; ++zI)
    {
      energy += is->getEnergy( 0, xI, yI, zI, 0 );
      localGradient = is->getEnergyGradient( 0, xI, yI, zI, 0 );

      for (gc = 0; gc < flowDimensions(); ++gc)
        energyGradient.at(_linearIndex( gc, xI, yI, zI, 0, _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum)) = localGradient.at(gc);
    }
    f0  += energy;
    df0 =  _addVectors( df0, energyGradient );
  }

  i = i + (length<0);

  //////////     s = -df0
  std::vector<T> s = _scaleVector(T(-1.0), df0);

  //////////     d0 = -s' * s     //////////
  T d0 = _scaleScalar( T(-1.0), _dotProduct( s, s ) );

  T x3 = red/(T(1.0)-d0);

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
    T x1, x2;
    T d1, d2, d3;
    T f1, f2, f3;
    T x4 = 0;
    T d4 = 0;
    T f4 = 0;
    std::vector<T> df1, df2, df3, df4;

    int success;
    while (1) {
      x2 = 0.0;  f2 = f0;  d2 = d0;  df2 = df0;  f3 = f0;  df3 = df0;
      success = 0;

      while ((!success) && (M > 0)) {
	try {
	  M = M-1; i = i + (length<0);

	  f3 = T(0.0);
	  df3 = std::vector<T>( energyGradientSize, T(0.0) );

  std::cout << "_itflow.atNXYZC( 0, 100, 100, 0, 0 ) = ";
  std::cout << _itflow.atNXYZC( 0, 100, 100, 0, 0 ) << std::endl;
  std::cout << "_itflow.atNXYZC( 0, 50, 50, 0, 0 ) = ";
  std::cout << _itflow.atNXYZC( 0, 50, 50, 0, 0 ) << std::endl;
          _itflow.assign( _reshapeFeedback( _addVectors( X, _scaleVector( x3, s ) ) ) );
  std::cout << "_itflow.atNXYZC( 0, 100, 100, 0, 0 ) = ";
  std::cout << _itflow.atNXYZC( 0, 100, 100, 0, 0 ) << std::endl;
  std::cout << "_itflow.atNXYZC( 0, 50, 50, 0, 0 ) = ";
  std::cout << _itflow.atNXYZC( 0, 50, 50, 0, 0 ) << std::endl;

          for (sIt = stencilsBegin; sIt != stencilsEnd; ++sIt) {
            is = (*((InputSlot<EnergyStencil<T>*>*)*sIt))();

            energy = T(0.0);
            for (xI = 0; xI < _pWidth; ++xI)
            for (yI = 0; yI < _pHeight; ++yI)
            for (zI = 0; zI < _pDepth; ++zI)
            {
              energy += is->getEnergy( 0, xI, yI, zI, 0 );
              localGradient = is->getEnergyGradient( 0, xI, yI, zI, 0 );
              for (gc = 0; gc < flowDimensions(); ++gc)
                energyGradient.at(_linearIndex( gc, xI, yI, zI, 0, _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum)) = localGradient.at(gc);
            }
	    f3  += energy;
	    df3 =  _addVectors( df3, energyGradient );
          }

	  if (isnan(f3) || isinf(f3)
	      || _anyPredicate( df3, _isnanPlusIsinf<T> ) ) {
            std::cout << "(!!) throw !!" << std::endl;
	    throw;
	  }
	  success = 1;
	} catch(...) {
	  x3 = T(0.5) * (x2+x3);
	}
      }

//      std::cout << "(f3 < F0) = " << f3 << " " << F0 << std::endl;  //  (!!) this does not work for CGSolver<float>
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


std::cout << "(II) CGSolver :: d3  @ PART2 = " << d3 << std::endl;
std::cout << "(II) CGSolver :: SIG @ PART2 = " << SIG << std::endl;
std::cout << "(II) CGSolver :: d0  @ PART2 = " << d0 << std::endl;

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

      f3  = T(0.0);
      df3 = std::vector<T>( energyGradientSize );
      _itflow.assign( _reshapeFeedback( _addVectors( X, _scaleVector( x3, s ) ) ) );
      for (sIt = stencilsBegin; sIt != stencilsEnd; ++sIt) {
        is = (*((InputSlot<EnergyStencil<T>*>*)*sIt))();

        energy = T(0.0);
        for (xI = 0; xI < _pWidth; ++xI)
        for (yI = 0; yI < _pHeight; ++yI)
        for (zI = 0; zI < _pDepth; ++zI)
        {
          energy += is->getEnergy( 0, xI, yI, zI, 0 );
          localGradient = is->getEnergyGradient( 0, xI, yI, zI, 0 );
          for (gc = 0; gc < flowDimensions(); ++gc)
            energyGradient.at(_linearIndex( gc, xI, yI, zI, 0, _pSize, _pWidth, _pHeight, _pDepth, _pSpectrum)) = localGradient.at(gc);
        }
        f3  += energy;
	df3 = _addVectors( df3, energyGradient );
      }

      if (f3 < F0) {
	X0 = _addVectors( X, _scaleVector( x3, s ) );
	F0 = f3;
	dF0 = df3;
      }

      M = M-1; i = i + (length<0);

      d3 = _dotProduct( df3, s );

    }  //  while


//////////     PART 3

std::cout << "(II) CGSolver :: d3  @ PART3 = " << d3 << std::endl;
std::cout << "(II) CGSolver :: SIG @ PART3 = " << SIG << std::endl;
std::cout << "(II) CGSolver :: d0  @ PART3 = " << d0 << std::endl;

    if (
	   std::abs(d3) < -SIG*d0
       &&
           f3 < f0+x3*RHO*d0
       ) {
      X = _addVectors( X, _scaleVector( x3, s ) );
      _itflow.assign( _reshapeFeedback( X ) );
	
      f0 = f3;

  std::cout << "(II) CGSolver :: performing update step" << std::endl;

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
      _itflow.assign( _reshapeFeedback( X ) );

      if (ls_failed || i > std::abs(length)) break;

      s = _scaleVector(T(-1.0), df0);
      d0 = T(-1.0) * _dotProduct(s,s);

      x3 = 1/(T(1.0)-d0);
      ls_failed = 1;

      std::cout << "(WW) CGSolver :: iteration " << i << " line search failed !" << std::endl;
    }

    sout << "(II) CGSolver iteration, energy = ";
    sout << i << ", " << f3 << std::endl;

  }  //  while (i < abs(length))

  sout << "(II) CGSolver " << i << " iterations performed" << std::endl;

}

#endif // _CGSOLVER_HXX_

