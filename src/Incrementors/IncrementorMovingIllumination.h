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
	Parameter<T> x;
	Parameter<T> y;
	Parameter<T> z;
	Parameter<T> t;

public:
	/// standart constructor
	IncrementorMovingIllumination (const std::string& name);
	InputSlot<cimg_library::CImg<T>& > image;
	InputSlot<IncrementorParameter<T>*> paramList;
	OutputSlot<Incrementor<T>*> actualPos;

	/** computes the next Step
	 *  the return is 0 if all max not reached and 1 if all max are reached
	 */
	/// @TODO maybe wrong
	bool doStep ();
	/**
	 *  A mothode to return the x value.
	 *  @return actual x value
	 */
	Parameter<T>& getX();
	/**
	 *  A mothode to return the y value.
	 *  @return actual y value
	 */
	Parameter<T>& getY();
	/**
	 *  A mothode to return the z value.
	 *  @return actual z value
	 */
	Parameter<T>& getZ();
	/**
	 *  A mothode to return the t value.
	 *  @return actual t value
	 */
	Parameter<T>& getT();

};

#endif /* INCREMENTORMOVINGILLUMINATION_H_ */
