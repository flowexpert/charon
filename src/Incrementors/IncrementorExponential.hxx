/*
 * IncrementorExponential.hxx
 *
 *  Created on: 27.08.2009
 *      Author: andreas
 */
/// @file IncrementorExponential.hxx
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 24.08.2009

#ifndef INCREMENTOREXPONENTIAL_HXX_
#define INCREMENTOREXPONENTIAL_HXX_

#include "Incrementor.hxx"
#include "IncrementorExponential.h"

template<typename T>
IncrementorExponential<T>::IncrementorExponential(const std::string& name) :
	Incrementor<T>::Incrementor(name)
{

}

template<typename T>
bool IncrementorExponential<T>::doStep()
{
	return false;
}

template<typename T>
std::vector<std::string> & IncrementorExponential<T>::getModifier()
{
	return this->modifier;
}

#endif /* INCREMENTOREXPONENTIAL_HXX_ */
