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

#include <charon-utils/Roi.h>
#include <charon-utils/CImg.h>

/// Image %Size
/** Get size of input object and export is as an ROI instance
 *  (e.g. for further use in the crop or resize plugins).
 *
 *  <h4>Usage example:</h4>
 *  If you want to double the size of an image,
 *  connect it to the image input slot of this plugin
 *  and to the input of an resize plugin.
 *  Leave the offset slot unconnected,set resize factor to 2.0
 *  and connect the roi output to the roi input of the resize plugin.
 *
 *  <h4>Using with PetscSolver</h4>
 *  This module may also be used to determine the image sequence
 *  dimensions to be passed to the PetscSolver in optical flow algorithms.
 *  Typically, the input sequence consists of (at least) two consecutive
 *  frames (i.e. the <tt>t</tt> dimension is of size two) to calculate one
 *  flow frame. So you have to subtract one frame from the <tt>t</tt>
 *  dimension, e.g. passing an offset roi with just zeros but
 *  one as <tt>tEnd</tt>.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup charon-helpers
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
	/** If connected all starting values in this ROI will be added to the
	 *  resulting ROI and all end values will be subtraced
	 *  from the resulting ROI.
	 *  The "resize factor" parameter is ignored when this slot is connected.
	 *
	 *  Usefull in combination with the cut plugin:
	 *  To remove a fixed with border around an image set the values in
	 *  offset to positive,"
	 *  or to add a black border around an image set the values to negative.
	 */
	InputSlot< Roi<int>* > offset;

	/// ROI(Region of Interest)
	OutputSlot< Roi<int>* > roi;

	/// resize Factor
	/** The Size of the resulting ROI will be multiplied with this factor
	 *  (when offset is not connected).
	 *
	 *  Usefull if you want to resize an image by a fixed factor.
	 */
	Parameter< double > resizeFactor;

	/// Update object.
	virtual void execute();

private:

	/// ROI instance to export via roi OutputSlot
	Roi<int> _roi ;
};

#endif // _SIZE_H_
