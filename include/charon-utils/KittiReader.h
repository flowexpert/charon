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
/** \file KittiReader.h
 *  Declaration of the parameter class KittiReader.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.08.2012
 */

#ifndef _KITTIREADER_H_
#define _KITTIREADER_H_

#ifdef _MSC_VER
#ifdef kittireader_EXPORTS
/// Visual C++ specific code
#define kittireader_DECLDIR __declspec(dllexport)
#else
#define kittireader_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define kittireader_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>
#include <charon-utils/CImg.h>

/// Reads KITTI flow data
/** reads the flow files from the KITTI stereo and optical flow benchmarks
 *  
 *  Image files from this benchmark are plain grayscale PNG files.
 *  Flow and disparity data are also stored in PNG files with a convention how
 *  to convert the integer values into floats. This conversion is done in this
 *  plugin. I uses the devkit from the benchmark website.
 *  See <a
 *  href="http://www.cvlibs.net/datasets/kitti/eval_stereo_flow.php?benchmark=flow">KITTI Vision Benchmark Suite</a> for more information.This module reads the image data and (if available) the corresponding ground truth data.It's possible to select between the stereo or optical flow pairs, as well as between occluded and non-occluded ground truth.
 */
template <typename T>
class kittireader_DECLDIR KittiReader :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	KittiReader(const std::string& name = "");

	/// image pair
	OutputSlot< cimg_library::CImgList<T> > seq;
	/// flow 1 (usually from gt_occ)
	OutputSlot< cimg_library::CImgList<T> > flow1;
	/// flow 2 (usually from gt_noc)
	OutputSlot< cimg_library::CImgList<T> > flow2;
	/// valid mask (to gt1)
	OutputSlot< cimg_library::CImgList<T> > valid1;
	/// valid mask (to gt2)
	OutputSlot< cimg_library::CImgList<T> > valid2;

	/// select stereo or flow data
	Parameter< std::string > kind;
	/// base folder (containing at least the folder image_0)
	Parameter< std::string > folder;
	/// path to flow1 (in disparity mode, this should be disp_occ)
	Parameter < std::string > pf1;
	/// path to flow2 (in disparity mode, this should be disp_noc)
	Parameter < std::string > pf2;
	/// image pair index
	Parameter< unsigned int > index;
	/// interpolate background
	Parameter< bool > interp;

protected:
	/// Update object.
	virtual void execute();
};

#endif // _KITTIREADER_H_
