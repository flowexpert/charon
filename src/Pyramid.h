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
/** @file   Pyramid.h
 *  @brief  Declaration of abstract class Pyramid.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date   15.06.2009
 *
 *  Based on Pyramid.hxx by Daniel Kondermann.
 *
 *  This file contains only the interface.
 *  Implementations of the template functions are in Pyramid.hxx.
 *  In most cases you want to include Pyramid.hxx instead of Pyramid.h.
 */
#ifndef _Pyramid_H
#define _Pyramid_H

#include "CImg.h"
#include <vector>
#include "ParameterFile.h"

/// Abstract base class for pyramid creation.
/// Whenever a pyramid is needed, an instance
/// of this object can be used to create and access the pyramid.
/// The creation is implementation dependent.
/// Level 0 always needs to be the coarsest level of the pyramid.
/// The constructor should receive an image and immediately create the pyramid.
template <typename T>
class Pyramid {
protected:
    std::string  _className;            ///< class name

    /// Copy properties to another pyramid (content is not touched).
    virtual void _copyTo(Pyramid<T>* target) const;

public:
    int nLevels;                        ///< number of levels
    cimg_library::CImgList<T> levels;   ///< level content
    std::vector<T> scaleFactorsX;       ///< scale factor (x-dim)
    std::vector<T> scaleFactorsY;       ///< scale factor (y-dim)

public:
    /// Constructor initializing level number.
    /// @param  className           class name
    /// @param  nLevels             number of levels
    Pyramid(std::string className, int nLevels);

    /// Constructor assigning image content and loads properties.
    /// @param  className           class name
    /// @param  src                 image content for initialization
    /// @param  pf                  file to read properties
    /// @param  name                instance name
    Pyramid(std::string className, cimg_library::CImg<T>& src,
            ParameterFile& pf, std::string name);

    virtual ~Pyramid();

    /// Return class name.
    std::string getClassName() const;

    /// Assign image content.
    /// Perform calculations to get pyramid filled using the given image.
    /// @param  src                 image source
    virtual void assign(const cimg_library::CImg<T>& src) = 0;

    /// Copy content to another pyramid and return pointer to it.
    virtual Pyramid<T>* getCopy() = 0;

    // virtual Roi<T> getRoi(Roi<int> srRoi, int n) = 0;

    /// Get number of pyramid levels.
    int getLevels() const;

    /// Return single level from the pyramid.
    /// @param  n                   number of the level to get
    cimg_library::CImg<T>* getLevel(int n);

    /// Returns the factor that was used to resize the image from
    /// the finest scale to the coarser scale n.
    /// Thus, getLevel(0)->dimx()*factor - getLevel(n)->dimx() < epsilon
    /// where epsilon is the error which occured due to integer rounding
    /// of the image dimensions.
    ///
    /// Version for x-dimension.
    /// @param n                    level to select
    T getScaleFactorX(int n) const;

    /// Returns the factor that was used to resize the image from
    /// the finest scale to the coarser scale n.
    /// Thus, getLevel(0)->dimy()*factor - getLevel(n)->dimy() < epsilon
    /// where epsilon is the error which occured due to integer rounding
    /// of the image dimensions.
    ///
    /// Version for y-dimension.
    /// @param n                    level to select
    T getScaleFactorY(int n) const;

    /*
    /// Returns the factor that was used to resize the image
    /// from the scale 'from' to the scale 'to'.
    /// Thus, getLevel(from)->dimx()*factor - getLevel(to)->dimx() < epsilon
    /// where epsilon is the error which occured due to integer rounding
    /// of the image dimensions.
    T getScaleBetweenLevels(int from, int to);
    */

    /// Save pyramid properties to a ParameterFile.
    /// Content is not saved.
    /// @param  pf                  ParameterFile to write to.
    /// @param  name                name prefix
    virtual void save(ParameterFile& pf, std::string name) = 0;

    /// Load pyramid properties from a ParameterFile.
    /// Content is not restored.
    /// @param  pf                  ParameterFile to read from.
    /// @param  name                name prefix
    virtual void load(ParameterFile& pf, std::string name) = 0;
};

#endif // _Pyramid_H
