/*  Copyright (C) 2009 René Steinbrügge

    This file is part of Charon.

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
/// @file BrightnessFunctorInterface.h
/// defines interface class for the brightness change functor
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _BrightnessFunctorInterface_h_
#define _BrightnessFunctorInterface_h_

#include <CImg.h>

/// Interface class for the functor of brightness change
template <typename T>
class BrightnessFunctorInterface
{
public:
	/**
	 *        applies a change of brightness to the sequence
	 * @param sequence sequence to work on
	 */
	virtual void operator() (cimg_library::CImg<T>& sequence) const = 0;  //TODO auf 3D und CImgList umstellen
	virtual void get3d(cimg_library::CImgList<T>& sequence) const = 0;
};


#endif
