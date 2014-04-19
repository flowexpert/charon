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
/** \file Warptest.cpp
 *  Unit tests for class Warp
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de>
 *      Jens-Malte Gottfried </a>
 *  \date 12.11.2010
 */

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

// needed CPP definitions
#ifndef PENGUINFILE
/// test image file
#define PENGUINFILE
#error PENGUINFILE not defined!
#endif

#include <charon-core/ExceptionHandler.h>
#include <charon-utils/FileReader.h>
#include <charon-utils/InterpolatorLinear.hxx>
#include <charon-utils/Warp.h>
#include <charon-utils/WarpSymmetric.h>
#include <limits>

void test() {
	sout.assign(std::cout);

	// create objects, establish connections
	std::cout << "-- Create objects" << std::endl;
	FileReader<double> imgObj, flowObj;
	Warp<double> warp;
	InterpolatorLinear<double> interp;
	imgObj.filename = PENGUINFILE;
	flowObj.filename = PENGUINFILE;
	imgObj.out.connect(warp.seqInput);
	flowObj.out.connect(warp.flowInput);
	interp.out.connect(warp.interpolator);

	// initialize flow
	std::cout << "-- Load test data" << std::endl;
	imgObj.run();
	flowObj.run();
	cimg_library::CImgList<double>& seq = imgObj.out();
	assert(seq.size() == 1);
	cimg_library::CImgList<double>& flow = flowObj.out();

	unsigned int dx = seq[0].width();
	unsigned int dy = seq[0].height();
	unsigned int dz = seq[0].depth();
	unsigned int dt = seq[0].spectrum();
	assert(dx >  1u); assert(dy >  1u);
	assert(dz == 1u); assert(dt == 1u);
	flow.assign(2, dx, dy, 1, 1);
	flow[0].fill(1);
	flow[1].fill(-2);

	// check simple warping
	std::cout << "-- Simple warping" << std::endl;
	warp.run();
	const cimg_library::CImgList<double>& res = warp.out();
	assert(res.size() == 1u);
	cimg_library::CImg<double> orig = seq[0], store = res[0];
	cimg_library::CImg<double> tmp = res[0];
	tmp.shift(1, -2);
	tmp -= seq[0];
	//(orig,store,tmp).display();
	tmp.crop(5, 5, dx-6, dy-6);
	double diffsum = tmp.abs().sum();
	if (diffsum > std::numeric_limits<double>::min()) {
		std::cerr << __FILE__ << ":" << __LINE__ << ":\n\t"
		<< "Simple Warping failed: diff=" << diffsum << std::endl;
		exit(EXIT_FAILURE);
	}
	tmp.clear();

	// check influence of weight
	std::cout << "-- Warping with weight 2" << std::endl;
	warp.weight = 2.f;
	warp.resetExecuted();
	warp.run();
	tmp = res[0];
	tmp.shift(2, -4);
	tmp -= seq[0];
	tmp.crop(5, 5, dx-6, dy-6);
	assert(tmp.abs().sum() <= std::numeric_limits<double>::min());
	tmp.clear();

	std::cout << "-- Warping with weight -2" << std::endl;
	warp.weight = -2.f;
	warp.resetExecuted();
	warp.run();
	tmp = res[0];
	tmp.shift(-2, 4);
	tmp -= seq[0];
	tmp.crop(5, 5, dx-6, dy-6);
	assert(tmp.abs().sum() <= std::numeric_limits<double>::min());
	tmp.clear();

	// check warping if input seq has 2 images
	std::cout << "-- Sequence warping" << std::endl;
	warp.weight = 1.f;
	seq[0].append(seq[0], 'c');
	warp.resetExecuted();
	warp.run();
	assert(res[0].spectrum() == 2u);
	// first one should stay untouched, second one is warped as above
	assert((res[0].get_channel(0u) - orig).abs().sum() <=
			std::numeric_limits<double>::min());
	assert((res[0].get_channel(1u) - store).abs().sum() <=
			std::numeric_limits<double>::min());

	seq[0]=orig.get_append(orig.get_shift(1,-2),'c');
	warp.resetExecuted();
	warp.run();
	tmp = res[0].get_crop(5,5,dx-6,dx-6);
	assert((tmp.get_channel(0)-tmp.get_channel(1)).abs().sum() <=
			std::numeric_limits<double>::min());

	// tests for WarpSymmetric
	std::cout << "-- Symmetric warping" << std::endl;
	WarpSymmetric<double> warpSymm;
	imgObj.out.connect(warpSymm.seqInput);
	flowObj.out.connect(warpSymm.flowInput);
	interp.out.connect(warpSymm.interpolator);

	seq[0]=orig.get_append(orig.get_shift(1,-2),'c');
	warpSymm.run();

	// temp(.,.,.,0) and temp(.,.,.,1) should now be different
	// from orig and store because both are warped half-way.
	const cimg_library::CImgList<double>& resSymm = warpSymm.out();
	tmp = resSymm[0];
	assert((tmp.get_shared_channel(0)-orig).abs().sum()
			> std::numeric_limits<double>::min());
	assert((tmp.get_shared_channel(0)-store).abs().sum()
			> std::numeric_limits<double>::min());
	assert((tmp.get_shared_channel(1)-orig).abs().sum()
			> std::numeric_limits<double>::min());
	assert((tmp.get_shared_channel(1)-store).abs().sum()
			> std::numeric_limits<double>::min());
	// but they should not differer from each other in the central region
	tmp.crop(5,5,dx-6,dx-6);
	assert((tmp.get_shared_channel(0)-tmp.get_shared_channel(1)).abs().sum()
			<= std::numeric_limits<double>::min());
}

int main() {
	return ExceptionHandler::run(test);
}
