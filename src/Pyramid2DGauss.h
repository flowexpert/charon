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
/** @file   Pyramid2DGauss.h
 *  @brief  Declaration of class Pyramid2DGauss.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date   15.06.2009
 *
 *  Based on Pyramid2DGauss.hxx by Daniel Kondermann.
 *
 *  This file contains only the interface.
 *  Implementations of the template functions are in Pyramid2DGauss.hxx.
 *  In most cases you want to include Pyramid2DGauss.hxx
 *  instead of Pyramid2DGauss.h.
 */
#ifndef _Pyramid2DGaussBy2_H
#define _Pyramid2DGaussBy2_H

#include "Pyramid.h"

/// Create a Gaussian pyramid upon creation. Each level is downsampled by two.
template <class T>
class Pyramid2DGauss : public Pyramid<T>
{
protected:
    float _sigmaDivisor;    ///< @todo  add content
    float _base;            ///< @todo  add content
    float _exponentFactor;  ///< @todo  add content

    /// Copy properties to another pyramid (content is not touched).
    virtual void _copyTo(Pyramid2DGauss<T>* target);

    Pyramid2DGauss();

public:
    /// Constructor assigning properties.
    /// @param  src                 image content for initialization
    /// @param  nLevels,sigmaDivisor,base,exponentFactor
    ///                             properties to set
    Pyramid2DGauss(const cimg_library::CImg<T>& src, int nLevels,
                   float sigmaDivisor = 1.4f, float base = 2.0f,
                   float exponentFactor = 1.0f);

    /// Constructor assigning image content and loads properties.
    /// @param  src                 image content for initialization
    /// @param  pf                  file to read properties
    /// @param  name                instance name
    Pyramid2DGauss(cimg_library::CImg<T>& src,
                   ParameterFile& pf,
                   std::string name);

    virtual ~Pyramid2DGauss();

    virtual void assign(const cimg_library::CImg<T>& src);

    Pyramid<T>* getCopy();

    virtual void save(ParameterFile& pf, std::string name);

    virtual void load(ParameterFile& pf, std::string name);
};

#endif // _Pyramid2DGaussBy2_H
