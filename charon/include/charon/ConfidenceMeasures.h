/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

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
/** \file ConfidenceMeasures.h
 *  Declaration of the parameter class ConfidenceMeasures.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 21.03.2013
 */

#ifndef _CONFIDENCEMEASURES_H_
#define _CONFIDENCEMEASURES_H_

#ifdef _MSC_VER
#ifdef confidencemeasures_EXPORTS
/// Visual C++ specific code
#define confidencemeasures_DECLDIR __declspec(dllexport)
#else
#define confidencemeasures_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define confidencemeasures_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// This module calculates some confidence measures
/** This module calculates some confidence measures.
 *  
 *  These measures are \Psi_{BOOTG}, \Psi_{BOOTGM} and \Psi_{BOOTA}
 *  as proposed by <i>Jan Kybic</i> and <i>Claudia Nieuwenhuis</i> within
 *  their 2011 paper <b>Bootstrap Optical Flow Confidence and Uncertainty
 *  Measure</b>.
 */
template <typename T>
class confidencemeasures_DECLDIR ConfidenceMeasures :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	ConfidenceMeasures(const std::string& name = "");

	/// multi-slot for motion-fields
	InputSlot< cimg_library::CImgList<T> > motions;

	// motion mean output
	OutputSlot< cimg_library::CImgList<T> > mean;
	// motion variance output
	OutputSlot< cimg_library::CImgList<T> > variance;

	/// bootG confidence measure
	OutputSlot< cimg_library::CImgList<T> > bootG;
	/// bootGM confidence measure
	OutputSlot< cimg_library::CImgList<T> > bootGM;
	/// bootA confidence measure
	OutputSlot< cimg_library::CImgList<T> > bootA;


protected:
	/// Update object.
	virtual void execute();
};

#endif // _CONFIDENCEMEASURES_H_

