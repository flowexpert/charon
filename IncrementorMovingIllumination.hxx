/*
 * IncrementorMovingIllumination.hxx
 *
 *  Created on: 27.08.2009
 *      Author: andreas
 */
/// @file IncrementorMovingIllumination.hxx
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 27.08.2009


#ifndef INCREMENTORMOVINGILLUMINATION_HXX_
#define INCREMENTORMOVINGILLUMINATION_HXX_

#include "Incrementor.hxx"
#include "IncrementorMovingIllumination.h"

template<typename T>
IncrementorMovingIllumination<T>::IncrementorMovingIllumination(
		const std::string& name) :
	Incrementor<T>::Incrementor(name)
{

}

template<typename T>
bool IncrementorMovingIllumination<T>::doStep()
{
	return false;
}

template<typename T>
std::vector<std::string> & IncrementorMovingIllumination<T>::getModifier()
{
	return this->modifier;
}

#endif /* INCREMENTORMOVINGILLUMINATION_HXX_ */
