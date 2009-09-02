/*
 * IncrementorDiffusion.hxx
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/// @file IncrementorDiffusion.hxx
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 24.08.2009

#ifndef INCREMENTORDIFFUSION_HXX_
#define INCREMENTORDIFFUSION_HXX_

#include "Incrementor.hxx"
#include "IncrementorDiffusion.h"

template<typename T>
IncrementorDiffusion<T>::IncrementorDiffusion(const std::string& name) :
	Incrementor<T>::Incrementor(name)
{

}

template<typename T>
bool IncrementorDiffusion<T>::doStep()
{
	//	paramList()->
	return false;
}

template<typename T>
std::vector<std::string> & IncrementorDiffusion<T>::getModifier()
{
	return this->modifier;
}

#endif /* INCREMENTORDIFFUSION_HXX_ */
