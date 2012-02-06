/*  Copyright (C) 2012 Stephan Meister

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
/** \file Size.h
 *  Declaration of the parameter class Size.
 *  \author <a href="mailto:stephan.meister@iwr.uni-heidelberg.de">
 *      Stephan Meister</a>
 *  \date 03.02.2012
 */

#ifndef _SIZE_H_
#define _SIZE_H_

#ifdef _MSC_VER
#ifdef size_EXPORTS
/// Visual C++ specific code
#define size_DECLDIR __declspec(dllexport)
#else
#define size_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define size_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#define roi_EXPORTS
#include <charon-utils/Roi.h>
#include <charon-utils/CImg.h>

/// Size
/** get Size of input object and export is as an ROI instance (e.g. for
 *  further use in the crop or resize plugins)
 */
template <typename T>
class size_DECLDIR Size :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	Size(const std::string& name = "");

	/// input image
	InputSlot< cimg_library::CImgList<T> > image;
	/// offset ROI
	InputSlot< Roi<int>* > offset;

	/// ROI(Region of Interest)
	OutputSlot< Roi<int>* > roi;

	/// resize Factor
	Parameter< double > resizeFactor;

	/// Update object.
	virtual void execute();

private:

	Roi<int> _roi ;
};

#endif // _SIZE_H_
