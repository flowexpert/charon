/*  Copyright (C) 2012 Jens-Malte Gottfried

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
/** \file KittiWriter.hxx
 *  Implementation of the parameter class KittiWriter.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.08.2012
 */

#ifndef _KITTIWRITER_HXX_
#define _KITTIWRITER_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "KittiWriter.h"
#include "kitti-devkit/io_flow.h"
#include "kitti-devkit/io_disp.h"
#include <iomanip>

template <typename T>
KittiWriter<T>::KittiWriter(const std::string& name) :
		TemplatedParameteredObject<T>(
			"KittiWriter", name,
			"<h2>writes flow or disparity output like in the KITTI stereo "
			"and optical flow benchmarks</h2><br>"
			"result writer for KITTI stereo and optical flow benchmarks<br>"
			"<br>"
			"Flow/disparity is determined by the number of image list items "
			"(1=disparity, 2=flow).<br>"
			"Files to submit are written into the given folder and named "
			"like 000123_10.png (as requested to submit the results).<br>"
			"False color representations are written into the same folder "
			"named like fcol000123.png<br>"
			"Delete these files before creating the zip file for submission."
			"<br><br>"
			"This plugin uses png++ by Alexander Shulgin<br>"
			"see include/png++/COPYING for copyright and license information."
		),
		valid(true,false), // optional
		gt_occ(true,false),
		gt_noc(true,false),
		gtvalid_occ(true,false),
		gtvalid_noc(true,false)
{
	ParameteredObject::_setTags("charon-utils;CImg;DiskIO") ;

	ParameteredObject::_addInputSlot(
		result, "result", "result input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
		valid, "valid", "valid pixel mask input", "CImgList<T>");
	ParameteredObject::_addInputSlot(gt_occ, "gt_occ",
		"ground truth (with occlusions) for error check", "CImgList<T>");
	ParameteredObject::_addInputSlot(gt_noc, "gt_noc",
		"ground truth (without occlusions) for error check", "CImgList<T>");
	ParameteredObject::_addInputSlot(gtvalid_occ, "gtvalid_occ",
		"ground truth valid (for gt_occ)", "CImgList<T>");
	ParameteredObject::_addInputSlot(gtvalid_noc, "gtvalid_noc",
		"ground truth valid (for gt_noc)", "CImgList<T>");

	ParameteredObject::_addParameter(
		path, "path", "path to output folder", "Path");
	ParameteredObject::_addParameter< unsigned int >(
		index, "index", "index of output files", 0u);
}

template <typename T>
void KittiWriter<T>::execute() {
	const bool flow = (result().size() > 1);
	if (result().size() > 2) {
		sout << "(WW) KITTI format does not allow more than 2 dimensions.\n"
			<< "(WW) u and v komponent will be stored, others discarded."
			<< std::endl;
	}
	if (result().size() < 1) {
		ParameteredObject::raise("invalid result dimensions (no list items)");
	}
	std::ostringstream strm;
	strm << path() << "/fcol" << std::setfill('0') << std::setw(6)
		<< index() << "_10.png";
	std::string nRes = strm.str(), nFCol = strm.str();
	nRes.erase(nRes.length()-17,4);
	nFCol.erase(nFCol.length()-7,3);
	std::string nErr = nFCol;
	nErr.replace(nErr.length()-14,4,"err");

	if (flow) {
		const cimg_library::CImg<T>& u=result()[0], &v=result()[1];
		assert(u.is_sameXYZC(v));
		assert(u.is_sameZC(1,1));
		FlowImage kitRes(u.width(),u.height());
		cimg_forXY(u,xx,yy) {
			kitRes.setValid(xx,yy,true);
			kitRes.setFlowU(xx,yy,u(xx,yy));
			kitRes.setFlowV(xx,yy,v(xx,yy));
		}
		if (valid.connected()) {
			assert(valid().size() == 1);
			const cimg_library::CImg<T>& o=valid()[0];
			assert(o.is_sameXY(u));
			cimg_forXY(o,xx,yy) {
				kitRes.setValid(xx,yy,(o(xx,yy)>0));
			}
		}
		sout << "(DD) \tmax flow: " << kitRes.maxFlow() << std::endl;
		sout << "(DD) \twriting " << nRes << std::endl;
		kitRes.write(nRes);
		sout << "(DD) \twriting " << nFCol << std::endl;
		kitRes.writeColor(nFCol);
		if (gt_occ.connected() && gt_noc.connected()) {
			assert(gt_occ().size()>=2);
			assert(gt_noc().size()>=2);
			const cimg_library::CImg<T> &guo=gt_occ()[0], &gvo=gt_occ()[1];
			const cimg_library::CImg<T> &gun=gt_noc()[0], &gvn=gt_noc()[1];
			assert(u.is_sameXYZC(guo));
			assert(v.is_sameXYZC(gvo));
			assert(u.is_sameXYZC(gun));
			assert(v.is_sameXYZC(gvn));
			FlowImage kitGto(u.width(),u.height());
			FlowImage kitGtn(u.width(),u.height());
			cimg_forXY(u,xx,yy) {
				kitGto.setValid(xx,yy,true);
				kitGto.setFlowU(xx,yy,guo(xx,yy));
				kitGto.setFlowV(xx,yy,gvo(xx,yy));
				kitGtn.setValid(xx,yy,true);
				kitGtn.setFlowU(xx,yy,gun(xx,yy));
				kitGtn.setFlowV(xx,yy,gvn(xx,yy));
			}
			if (gtvalid_occ.connected() && gtvalid_noc.connected()) {
				assert(gtvalid_occ().size() == 1);
				assert(gtvalid_noc().size() == 1);
				const cimg_library::CImg<T>& oo=gtvalid_occ()[0];
				const cimg_library::CImg<T>& on=gtvalid_noc()[0];
				assert(oo.is_sameXY(guo));
				assert(on.is_sameXY(gun));
				cimg_forXY(oo,xx,yy) {
					kitGto.setValid(xx,yy,(oo(xx,yy)>0));
					kitGtn.setValid(xx,yy,(on(xx,yy)>0));
				}
			}
			sout << "(DD) \twriting " << nErr << std::endl;
			kitRes.errorImage(kitGtn,kitGto).write(nErr.c_str());
		}
	}
	else {
		const cimg_library::CImg<T>& u=result()[0];
		assert(u.is_sameZC(1,1));
		DisparityImage kitRes(u.width(),u.height());
		cimg_forXY(u,xx,yy) {
			kitRes.setDisp(xx,yy,u(xx,yy));
		}
		if (valid.connected()) {
			assert(valid().size() == 1);
			const cimg_library::CImg<T>& o=valid()[0];
			assert(o.is_sameXY(u));
			assert(o.is_sameZC(1,1));
			cimg_forXY(o,xx,yy) {
				if (o(xx,yy) <= 0) {
					kitRes.setInvalid(xx,yy);
				}
			}
		}
		kitRes.write(nRes);
		sout << "(DD) \twriting " << nRes << std::endl;
		kitRes.writeColor(nFCol);
		sout << "(DD) \twriting " << nFCol << std::endl;
	}
}

#endif /* _KITTIWRITER_HXX_ */
