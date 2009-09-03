/*
 * IncrementorDiffusion.h
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/** @file IncrementorDiffusion.h
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef INCREMENTORDIFFUSION_H_
#define INCREMENTORDIFFUSION_H_

#include "Incrementor.h"
#include "Diffusion.h"

template<typename T>
class IncrementorDiffusion: public Incrementor<T>
{
private:
	Parameter<T> x;
	Parameter<T> y;
	Parameter<T> z;
	Parameter<T> t;
	Parameter<T> D;

public:
	/// standart constructor
	IncrementorDiffusion(const std::string& name);
	InputSlot<cimg_library::CImg<T>&> image;
	InputSlot<IncrementorParameter<T>*> paramList;
	OutputSlot<Incrementor<T>*> actualPos;

	/** computes the next Step
	 *  the return is 0 if all max not reached and 1 if all max are reached
	 */
	bool doStep();
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
	/**
	 *  A mothode to return the D value.
	 *  @return actual D value
	 */
	Parameter<T>& getD();

};

#endif /* INCREMENTORDIFFUSION_H_ */
