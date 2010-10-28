/*  This file is part of Charon.

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
/**
 *  @file BlockMatching.h
 *  @brief declaration of abstract base class BlockMatching
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef BLOCKMATCHING_H_
#define BLOCKMATCHING_H_

#if  defined(MSVC) && defined(HANDLE_DLL)
#ifdef blockmatching_EXPORTS
///Visual C++ specific code
#define blockmatching_DECLDIR __declspec(dllexport)
#else
#define blockmatching_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define blockmatching_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/Roi.h>
#include <charon-utils/CImg.h>
#include <charon-utils/Interpolator.h>
#include "Incrementor.h"
#include "ObjectiveFunction.h"
#include "SurfaceAnalysis.h"
#include "PixelSelection.h"
#include "BrightnessModel.h"
#include "MotionModel.h"
#include <vector>

/// This is the abstract base class of the declaration and implementation of the 
/// blockmatching algorithm

template<typename T>
class blockmatching_DECLDIR BlockMatching: public TemplatedParameteredObject<T>

{
public:
	/// standard constructor
	BlockMatching(const std::string& name = "", const std::string& pluginName = "");

	/// inputslot for the sequence of images where the flow will be computed
	InputSlot<cimg_library::CImgList<T> > sequence;

	/// inputslot for a list of Pixel out of the ROI to limit the region of 
	/// computation
	InputSlot<PixelSelection<T> *> pixelList;

	/// inputslot for the Params computed in the Incrementor
	InputSlot<Incrementor<T> *> newParams;

	/// inputslot to get the changes inside the image in t+1 from the 
	/// ObjectiveFunction
	InputSlot<ObjectiveFunction<T> *> changes;

	/// inputslot for the results from the SurfaceAnalysis
	InputSlot<SurfaceAnalysis<T> *> bestParam;

	/// outputslot to get the results from the flow estimation algorithm 
	/// BlockMatching
	OutputSlot<cimg_library::CImgList<T> > flow;

	/// standard execute from ParameteredObject
	virtual void execute() =0;
	
	/// the implentation of the BlockMatching algorithm
	virtual void findFlow () =0;
};

#endif /* BLOCKMATCHING_H_ */
