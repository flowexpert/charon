/*
 * IncrementorParameter.hxx
 *
 *  Created on: 31.08.2009
 *      Author: andreas
 */
/** @file IncrementorParameter.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 31.08.2009
 */

#ifndef INCREMENTORPARAMETER_HXX_
#define INCREMENTORPARAMETER_HXX_

#include "IncrementorParameter.h"

template <typename T>
IncrementorParameter<T>::IncrementorParameter(const std::string& name) :
	ParameteredObject ("incrementorparameter", name,
	"class for Incrementor Parameters", t), paramList(this)
{
	_addOutputSlot( paramList, "paramList", "list of Parameters for Incrementor",
	               "Incrementor Parameter", "IncrementorParameter");
	_addParameter( min , "min" , "minimum of parameter"           , "T");
	_addParameter( max , "max" , "maximum of parameter"           , "T");
	_addParameter( step, "step", "stepsize to increment parameter", "T");
}

template<typename T>
bool IncrementorParameter<T>::compareNames(const std::string& nameToCompareWith) const
{
	if (this->parameterName == nameToCompareWith)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
Parameter<T>& IncrementorParameter<T>::getMin()
{
	return this->min;
}

template<typename T>
Parameter<T>& IncrementorParameter<T>::getMax()
{
	return this->max;
}

template<typename T>
Parameter<T>& IncrementorParameter<T>::getStepsize()
{
	return this->step;
}

#endif /* INCREMENTORPARAMETER_HXX_ */
