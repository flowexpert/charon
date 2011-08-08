/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file cimgtest.cpp
/// Some tests for CImg usage.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @date 06.10.2009

#ifndef TESTDIR
#error TESTDIR undefined!
#define TESTDIR
#endif

#include <charon-utils/CImg.h>
#include <iostream>

/// main function
int main() {
	bool success=true;
	cimg_library::CImgList<int> imgCimg, imgPNG(1u);
	imgPNG[0].load_png(TESTDIR "/smile.png");
	imgCimg.load_cimg(TESTDIR "/smile.cimg");
	if(imgCimg.size() <= 0 || imgPNG.size() <= 0) {
		std::cerr << "Error: cimg or png list empty" << std::endl;
		success = false;
	}
	if (!imgPNG.is_sameNXYZC(imgCimg)) {
		std::cerr << "Error: cimg and png sizes mismatch!" << std::endl;
		success = false;
	}
	if ((imgPNG[0u] - imgCimg[0u]).abs().max() > 0) {
		std::cerr << "Error: cimg and png content differs!" << std::endl;
		success = false;
	}

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
