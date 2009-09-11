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
/** @file   Pyramid.hxx
 *  @brief  Implementations for abstract class Pyramid
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date   15.06.2009
 *
 *  Interface is in Pyramid.h.
 *  Include Pyramid.h, if you only need the class and
 *  function declarations (prototypes).
 */

#ifndef _Pyramid_HXX
#define _Pyramid_HXX

#include "Pyramid.h"
#include "ParameterFile.hxx"
#include <cassert>
#include "Convolution.hxx"


template <typename T>
void Pyramid<T>::_copyTo(Pyramid<T>* target) const {
    target->nLevels       = nLevels;
    target->levels        = levels;
    target->scaleFactorsX = scaleFactorsX;
    target->scaleFactorsY = scaleFactorsY;
}

template <typename T>
Pyramid<T>::Pyramid(std::string className, int newNLevels) :
        _className(className),
        nLevels(newNLevels) {
}

template <typename T>
Pyramid<T>::Pyramid(std::string className,
                    cimg_library::CImg<T>& /*src*/,
                    ParameterFile& /*pf*/,
                    std::string /*name*/) :
        _className(className)  {
}

template <typename T>
Pyramid<T>::~Pyramid() {
}

template <typename T>
std::string Pyramid<T>::getClassName() const {
    return _className;
}

template <typename T>
int Pyramid<T>::getLevels() const {
    return nLevels;
}

template <typename T>
cimg_library::CImg<T>* Pyramid<T>::getLevel(int n) {
    assert (n < nLevels);
    return &levels[n];
}

template <typename T>
T Pyramid<T>::getScaleFactorX(int n) const {
    assert (n < nLevels);
    return scaleFactorsX[n];
}

template <typename T>
T Pyramid<T>::getScaleFactorY(int n) const {
    assert (n < nLevels);
    return scaleFactorsY[n];
}

/*
template <class T> T Pyramid<T>::getScaleBetweenLevels(int from, int to)
{
    assert(from < nLevels && from >= 0);
    assert(to < nLevels && to >= 0);

    T factor;
    if(from < to) //going from finer level to coarser level, factor < 1
    {
        factor =1.0f/(getScaleFactor(from)/getScaleFactor(to));
    }
    else if(from > to) //factor > 1
    {
        factor =getScaleFactor(to)/getScaleFactor(from);
    }
    else
    {
        factor =(T)1.0;
    }

    //float test =(float)fabs(getLevel(from)->dimx()*factor - (float)getLevel(to)->dimx());
    //assert(fabs(getLevel(from)->dimx()*factor - (float)getLevel(to)->dimx()) < 0.1f);
    return factor;
}
*/

#endif // _Pyramid_HXX
