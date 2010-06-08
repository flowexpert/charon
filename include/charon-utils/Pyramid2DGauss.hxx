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
/** @file   Pyramid2DGauss.hxx
 *  @brief  Implementations for class Pyramid2DGauss
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date   15.06.2009
 *
 *  Interface is in Pyramid2DGauss.h.
 *  Include Pyramid2DGauss.h, if you only need the class and
 *  function declarations (prototypes).
 */

#ifndef _Pyramid2DGaussBy2_HXX
#define _Pyramid2DGaussBy2_HXX

#include "Pyramid2DGauss.h"
#include "Pyramid.hxx"

template <class T>
void Pyramid2DGauss<T>::_copyTo(Pyramid2DGauss<T>* target) {
    Pyramid<T>::_copyTo(target);
    target->_sigmaDivisor   = _sigmaDivisor;
    target->_base           = _base;
    target->_exponentFactor = _exponentFactor;
}

template <class T>
Pyramid2DGauss<T>::Pyramid2DGauss() :
        Pyramid<T>("Pyramid2DGauss", 0) {
}

template <class T>
Pyramid2DGauss<T>::Pyramid2DGauss(
        const cimg_library::CImg<T>& src,
        int nLevels, float sigmaDivisor,
        float base, float exponentFactor) :
                Pyramid<T>("Pyramid2DGauss", nLevels),
                _sigmaDivisor(sigmaDivisor),
                _base(base),
                _exponentFactor(exponentFactor) {
    assign(src);
}

template <class T>
Pyramid2DGauss<T>::Pyramid2DGauss(
        cimg_library::CImg<T>& src,
        ParameterFile& pf,
        std::string name) :
                Pyramid<T>("Pyramid2DGauss", 0) {
    load(pf, name);
    assign(src);
}

template <class T>
Pyramid2DGauss<T>::~Pyramid2DGauss(void) {
}

template <class T>
Pyramid<T>* Pyramid2DGauss<T>::getCopy() {
    Pyramid2DGauss<T>* ret = new Pyramid2DGauss();
    _copyTo(ret);
    return ret;
}

template <class T>
void Pyramid2DGauss<T>::save(ParameterFile& pf, std::string name) {
    pf.set(name+".nLevels",        Pyramid<T>::nLevels);
    pf.set(name+".sigmaDivisor",   _sigmaDivisor);
    pf.set(name+".base",           _base);
    pf.set(name+".exponentFactor", _exponentFactor);
}

template <class T>
void Pyramid2DGauss<T>::load(ParameterFile& pf, std::string name) {
    Pyramid<T>::nLevels = pf.get<int>  (name+".nLevels",     2 );
    _sigmaDivisor   = pf.get<float>(name+".sigmaDivisor",  1.4f);
    _base           = pf.get<float>(name+".base",           2.f);
    _exponentFactor = pf.get<float>(name+".exponentFactor", 1.f);
}

template <class T>
void Pyramid2DGauss<T>::assign(const cimg_library::CImg<T>& src) {
    Pyramid<T>::levels.clear();
    Pyramid<T>::levels.push_back(src);
    Pyramid<T>::scaleFactorsX.push_back(T(1));
    Pyramid<T>::scaleFactorsY.push_back(T(1));

    cimg_library::CImg<T> maskH;
    cimg_library::CImg<T> maskV;
    cimg_library::CImg<T> tmp(src.width(), src.height());
    cimg_library::CImg<T> downsampled;

    for (int i = 1; i < Pyramid<T>::nLevels; ++i) {
        // standard downsampling by two would be base==2 and exponentFactor==1
        float factor = pow(_base, (float)i*_exponentFactor);
        // spatial sigma, roughly optimal wrt spatial low-pass filter
        // float sigma =1.0f/(0.8f*factor);
        float sigma = factor / _sigmaDivisor;
        //put a gaussian filter on image (not speed-optimal)
        Convolution::getGauss2DSeperable(sigma, maskH, maskV);
        Convolution::convolveSeperable(maskH, maskV, src, tmp);

        // using two factors might slightly stretch the image
        // therefore these factors are stored for convenience
        // so that when upsampling results
        // based on a given level, the factor can be used to restore the
        // original proportions of the image
        int newwidth = (int)ceilf(src.width()/factor);
        int newheight = (int)ceilf(src.height()/factor);
        downsampled.assign(newwidth, newheight, src.depth(), src.spectrum(), 0);
        //tmp.display();

        /*
        InterpolatorCubic<T> interpolator;
        cimg_forXYZV(downsampled, x, y, z, v)
        {
            // get pixels at center of each pixel location
            // center = position where the upsampled coordinate
            // falls in the original image
            // hence no translation occurs
            downsampled(x, y, z, v) = interpolator.interpolate(tmp,
                (x-0.5f)*src.width()/(float)newwidth,
                (y-0.5f)*src.height()/(float)newheight, z, v);
        }*/

        downsampled = tmp.get_resize(newwidth, newheight, downsampled.depth(),
                                     downsampled.spectrum(), 5);

        //downsampled.display("downsampled");
        // save result to current pyramid level (level 0 is original)
        Pyramid<T>::levels.push_back(downsampled);

        Pyramid<T>::scaleFactorsX.push_back(src.width()/(float)newwidth);
        Pyramid<T>::scaleFactorsY.push_back(src.height()/(float)newheight);

        /*
        this->scaleFactorsX.push_back(factor);
        this->scaleFactorsY.push_back(factor);
        */


        /*
        float sigma =(float)pow((float)2, (float)i-1);
        // put a gaussian filter on image
        conv.getGauss2DSeperable(sigma, maskH, maskV);
        conv.convolveSeperable(maskH, maskV, src, tmp);
        // downsample tmp by 2 based on zero(first) level
        float factor =1.0f/(2*sigma);
        // stop building pyramid if images become too small
        if(src.width()*factor < 5 && src.height()*factor < 5)
            break;
        tmp.resize((int)ceil(src.width()*factor), (int)ceil(src.height()*factor));
        // save result to current pyramid level (level 0 is original)
        this->levels << tmp;
        scaleFactors.push_back(factor);
        */
    }

    Pyramid<T>::nLevels = Pyramid<T>::levels.size();
}

#endif // _Pyramid2DGaussBy2_HXX
