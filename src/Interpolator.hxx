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
/** @file Interpolator.hxx
 *  Implementation of template class Interpolator and related classes.
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  Based on a file written by Daniel Kondermann, modified by Jens-Malte
 *  Gottfried.
 *
 *  @date 05.05.2008
 */

#ifndef _Interpolator_HXX_
#define _Interpolator_HXX_

#include "Interpolator.h"
#include "ParameteredObject.hxx"
#include <cassert>

template <typename T>
Interpolator<T>::Interpolator(const std::string& className,
                              const std::string& name,
                              const std::string& doc) :
        TemplatedParameteredObject<T>(className, name, doc),
        out(0) {
    _addOutputSlot(out, "out", "interpolator pointer", "interpolator");
    out = this;
}

template <typename T>
Interpolator<T>::~Interpolator() {
}

#endif // _Interpolator_HXX_
