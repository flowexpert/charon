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
 *  @file Blockmatching.h
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

#include <ParameteredObject.hxx>
#include <Roi.h>
#include <CImg.h>
#include "Incrementor.h"
#include "ObjectiveFunction.h"
#include "SurfaceAnalysis.h"
#include "PixelSelection.h"
#include "BrightnessModel.h"
#include "MotionModel.h"
#include <Interpolator.h>
#include <vector>

template<typename T>
class blockmatching_DECLDIR BlockMatching: public TemplatedParameteredObject<T>

{
protected:
	cimg_library::CImgList<T> surface;

public:
	/// standard constructor
	BlockMatching(const std::string& name);
	InputSlot<cimg_library::CImgList<T>&> sequence;
	InputSlot<Roi<int>*> range;
	InputSlot<std::vector<Pixel<T>*>*> pixelList;
	InputSlot<Incrementor<T> *> newParams;
	InputSlot<ObjectiveFunction<T> *> changes;
	InputSlot<SurfaceAnalysis<T> *> bestParam;
	OutputSlot<BlockMatching<T> *> out;
	/// standard execute from Parametered Object
	void execute();
	/// trys to find the flow in the sequence
	virtual void /*cimg_library::CImgList<T>&*/findFlow() =0;
	cimg_library::CImgList<T>& getFlow();

};

#endif /* BLOCKMATCHING_H_ */
