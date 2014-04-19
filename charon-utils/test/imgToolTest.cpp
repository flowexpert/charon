/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// @file imgToolTest.cpp
/// Some tests for ImgTool usage.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @date 16.06.2009

#include <charon-utils/ImgTool.hxx>
#include <charon-utils/InterpolatorLinear.hxx>

#ifndef PENGUINFILE
#error PENGUINFILE not defined!
#define PENGUINFILE
#endif
#ifndef TESTDIR
#error TESTDIR undefined!
#define TESTDIR
#endif

int main() {
	const float epsilon = 10.e-5f;
	bool success = true;
	cimg_library::CImg<float> img(PENGUINFILE);
	cimg_library::CImg<float> moved = img.get_crop(3,-2,0,0,
		img.width()+2,img.height()-3,img.depth()-1,img.spectrum()-1);

	// test warping
	cimg_library::CImgList<float> flow(2, img.width(), img.height(),
		img.depth(), img.spectrum());
	flow[0].fill(3.f);
	flow[1].fill(-2.f);
	Interpolator<float>* interpolator = new InterpolatorLinear<float>();
	cimg_library::CImg<float> warped;
	ImgTool::warp2D(img, flow, warped, interpolator);
	cimg_library::CImg<float>diff =
		(moved-warped).get_crop(5,5,img.width()-6,img.height()-6);
	std::cout << "warping...      " << std::flush;
	std::cout << "diff.mean() : " << std::abs(diff.mean()) << std::endl;
	if (std::abs(diff.mean()) > epsilon) {
		std::cerr << "Warping failed!" << std::endl;
		success = false;
	}

	// warp back
	ImgTool::warp2D(warped, flow, warped, interpolator, -1.0);
	delete interpolator;
	diff = (warped - img).get_crop(5,5,img.width()-6,img.height()-6);
	std::cout << "warping back... " << std::flush;
	std::cout << "diff.mean() : " << std::abs(diff.mean()) << std::endl;
	if (diff.abs().mean() > epsilon) {
		std::cerr << "Warping back failed!" << std::endl;
		success = false;
	}

	cimg_library::CImg<float> check;
	cimg_library::CImg<float> integral;
	ImgTool::integralImage2D(img, integral);
	integral.save("PenguinIntegral.cimg");
	check.load(TESTDIR "/PenguinIntegral.cimg");
	if ((integral-check).abs().mean() > epsilon) {
		std::cerr << "Integral image failed!" << std::endl;
		success = false;
	}

	cimg_library::CImg<float> out;
	ImgTool::grayToHsv(img, out);
	out.save("IntegralHsv.cimg");
	check.load(TESTDIR "/IntegralHsv.cimg");
	float err = (out-check).abs().mean();
	if (err > epsilon) {
		std::cerr << "Gray to HSV failed!" << std::endl;
		success = false;
	}

	ImgTool::chessBoard2D(20, 20, integral);
	integral.save("chessboard.cimg");
	check.load(TESTDIR "/chessboard.cimg");
	if ((integral-check).abs().mean() > epsilon) {
		std::cerr << "Chessboard failed!" << std::endl;
		success = false;
	}

	cimg_library::CImg<float> randPat;
	ImgTool::createAllScaleRandomPattern(randPat, 100, 100, 1.f, 5);
	randPat.save("randomPattern.cimg");
#ifdef cimg_use_png
	randPat.save_png("randomPattern.png");
#endif

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
