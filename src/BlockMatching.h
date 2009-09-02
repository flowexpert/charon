/*
 * Blockmatching.h
 *
 *  Created on: 12.08.2009
 *      Author: andreas
 */
/// @file Blockmatching.h
/// @date 12.08.2009


#ifndef BLOCKMATCHING_H_
#define BLOCKMATCHING_H_

#include "ParameteredObject.hxx"
#include "Roi.h"
#include "CImg.h"
#include "Incrementor.h"
#include "SurfaceAnalysis.h"
#include "PixelSelection.h"

template <typename T>
class BlockMatching : public ParameteredObject

{

public:
	BlockMatching (const std::string& name);
	InputSlot <cimg_library::CImg <T> > sequence;
	InputSlot <Roi <int>* > range;
	void execute (){};

};

#endif /* BLOCKMATCHING_H_ */
