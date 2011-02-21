/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file flowToHSV.cpp
 *  Unit tests for class FlowToHSV
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de>
 *      Jens-Malte Gottfried </a>
 *  \date 07.01.2011
 */

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

#include <charon-core/ExceptionHandler.h>
#include <charon-utils/FileReader.h>
#include <charon-utils/Flow2HSV.h>
#include<limits>

int test() {
	sout.assign(std::cout);

	std::cout << "-- Create test flow image" << std::endl;
	cimg_library::CImgList<float> test(2,201,201,1,1,0.f);
	cimg_forXYZC(test[0u],xx,yy,zz,tt) {
		test(0u,xx,yy,zz,tt)=(float)xx-101.f;
		test(1u,xx,yy,zz,tt)=(float)yy-101.f;
	}
	test.save_cimg("testFlow.cimg",true);

	FileReader<float> reader;
	reader.filename = "testFlow.cimg";
	Flow2HSV<float> viz;
	reader.out.connect(viz.flow);
	viz.execute();
	assert(viz.out().size() == 1u);
	const cimg_library::CImg<float>& o = viz.out()[0];
	o.save_cimg("flow2hsvColorwheel.cimg",true);
#ifndef _MSC_VER
#ifdef cimg_use_png
	o.save_png("flow2hsvColorwheel.png",1u);
#endif
#endif

	std::cout << "-- Check for steps at quadrant borders" << std::endl;
	cimg_library::CImg<float> a1,a2,b1,b2,c1,c2,d1,d2;
	float s1,s2,s3,s4;
	a1 = o.vector(o(  0,100,0,0),o(  0,100,0,1),o(  0,100,0,2));
	a2 = o.vector(o(  0,101,0,0),o(  0,101,0,1),o(  0,101,0,2));
	s1 = (a1-a2).norm().max();

	b1 = o.vector(o(100,  0,0,0),o(100,  0,0,1),o(100,  0,0,2));
	b2 = o.vector(o(101,  0,0,0),o(101,  0,0,1),o(101,  0,0,2));
	s2 = (b1-b2).norm().max();

	c1 = o.vector(o(200,100,0,0),o(200,100,0,1),o(200,100,0,2));
	c2 = o.vector(o(200,101,0,0),o(200,101,0,1),o(200,101,0,2));
	s3 = (c1-c2).norm().max();

	d1 = o.vector(o(100,200,0,0),o(100,200,0,1),o(100,200,0,2));
	d2 = o.vector(o(101,200,0,0),o(101,200,0,1),o(101,200,0,2));
	s4 = (d1-d2).norm().max();

	std::cout << "left: " << s1 << std::endl;
	std::cout << "top: " << s2 << std::endl;
	std::cout << "right: " << s3 << std::endl;
	std::cout << "bottom: " << s4 << std::endl;

	assert(s1 < 10.f);
	assert(s2 < 10.f);
	assert(s3 < 10.f);
	assert(s4 < 10.f);

	return EXIT_SUCCESS;
}

int main() {
	return ExceptionHandler::run(test);
}
