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

#include "ParameteredObject.hxx"
#include "Roi.h"
#include "CImg.h"
#include "Incrementor.h"
#include "SurfaceAnalysis.h"
#include "PixelSelection.h"
#include "BrightnessModel.h"
#include "MotionModel.h"
#include "Interpolator.h"
#include <vector>

template<typename T>
class BlockMatching: public TemplatedParameteredObject<T>

{

public:
	/// standard constructor
	BlockMatching(const std::string& name);
	InputSlot<cimg_library::CImgList<T>& > sequence;
	InputSlot<Roi<int>*> range;
	InputSlot<std::vector<Pixel<T> >*> pixelList;
	InputSlot<Incrementor<T> *> newParams;
	InputSlot<std::vector<std::string>*> changes;
	/// standard execute from Parametered Object
	void execute();
	/// trys to find the flow in the sequence
	virtual void findFlow() =0;

};

#endif /* BLOCKMATCHING_H_ */
