/*
 * IncrementorDiffusion.h
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/// @file IncrementorDiffusion.h
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 24.08.2009

#ifndef INCREMENTORDIFFUSION_H_
#define INCREMENTORDIFFUSION_H_

#include "Incrementor.h"
#include "Diffusion.h"

template <typename T>
class IncrementorDiffusion: public Incrementor<T>
{
private:
//	std::vector<std::string> modifier;
	BrightnessModels::Diffusion<T> diffusion;
public:
	/// standart constructor
	IncrementorDiffusion (const std::string& name);
	InputSlot<cimg_library::CImg<T>& > image;
//	OutputSlot <Incrementor<T>*> newpos;

	virtual std::vector<std::string> & getModifier ();
	/** computes the next Step
	 *  the return is 0 if all max not reached and 1 if all max are reached
	 */
	bool doStep ();
};

#endif /* INCREMENTORDIFFUSION_H_ */
