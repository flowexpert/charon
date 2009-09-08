/*
 * Blockmatching.h
 *
 *  Created on: 12.08.2009
 *      Author: andreas
 */
/** @file Blockmatching.h
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
#include <vector>

template<typename T>
class BlockMatching: public TemplatedParameteredObject<T>

{

public:
	/// standard constructor
	BlockMatching(const std::string& name);
	InputSlot<cimg_library::CImg<T> > sequence;
	InputSlot<Roi<int>*> range;
	InputSlot<std::vector<Pixel<T> >*> pixelList;
	InputSlot<Incrementor<T> *> newpos;
	InputSlot<std::vector<std::string>*> changes;
	/// standard execute from Parametered Object
	virtual void execute() =0;
	/// trys to find the flow in the sequence
	virtual void findFlow() =0;

};

#endif /* BLOCKMATCHING_H_ */
