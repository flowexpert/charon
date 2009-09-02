/*
 * IncrementorConstant.h
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/// @file IncrementorConstant.h
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 24.08.2009

#ifndef INCREMENTORCONSTANT_H_
#define INCREMENTORCONSTANT_H_

#include "Incrementor.h"
#include "Constant.h"

template <typename T>
class IncrementorConstant: public Incrementor<T>
{
private:
//	std::vector<std::string> modifier;
	//BrightnessModels::Constant<T> constant;
	Parameter<T> x;
	Parameter<T> y;
	Parameter<T> z;
	Parameter<T> t;

public:
	/// standart constructor
	IncrementorConstant (const std::string& name);
	InputSlot<cimg_library::CImg<T>& > image;
	InputSlot<IncrementorParameter<T>* > paramList;
	OutputSlot <Incrementor<T>*> actualPos;

	virtual std::vector<std::string> & getModifier ();
	/// computes the next Step
	bool doStep ();
};


#endif /* INCREMENTORCONSTANT_H_ */
