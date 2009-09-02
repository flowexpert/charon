/*
 * IncrementorExponential.h
 *
 *  Created on: 27.08.2009
 *      Author: andreas
 */
/// @file IncrementorExponential.h
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 24.08.2009

#ifndef INCREMENTOREXPONENTIAL_H_
#define INCREMENTOREXPONENTIAL_H_

#include "Incrementor.h"
#include "Exponential.h"

template <typename T>
class IncrementorExponential : public Incrementor<T>
{
private:
//	std::vector<std::string> modifier;
//	BrightnessModels::Exponential<T> exponential;
public:
	/// standart constructor
	IncrementorExponential (const std::string& name);
	InputSlot<cimg_library::CImg<T>& > image;
//	OutputSlot <Incrementor<T>*> newpos;

	virtual std::vector<std::string> & getModifier ();
	/** computes the next Step
	 *  the return is 0 if all max not reached and 1 if all max are reached
	 */
	/// @TODO maybe wrong
	bool doStep ();
};

#endif /* INCREMENTOREXPONENTIAL_H_ */
