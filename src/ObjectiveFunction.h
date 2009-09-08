/// @file ObjectiveFunction.h
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 12.08.2009

#ifndef OBJECTIVEFUNKTION_H_
#define OBJECTIVEFUNKTION_H_

#include "ParameteredObject.h"
#include "BlockMatching.h"
#include "PixelSelection.h"
#include <vector>

template <typename T>
class ObjectiveFunction : ParameteredObject
{

public:
	/// standard constructor
	ObjectiveFunction (const std::string& name);
	InputSlot<cimg_library::CImg<T>& > image;
/// @TODO make 2 pixels as only inputslots first computed Pixel, second Pixel to compare in t+1
/// @TODO outputslot hemming distance
	/// compares the changed List of Pixels from the apply function
	/// (BrightnessModel) with the next picture and try to find the right
	/// position for every pixel.
	virtual std::vector<T> execute (const std::vector<Pixel<T> > & pixelList) =0;

};

#endif /* OBJECTIVEFUNKTION_H_ */
