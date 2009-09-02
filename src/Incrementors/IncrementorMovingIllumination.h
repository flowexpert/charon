/*
 * IncrementorMovingIllumination.h
 *
 *  Created on: 27.08.2009
 *      Author: andreas
 */

#ifndef INCREMENTORMOVINGILLUMINATION_H_
#define INCREMENTORMOVINGILLUMINATION_H_

#include "Incrementor.h"
#include "MovingIllumination.h"

template <typename T>
class IncrementorMovingIllumination : public Incrementor<T>
{
private:
//	std::vector<std::string> modifier;
	//BrightnessModels::MovingIllumination<T> movingillumination;
public:
	/// standart constructor
	IncrementorMovingIllumination (const std::string& name);
	InputSlot<cimg_library::CImg<T>& > image;
//	OutputSlot <Incrementor<T>*> newpos;
	virtual std::vector<std::string> & getModifier ();
	/** computes the next Step
	 *  the return is 0 if all max not reached and 1 if all max are reached
	 */
	/// @TODO maybe wrong
	bool doStep ();
};

#endif /* INCREMENTORMOVINGILLUMINATION_H_ */
