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
/** @file   Convolution.hxx
 *  @brief  %Convolution implementations.
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date   15.06.2009
 *
 *  Interface is in Convolution.h.
 *  Include Convolution.h, if you only need the class and
 *  function declarations (prototypes).
 */

#ifndef _Convolution_HXX
#define _Convolution_HXX

#include "Convolution.h"
#include <cassert>
#include "Interpolator.h"
#include "PdeBoundaryHandler.hxx"

#ifndef PI
    /// Circle number \f$\pi\f$.
    #define PI 3.14159265358979323846
#endif

template<typename T, typename T2>
T Convolution::convolve(const cimg_library::CImg<T2>& mask1,
                        const cimg_library::CImg<T >& mask2) {
    assert(mask1.dimx() == mask2.dimx());
    assert(mask1.dimy() == mask2.dimy());
    assert(mask1.dimz() == mask2.dimz());
    assert(mask1.dimv() == 1);
    assert(mask2.dimv() == 1);

    T   sum  = 0;
    cimg_forXYZ(mask1, x, y, z)
        sum += mask1(x,y,z) * mask2(x,y,z);

    return sum;
}

template<typename T>
void Convolution::convolve(const cimg_library::CImg<T>& mask,
                           const cimg_library::CImg<T>& src,
                                 cimg_library::CImg<T>& dst) {
    dst = src.get_convolve(mask);
}

template<typename T>
T Convolution::convolvePoint(const cimg_library::CImg<T>& mask,
                             const cimg_library::CImg<T>& src,
                             int x, int y, int z, int v,
                             int centerX, int centerY, int centerZ) {
    assert(mask.dimx() % 2);
    assert(mask.dimy() % 2);
    assert(mask.dimz() % 2);
    T sum = 0;

    for (int zm=-centerZ; zm<=centerZ; zm++)
        for (int ym=-centerY; ym<=centerY; ym++)
            for (int xm=-centerX; xm<=centerX; xm++)
                sum += src.atXYZ(x-xm,y-ym,z-zm,v) *
                       mask(centerX+xm,centerY+ym,centerZ+zm);
    return sum;
}

template<typename T>
T Convolution::convolvePoint(const cimg_library::CImg<T>& mask,
                             const cimg_library::CImg<T>& src,
                             int x, int y, int z, int v) {
    int centerX = (int)(mask.dimx()/2);
    int centerY = (int)(mask.dimy()/2);
    int centerZ = (int)(mask.dimz()/2);
    return convolvePoint(mask, src, x, y, z, v, centerX, centerY, centerZ);
}

template<typename T>
T Convolution::convolvePointF(const cimg_library::CImg<T>& mask,
                              const cimg_library::CImg<T>& src,
                              Interpolator<T>* interpolator,
                              float x, float y, float z, int v,
                              int centerX, int centerY, int centerZ) {
    assert(mask.dimx() % 2);
    assert(mask.dimy() % 2);
    assert(mask.dimz() % 2);
    T sum = 0;
    for(int zm=-centerZ; zm<=centerZ; zm++)
        for(int ym=-centerY; ym<=centerY; ym++)
            for(int xm=-centerX; xm<=centerX; xm++)
                sum += interpolator->interpolate(src, x-xm, y-ym, z-zm, v) *
                       mask(centerX+xm, centerY+ym, centerZ+zm);
    return sum;
}

template<typename T>
T Convolution::convolvePointF(const cimg_library::CImg<T>& mask,
                              const cimg_library::CImg<T>& src,
                              Interpolator<T>* interpolator,
                              float x, float y, float z, int v) {
    int centerX =(int)(mask.dimx()/2);
    int centerY =(int)(mask.dimy()/2);
    int centerZ =(int)(mask.dimz()/2);
    return convolvePointF(mask, src, interpolator, x, y, z, v,
                          centerX, centerY, centerZ);
}

template<typename T>
void Convolution::convolveSeperable(const cimg_library::CImg<T>& maskH,
                                    const cimg_library::CImg<T>& maskV,
                                    const cimg_library::CImg<T>& src,
                                          cimg_library::CImg<T>& dst) {
    dst = src.get_convolve(maskH);
    dst = dst.get_convolve(maskV);
}

template<typename T>
T Convolution::convolvePoint(const cimg_library::CImg<T>& mask,
                             const PdeBoundaryHandler<T>* src,
                             int x, int y, int z, int v,
                             int centerX, int centerY, int centerZ) {
    T res = 0;
    for(int zm=-centerZ; zm<=centerZ; zm++)
        for(int ym=-centerY; ym<=centerY; ym++)
            for(int xm=-centerX; xm<=centerX; xm++)
                res += (T)(src->getU(x-xm, y-ym, z-zm)[v] *
                           mask(centerX+xm, centerY+ym, centerZ+zm));
    return res;
}

