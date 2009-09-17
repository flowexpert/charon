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
/** @file   Convolution.h
 *  @brief  Declaration of convolution routines.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date   15.06.2009
 *
 *  Based on Convolution.hxx by Daniel Kondermann.
 *
 *  This file contains only the routine declarations.
 *  Implementations of the template functions are in Convolution.hxx.
 *  In most cases you want to include Convolution.hxx instead of
 *  Convolution.h.
 */

#ifndef _Convolution_H
#define _Convolution_H

#include <CImg.h>
#include "Interpolator.h"
#include "PdeBoundaryHandler.h"

/// %Convolution routines.
namespace Convolution {
    /// Just do product-sum (3D).
    /**
        \f[
            \text{res}=\sum_{x,y,z} A_{x,y,z}\cdot B_{x,y,z}
        \f]
     */
    /// @param  mask1           \f$A\f$
    /// @param  mask2           \f$B\f$
    /// @returns                \f$\text{res}\f$
    /// @todo                   Function name misleading!
    template <typename T, typename T2>
    T convolve (            const cimg_library::CImg<T2>& mask1,
                            const cimg_library::CImg<T >& mask2);

    /// Simple convolution.
    /// Convolve src with mask, output is dst.
    /// @param  src             image input
    /// @param  mask            convolution mask
    /// @param  dst             result output
    template <typename T>
    void convolve(          const cimg_library::CImg<T>& mask,
                            const cimg_library::CImg<T>& src,
                                  cimg_library::CImg<T>& dst);

    /// Get single point of convolution result.
    /// Mask is not shifted.
    /// @param  mask            convolution mask (<=3D)
    /// @param  src             image input
    /// @param  x,y,z,v         image coordinates
    /// @returns                convolution value at (x,y,z,v)
    template <typename T>
    T convolvePoint(        const cimg_library::CImg<T>& mask,
                            const cimg_library::CImg<T>& src,
                            int x, int y, int z, int v);

    /// Get single point of interpolated convolution result.
    /// Mask is not shifted.
    /// @param  mask            convolution mask (<=3D)
    /// @param  src             image input
    /// @param  x,y,z,v         image coordinates
    /// @param  interpolator    used for image interpolation
    /// @returns                convolution value at (x,y,z,v)
    template <typename T>
    T convolvePointF(       const cimg_library::CImg<T>& mask,
                            const cimg_library::CImg<T>& src,
                            Interpolator<T>* interpolator,
                            float x, float y, float z, int v);

    /// Get single point of convolution result.
    /// Mask center is shifted to the given coordinates.
    /// @param  mask            convolution mask (<=3D)
    /// @param  src             image input
    /// @param  x,y,z,v         image coordinates
    /// @param  centerX,centerY,centerZ
    ///                         mask shift coordinates
    /// @returns                convolution value at (x,y,z,v)
    template <typename T>
    T convolvePoint(        const cimg_library::CImg<T>& mask,
                            const cimg_library::CImg<T>& src,
                            int x, int y, int z, int v,
                            int centerX, int centerY, int centerZ);

    /// Get single point of convolution result with boundary handling.
    /// Mask center is shifted to the given coordinates.
    /// @param  mask            convolution mask (<=3D)
    /// @param  src             image input as boundary handler
    /// @param  x,y,z,v         image coordinates
    /// @param  centerX,centerY,centerZ
    ///                         mask shift coordinates
    /// @returns                convolution value at (x,y,z,v)
    template <typename T>
    T convolvePoint(        const cimg_library::CImg<T>& mask,
                            const PdeBoundaryHandler<T>* src,
                            int x, int y, int z, int v,
                            int centerX, int centerY, int centerZ);

    /// Get single point of interpolated convolution result.
    /// Mask center is shifted to the given coordinates.
    /// @param  mask            convolution mask (<=3D)
    /// @param  src             image input
    /// @param  x,y,z,v         image coordinates
    /// @param  centerX,centerY,centerZ
    ///                         mask shift coordinates
    /// @param  interpolator    used for image interpolation
    /// @returns                convolution value at (x,y,z,v)
    template <typename T>
    T convolvePointF(       const cimg_library::CImg<T>& mask,
                            const cimg_library::CImg<T>& src,
                            Interpolator<T>* interpolator,
                            float x, float y, float z, int v,
                            int centerX, int centerY, int centerZ);

    /// Convolve twice with separable masks.
    /// Convolves src with maskH and maskV. The resulting mask is
    /// the convolution of maskH and maskV but the double convolution
    /// can be much faster, if maskH is only a 1D-mask in x-direction
    /// and maskV is only a 1D mask in y-direction.
    /// \f[\text{res}=(\text{src}*m_1)*m_2\f]
    /// @param  maskH           first  convolution mask (1D), \f$m_1\f$
    /// @param  maskV           second convolution mask (1D), \f$m_2\f$
    /// @param  src             image input
    /// @param  dst             result output, \f$\text{res}\f$
    template <typename T>
    void convolveSeperable( const cimg_library::CImg<T>& maskH,
                            const cimg_library::CImg<T>& maskV,
                            const cimg_library::CImg<T>& src,
                                  cimg_library::CImg<T>& dst);

    /// Get gaussion convolution masks.
    /// These separable masks can be used in convolveSeparable.
    /// Mask size is range*sigma.
    /// @param  sigma           gaussian width
    /// @param  maskH           horizontal mask output
    /// @param  maskV           vertical mask output (= maskH transposed)
    /// @param  range           adjust mask size (range*sigma)
    template <typename T>
    void getGauss2DSeperable(float sigma,
                             cimg_library::CImg<T>& maskH,
                             cimg_library::CImg<T>& maskV,
                             float range = 3.0f);

    /// Get 1D gaussian mask.
    /// Mask direction can be chosen.
    /// @param  sigma           gaussian width
    /// @param  mask            mask output
    /// @param  axe             mask direction (0:x,1:y,2:z,3:v)
    /// @param  range           adjust mask size (range*sigma)
    template <typename T>
    void getGauss1D(        float sigma,
                            cimg_library::CImg<T>& mask,
                            char axe, float range = 3.0f);

    /// Get 2D gaussian mask.
    /// Mask is in xy-plane.
    /// @param  sigma           gaussian width
    /// @param  mask            mask output
    /// @param  range           adjust mask size (range*sigma)
    template <typename T>
    void getGauss2D(        float sigma,
                            cimg_library::CImg<T>& mask,
                            float range = 3.0f);

    /// Get 3D gaussian mask.
    /// Mask is in xyz-cube.
    /// @param  sigmaX,sigmaY,sigmaZ
    ///                         gaussian widths for each direction
    /// @param  mask            mask output
    /// @param  range           adjust mask size (range*sigma)
    template <typename T>
    void getGauss3D(        float sigmaX, float sigmaY, float sigmaZ,
                            cimg_library::CImg<T>& mask,
                            float range = 3.0f);

    /// Get 2D laplace of gaussian mask.
    /// Also known as "mexican hat" or "Sombrerofilter".
    /// Mask is in xy-plane. Size is about \f$6\sigma\f$.
    /**
        \f[
            g(x, y) = -\frac{1}{\pi \sigma^4}
                e^{-\frac{x^2 + y^2}{2 \sigma^2}}
                    \left(1-\frac{x^2 + y^2}{2 \sigma^2}\right)
        \f]
     */
    /// @param  sigma           gaussian width
    /// @param  mask            mask output, \f$g\f$
    template <typename T>
    void getLoG2D(          float sigma,
                            cimg_library::CImg<T> &mask);
}

#endif // _Convolution_H
