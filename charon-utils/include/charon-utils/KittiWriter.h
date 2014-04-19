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
/** \file KittiWriter.h
 *  Declaration of the parameter class KittiWriter.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.08.2012
 */

#ifndef _KITTIWRITER_H_
#define _KITTIWRITER_H_

#ifdef _MSC_VER
#ifdef kittiwriter_EXPORTS
/// Visual C++ specific code
#define kittiwriter_DECLDIR __declspec(dllexport)
#else
#define kittiwriter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define kittiwriter_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// writes flow or disparity output like in the KITTI stereo and optical flow
/// benchmarks
/** result writer for KITTI stereo and optical flow benchmarks
 *  
 *  Flow/disparity is determined by the number of image list items
 *  (1=disparity, 2=flow).
 *
 *  Files to submit are written into the given folder and named like
 *  000123_10.png (as requested to submit the results).
 *  False color representations are written into the same folder named like
 *  fcol000123.png
 *  Delete these files before creating the zip file for submission.
 */
template <typename T>
class kittiwriter_DECLDIR KittiWriter :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	KittiWriter(const std::string& name = "");

	/// result input
	InputSlot< cimg_library::CImgList<T> > result;
	/// valid pixel mask input
	InputSlot< cimg_library::CImgList<T> > valid;
	/// ground truth (with occlusions, for error check)
	InputSlot< cimg_library::CImgList<T> > gt_occ;
	/// ground truth (without occlusions, for error check)
	InputSlot< cimg_library::CImgList<T> > gt_noc;
	/// ground truth valid (for gt_occ)
	InputSlot< cimg_library::CImgList<T> > gtvalid_occ;
	/// ground truth valid (for gt_noc)
	InputSlot< cimg_library::CImgList<T> > gtvalid_noc;

	/// path to output folder
	Parameter< std::string > path;
	/// index of output files
	Parameter< unsigned int > index;

	/// Update object.
	virtual void execute();
};

#endif // _KITTIWRITER_H_