template<typename T>
void Convolution::getGauss2DSeperable(float sigma,
                                      cimg_library::CImg<T>& maskH,
                                      cimg_library::CImg<T>& maskV,
                                      float range) {
    assert(sigma > 0);

    int i;
    int size = (int)ceil(range*sigma);
    //prevent box sizes of one
    if(size == 1)
        size = 3;
    if(!(size % 2)) ++size;
    int offset= (int)(size/2);
    double b = 1.0/(2*sigma*sigma);

    maskH.assign(size, 1);

    T sum = 0;
    cimg_forX(maskH,x) {
        i = x-offset;
        sum += maskH(x) = (T)(exp(-i*i*b));
    }
    maskH /= sum;

    maskV = maskH.get_transpose();
}

template<typename T>
void Convolution::getGauss1D(float sigma,
                             cimg_library::CImg<T>& mask,
                             char axe, float range) {
    assert(sigma > 0);
    assert(axe >= 0 && axe <= 3);

    int i;
    int size = (int)ceil(range*sigma);
    if (size == 1) //prevent box sizes of one
        size = 3;
    if (!(size % 2)) ++size;
    int offset = (int)(size/2);
    double b = 1.0/(2*sigma*sigma);

    cimg_library::CImg<T> tmp(3);

    T sum = 0;
    cimg_forX(tmp,x) {
        i =x-offset;
        sum +=tmp(x) =(T)(exp(-i*i*b));
    }

    tmp /= sum;

    if(axe == 0)
        mask = tmp;
    else if(axe == 1)
        mask = tmp.get_transpose();
    else if(axe == 2) {
        mask.assign(1, 1, tmp.dimx());
        cimg_forX(tmp, x)
            mask(0, 0, x) = tmp(x);
    }
    else if(axe == 3) {
        mask.assign(1, 1, 1, tmp.dimx());
        cimg_forX(tmp, x)
            mask(0, 0, 0, x) = tmp(x);
    }
}

template<typename T>
void Convolution::getGauss2D(float sigma,
                             cimg_library::CImg<T>& mask,
                             float range) {
    assert(sigma > 0);

    int i,j;
    int size =(int)ceil(range*sigma);
    if (!(size % 2))
        ++size;

    int offset = (int)(size/2);
    double b = 1.0/(2*sigma*sigma);
    mask.assign(size, size);

    T sum = 0;
    cimg_forXY(mask,x,y) {
        i = x-offset;
        j = y-offset;
        sum += (mask(x,y) = (T)(exp(-(i*i+j*j)*b)));
    }

    mask /= sum;
}

template<typename T>
void Convolution::getGauss3D(float sigmaX, float sigmaY, float sigmaZ,
                             cimg_library::CImg<T>& mask,
                             float range) {
    assert(sigmaX > 0 || sigmaY > 0 || sigmaZ > 0);

    int i, j, k;
    int sizeX = (int)ceil(range*sigmaX);
    int sizeY = (int)ceil(range*sigmaY);
    int sizeZ = (int)ceil(range*sigmaZ);
    if (!(sizeX % 2)) ++sizeX;
    if (!(sizeY % 2)) ++sizeY;
    if (!(sizeZ % 2)) ++sizeZ;

    int offsetX=(int)(sizeX/2);
    int offsetY=(int)(sizeY/2);
    int offsetZ=(int)(sizeZ/2);

    double bX=sigmaX ? 1.0/(2*sigmaX*sigmaX) : 0;
    double bY=sigmaY ? 1.0/(2*sigmaY*sigmaY) : 0;
    double bZ=sigmaZ ? 1.0/(2*sigmaZ*sigmaZ) : 0;
    mask.assign(sizeX, sizeY, sizeZ, 1, 0);

    T sum =0;
    cimg_forXYZ(mask, x, y, z) {
        i =x-offsetX;
        j =y-offsetY;
        k =z-offsetZ;
        sum +=mask(x, y, z) =(T)(exp(-i*i*bX-j*j*bY-k*k*bZ));
    }

    mask /= sum;
}

template<typename T>
void Convolution::getLoG2D(float sigma, cimg_library::CImg<T>& mask) {
    int kSize = (int)ceil(6.0*sigma);
    if (kSize % 2 == 0)
        kSize++;

    mask.assign(kSize, kSize);

    int w = kSize/2;
    T n1sn2s, twoss, g;
    T sum = 0;
    for(int x =-w; x <= w; ++x)
        for(int y =-w; y <= w; ++y) {
            n1sn2s = (T)x*x+y*y;
            twoss  = 2*sigma*sigma;
            g      = (T)exp(-n1sn2s/twoss);
            //mask[pos] =(n1sn2s-twoss)*g /
                          (2*PI*sigma*sigma*sigma*sigma*sigma*sigma);
            mask(x+w, y+w) = (T)(-1/(PI*sigma*sigma*sigma*sigma)
                                 * (1-n1sn2s/twoss)*g);
            sum += mask(x+w, y+w);
        }

    mask /= sum;
}

#endif // _Convolution_HXX
