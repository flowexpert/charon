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
/// @file imgToolTest.cpp
/// Some tests for ImgTool usage.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @date 16.06.2009

#include "ImgTool.hxx"

#ifndef PENGUINFILE
#define PENGUINFILE "Penguin.pgm"
#warning using "Penguin.pgm"
#endif

const unsigned int wTime = 500;

int main() {
    cimg_library::CImg<unsigned int> img(PENGUINFILE);
    cimg_library::CImgDisplay disp(img, "Original Penguin", 1, false, false);
    disp << img;
    disp.wait(wTime);

    cimg_library::CImg<float> integral;
    ImgTool::integralImage2D(img, integral);
    disp << integral;
    disp.wait(wTime);

    cimg_library::CImg<double> noise;
    ImgTool::createAllScaleRandomPattern (noise, 100, 100, 0.5, 3);
    disp.assign(noise, "noise", 1, false, false);
    disp << noise;
    disp.wait(wTime);

    cimg_library::CImg<double> out;
    ImgTool::grayToHsv(noise, out);
    disp << out;
    disp.wait(wTime);

    ImgTool::chessBoard2D(20, 20, noise);
    disp << noise;
    disp.wait(wTime);

    return 0;
}
