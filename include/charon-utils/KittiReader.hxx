/*  Copyright (C) 2012 Jens-Malte Gottfried

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
/** \file KittiReader.hxx
 *  Implementation of the parameter class KittiReader.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.08.2012
 */

#ifndef _KITTIREADER_HXX_
#define _KITTIREADER_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "KittiReader.h"
#include "kitti-devkit/io_flow.h"
#include "kitti-devkit/io_disp.h"
#include <iomanip>
#include <cassert>

template <typename T>
KittiReader<T>::KittiReader(const std::string& name) :
		TemplatedParameteredObject<T>(
			"KittiReader", name,
			"<h2>Reads KITTI flow data</h2><br>"
			"reads the flow files from the KITTI stereo and optical flow "
			"benchmarks<br>"
			"<br>"
			"Image files from this benchmark are plain grayscale PNG "
			"files.<br>"
			"Flow and disparity data are also stored in PNG files with a "
			"convention how to convert the integer values into floats. This "
			"conversion is done in this plugin. I uses the devkit from the "
			"benchmark website.<br>"
			"See <a "
			"href=\"http://www.cvlibs.net/datasets/kitti/eval_stereo_flow.php?benchmark=flow\">"
			"KITTI Vision Benchmark Suite</a> for more information.<br><br>"
			"This module reads the image data and (if available) the "
			"corresponding ground truth data.<br>"
			"It's possible to select between the stereo or optical flow pairs, "
			"as well as between occluded and non-occluded ground truth."
		)
{
	ParameteredObject::_addOutputSlot(seq, "seq", "image pair", "CImgList<T>");
	ParameteredObject::_addOutputSlot(gt_occ, "gt_occ",
		"ground truth (occlusions)", "CImgList<T>");
	ParameteredObject::_addOutputSlot(gt_noc, "gt_noc",
		"ground truth (no occlusions)", "CImgList<T>");
	ParameteredObject::_addOutputSlot(valid_occ, "valid_occ",
		"valid ground truth mask (to gt_occ)", "CImgList<T>");
	ParameteredObject::_addOutputSlot(valid_noc, "valid_noc",
		"valid ground truth mask (to gt_noc)", "CImgList<T>");

	ParameteredObject::_addParameter(
		folder, "folder",
		"base folder (containing at least the folders image_0 and image_1)",
		"Path");
	ParameteredObject::_addParameter<std::string>(
		kind, "kind",
		"select stereo or flow data",
		"flow", "{flow;stereo}");
	ParameteredObject::_addParameter(index, "index", "image pair index", 0u);
	ParameteredObject::_addParameter(interp, "interp",
		"interpolate background", true);
}

template <typename T>
void KittiReader<T>::execute() {
	if (index() >= 1e7) {
		ParameteredObject::raise("Index out of range (max: 999999)");
	}
	std::ostringstream strm;
	strm << folder() << "/image_0/" << std::setfill('0') << std::setw(6)
		<< index() << "_10.png";
	std::string imFirst = strm.str();
	std::string imSecond = imFirst, imGTo = imFirst, imGTn = imFirst;
	if (kind() == "flow") {
		imSecond.replace(imSecond.length()-5,1,1,'1');
		imGTo.replace(imGTo.length()-21,7,"flow_occ");
		imGTn.replace(imGTn.length()-21,7,"flow_noc");
	}
	else if (kind() == "stereo") {
		imSecond.replace(imSecond.length()-15,1,1,'1');
		imGTo.replace(imGTo.length()-21,7,"disp_occ");
		imGTn.replace(imGTn.length()-21,7,"disp_noc");
	}
	else {
		ParameteredObject::raise("invalid kind given: " + kind());
	}

	cimg_library::CImg<T> ciFirst, ciSecond;
	ciFirst.load_png(imFirst.c_str());
	ciSecond.load_png(imSecond.c_str());
	assert(ciFirst.is_sameXYZC(ciSecond));
	assert(ciFirst.is_sameZC(1,1));
	seq().clear();
	seq().push_back(ciFirst.get_append(ciSecond, 'c'));

	try {
		if (kind() == "flow") {
			FlowImage kitGTo(imGTo), kitGTn(imGTn);
			assert(ciFirst.is_sameXY(kitGTo.width(),kitGTo.height()));
			assert(ciFirst.is_sameXY(kitGTn.width(),kitGTn.height()));
			gt_occ().assign(2,ciFirst.width(),ciFirst.height(),1,1);
			gt_noc().assign(2,ciFirst.width(),ciFirst.height(),1,1);
			valid_occ().assign(1,ciFirst.width(),ciFirst.height(),1,1);
			valid_noc().assign(1,ciFirst.width(),ciFirst.height(),1,1);
			cil::CImg<T>&uo=gt_occ()[0], &vo=gt_occ()[1], & oo=valid_occ()[0];
			cil::CImg<T>&un=gt_noc()[0], &vn=gt_noc()[1], & on=valid_noc()[0];
			cimg_forXY(oo,xx,yy) {
				oo(xx,yy) = kitGTo.isValid(xx,yy);
				on(xx,yy) = kitGTn.isValid(xx,yy);
			}
			if (interp()) {
				kitGTo.interpolateBackground();
				kitGTn.interpolateBackground();
			}
			cimg_forXY(uo,xx,yy) {
				uo(xx,yy) = kitGTo.getFlowU(xx,yy);
				vo(xx,yy) = kitGTo.getFlowV(xx,yy);
				un(xx,yy) = kitGTn.getFlowU(xx,yy);
				vn(xx,yy) = kitGTn.getFlowV(xx,yy);
			}
		}
		else {
			DisparityImage kitGTo(imGTo), kitGTn(imGTn);
			assert(ciFirst.is_sameXY(kitGTo.width(),kitGTo.height()));
			assert(ciFirst.is_sameXY(kitGTn.width(),kitGTn.height()));
			gt_occ().assign(1,ciFirst.width(),ciFirst.height(),1,1);
			gt_noc().assign(1,ciFirst.width(),ciFirst.height(),1,1);
			valid_occ().assign(1,ciFirst.width(),ciFirst.height(),1,1);
			valid_noc().assign(1,ciFirst.width(),ciFirst.height(),1,1);
			cimg_library::CImg<T>& dio=gt_occ()[0], & oo=valid_occ()[0];
			cimg_library::CImg<T>& din=gt_noc()[0], & on=valid_noc()[0];
			cimg_forXY(oo,xx,yy) {
				oo(xx,yy) = kitGTo.isValid(xx,yy);
				on(xx,yy) = kitGTn.isValid(xx,yy);
			}
			if(interp()) {
				kitGTo.interpolateBackground();
				kitGTn.interpolateBackground();
			}
			cimg_forXY(dio,xx,yy) {
				dio(xx,yy) = kitGTo.getDisp(xx,yy);
				din(xx,yy) = kitGTn.getDisp(xx,yy);
			}
		}
	}
	catch (...) {
		sout << "(II) \tNo ground truth found." << std::endl;
	}
}

#endif /* _KITTIREADER_HXX_ */
