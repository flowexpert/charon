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

#include <charon-utils/Pyramid2DGauss.hxx>
#include <charon-utils/ImgTool.hxx>
#include <charon-utils/InterpolatorLinear.hxx>

#ifndef PENGUINFILE
#error PENGUINFILE not defined!
#endif

const unsigned int wTime = 500;

int main() {
    cimg_library::CImg<float> img(PENGUINFILE);
    cimg_library::CImgDisplay disp(img, "Original Penguin", 1, false, false);
    disp.display(img);
    disp.wait(wTime);

	cimg_library::CImg<float> moved = img.get_crop(3,-2,0,0,
		img.width()+2,img.height()-3,img.depth()-1,img.spectrum()-1);
	moved.display(disp);
	disp.wait(wTime);

	// test warping
	cimg_library::CImgList<float> flow(2, img.width(), img.height(),
		img.depth(), img.spectrum());
	flow[0].fill(3.f);
	flow[1].fill(-2.f);
	Interpolator<float>* interpolator = new InterpolatorLinear<float>();
	cimg_library::CImg<float> warped;
	ImgTool::warp2D(img, flow, warped, interpolator);
	disp.display(moved);
	disp.wait(wTime);
	disp.display(warped);
	disp.wait(wTime);
	cimg_library::CImg<float>diff =
		(moved-warped).get_crop(5,5,img.width()-6,img.height()-6);
	std::cout << "warping...      " << std::flush;
	std::cout << "diff.mean() : " << std::abs(diff.mean()) << std::endl;
	assert(std::abs(diff.mean()) < std::numeric_limits<float>::min());

	// warp back
	ImgTool::warp2D(warped, flow, warped, interpolator, -1.0);
	delete interpolator;
	disp.display(warped);
	disp.wait(wTime);
	diff = (warped - img).get_crop(5,5,img.width()-6,img.height()-6);
	std::cout << "warping back... " << std::flush;
	std::cout << "diff.mean() : " << std::abs(diff.mean()) << std::endl;
	assert(std::abs(diff.mean()) < std::numeric_limits<float>::min());

    cimg_library::CImg<float> integral;
    ImgTool::integralImage2D(img, integral);
	disp.display(integral);
    disp.wait(wTime);

    cimg_library::CImg<double> noise;
    ImgTool::createAllScaleRandomPattern (noise, 100, 100, 0.5, 3);
    disp.assign(noise, "noise", 1, false, false);
    disp.display(noise);
    disp.wait(wTime);

    cimg_library::CImg<double> out;
    ImgTool::grayToHsv(noise, out);
    disp.display(out);
    disp.wait(wTime);

    ImgTool::chessBoard2D(20, 20, noise);
    disp.display(noise);
    disp.wait(wTime);

    return 0;
}
