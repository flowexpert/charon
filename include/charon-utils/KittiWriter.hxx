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
		)
{
	ParameteredObject::_addInputSlot(
		result, "result", "result input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
		valid, "valid", "valid pixel mask input", "CImgList<T>");

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

	if (flow) {
		const cimg_library::CImg<T>& u=result()[0], &v=result()[1];
		assert(u.is_sameXYZC(v));
		assert(u.is_sameZC(1,1));
		FlowImage kitRes(u.width(),u.height());
		cimg_forXY(u,xx,yy) {
			kitRes.setFlowU(xx,yy,u(xx,yy));
			kitRes.setFlowV(xx,yy,v(xx,yy));
		}
		if (valid.connected()) {
			assert(valid().size() > 0);
			const cimg_library::CImg<T>& o=valid()[0];
			assert(o.is_sameXY(u));
			cimg_forXY(o,xx,yy) {
				kitRes.setValid(xx,yy,(o(xx,yy)>0));
			}
		}
		kitRes.write(nRes);
		kitRes.writeColor(nFCol);
	}
	else {
		const cimg_library::CImg<T>& u=result()[0];
		assert(u.is_sameZC(1,1));
		DisparityImage kitRes(u.width(),u.height());
		cimg_forXY(u,xx,yy) {
			kitRes.setDisp(xx,yy,u(xx,yy));
		}
		if (valid.connected()) {
			assert(valid().size() > 0);
			const cimg_library::CImg<T>& o=valid()[0];
			assert(o.is_sameXY(u));
			cimg_forXY(o,xx,yy) {
				if (o(xx,yy) <= 0) {
					kitRes.setInvalid(xx,yy);
				}
			}
		}
		kitRes.write(nRes);
		kitRes.writeColor(nFCol);
	}
}

#endif /* _KITTIWRITER_HXX_ */
