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
	ParameteredObject::_setTags("charon-utils;CImg;I/O") ;

	ParameteredObject::_addOutputSlot(seq, "seq", "image pair", "CImgList<T>");
	ParameteredObject::_addOutputSlot(flow1, "flow1",
		"flow 1 (usually from gt_occ)", "CImgList<T>");
	ParameteredObject::_addOutputSlot(flow2, "flow2",
		"flow 2 (usually from gt_noc)", "CImgList<T>");
	ParameteredObject::_addOutputSlot(valid1, "valid1",
		"valid mask (of flow1) ", "CImgList<T>");
	ParameteredObject::_addOutputSlot(valid2, "valid2",
		"valid mask (of flow2)", "CImgList<T>");

	ParameteredObject::_addParameter(
		folder, "folder",
		"base folder (containing at least the folders image_0 and image_1)",
		"Path");
	ParameteredObject::_addParameter<std::string>(
		kind, "kind",
		"select stereo or flow data",
		"flow", "{flow;stereo}");
	ParameteredObject::_addParameter(index, "index", "image pair index", 0u);
	ParameteredObject::_addParameter<std::string>(
		pf1, "pf1", "path to flow1 (in disparity mode, "
		"this should be disp_occ)", "flow_occ");
	ParameteredObject::_addParameter<std::string>(
		pf2, "pf2", "path to flow2 (in disparity mode, "
		"this should be disp_noc)", "flow_noc");
	ParameteredObject::_addParameter(
		interp, "interp", "interpolate background", false);
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
	std::string imSecond = imFirst, imGT1 = imFirst, imGT2 = imFirst;
	if (kind() == "flow") {
		imSecond.replace(imSecond.length()-5,1,1,'1');
	}
	else if (kind() == "stereo") {
		imSecond.replace(imSecond.length()-15,1,1,'1');
	}
	else {
		ParameteredObject::raise("invalid kind given: " + kind());
	}
	imGT1.replace(imGT1.length()-21,7,pf1());
	imGT2.replace(imGT2.length()-21,7,pf2());

	try {
		cimg_library::CImg<T> ciFirst, ciSecond;
		ciFirst.load_png(imFirst.c_str());
		ciSecond.load_png(imSecond.c_str());
		assert(ciFirst.is_sameXYZC(ciSecond));
		assert(ciFirst.is_sameZC(1,1));
		seq().clear();
		seq().push_back(ciFirst.get_append(ciSecond, 'c'));
	}
	catch (const cimg_library::CImgException& e) {
		sout << "(WW) failed to load sequence:\n(WW) \t" << e.what();
		seq().clear();
	}

	if (kind() == "flow") {
		try {
			FlowImage kitF(imGT1);
			flow1().assign(2,kitF.width(),kitF.height(),1,1);
			valid1().assign(1,kitF.width(),kitF.height(),1,1);
			cil::CImg<T>&u=flow1()[0], &v=flow1()[1], & o=valid1()[0];
			cimg_forXY(o,xx,yy) {
				o(xx,yy) = kitF.isValid(xx,yy);
			}
			if (interp()) {
				kitF.interpolateBackground();
			}
			cimg_forXY(u,xx,yy) {
				u(xx,yy) = kitF.getFlowU(xx,yy);
				v(xx,yy) = kitF.getFlowV(xx,yy);
			}
		}
		catch(...) {
			sout << "(WW) failed to load flow 1" << std::endl;
			flow1().clear();
		}
		try {
			FlowImage kitF(imGT2);
			flow2().assign(2,kitF.width(),kitF.height(),1,1);
			valid2().assign(1,kitF.width(),kitF.height(),1,1);
			cil::CImg<T>&u=flow2()[0], &v=flow2()[1], & o=valid2()[0];
			cimg_forXY(o,xx,yy) {
				o(xx,yy) = kitF.isValid(xx,yy);
			}
			if (interp()) {
				kitF.interpolateBackground();
			}
			cimg_forXY(u,xx,yy) {
				u(xx,yy) = kitF.getFlowU(xx,yy);
				v(xx,yy) = kitF.getFlowV(xx,yy);
			}
		}
		catch(...) {
			sout << "(WW) failed to load flow 2" << std::endl;
			flow2().clear();
		}
	}
	else {
		if ((pf1() == "flow_occ") || (pf2() == "flow_noc")) {
			sout << "(WW) using flow directories in disparity mode\n"
				<< "(WW) check if pf1/pf2 sould be disp_occ/noc!"
				<< std::endl;
		}
		try {
			DisparityImage kitD(imGT1);
			flow1().assign(1,kitD.width(),kitD.height(),1,1);
			valid1().assign(1,kitD.width(),kitD.height(),1,1);
			cimg_library::CImg<T>& di=flow1()[0], & oo=valid1()[0];
			cimg_forXY(oo,xx,yy) {
				oo(xx,yy) = kitD.isValid(xx,yy);
			}
			if(interp()) {
				kitD.interpolateBackground();
			}
			cimg_forXY(di,xx,yy) {
				di(xx,yy) = kitD.getDisp(xx,yy);
			}
		}
		catch(...) {
			sout << "(WW) failed to load disparity 1" << std::endl;
			flow1().clear();
		}
		try {
			DisparityImage kitD(imGT2);
			flow2().assign(1,kitD.width(),kitD.height(),1,1);
			valid2().assign(1,kitD.width(),kitD.height(),1,1);
			cimg_library::CImg<T>& di=flow2()[0], & oo=valid2()[0];
			cimg_forXY(oo,xx,yy) {
				oo(xx,yy) = kitD.isValid(xx,yy);
			}
			if(interp()) {
				kitD.interpolateBackground();
			}
			cimg_forXY(di,xx,yy) {
				di(xx,yy) = kitD.getDisp(xx,yy);
			}
		}
		catch(...) {
			sout << "(WW) failed to load disparity 2" << std::endl;
			flow2().clear();
		}
	}
}

#endif /* _KITTIREADER_HXX_ */
