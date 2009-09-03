/*
 * IncrementorConstant.h
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/** @file IncrementorConstant.h
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef INCREMENTORCONSTANT_H_
#define INCREMENTORCONSTANT_H_

#include "Incrementor.h"
#include "Constant.h"

template<typename T>
class IncrementorConstant: public Incrementor<T>
{
private:
	Parameter<T> x;
	Parameter<T> y;
	Parameter<T> z;
	Parameter<T> t;

public:
	/// standart constructor
	IncrementorConstant(const std::string& name);
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

};

#endif /* INCREMENTORCONSTANT_H_ */
