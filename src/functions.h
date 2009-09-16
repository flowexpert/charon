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
/// @file functions.h
/// includes some helpful functions
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _functions_h_
#define _functions_h_

//#include "main.h"
#include "CImg.h"
#include <iostream>
//#include "DataAssembler.h"


/// namespace for some helpful functions
namespace Functions
{

/**
 * computes a 2D gaussian filter
 * @param sigma sigma of the gaussian
 */
cimg_library::CImg<> gauss_filter(float sigma);
//void assemblertest(DataAssembler* assembler);

cimg_library::CImg<> gauss_filter3d(float sigma);

/**
 * computes the convolution of two filters
 * @param f1 filter 1
 * @param f2 filter 2
 * @return f1*f2
 *///FIXME keine CImg Rueckgabe
cimg_library::CImg<> falte_filter(const cimg_library::CImg<float>& f1,
		const cimg_library::CImg<float>& f2);

/// prints the matrix to standard output
void printMatrix(const cimg_library::CImg<>& m);

}

#endif
