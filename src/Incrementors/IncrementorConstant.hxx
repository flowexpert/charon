/*
 * IncrementorConstant.hxx
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/// @file IncrementorConstant.hxx
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 24.08.2009

#ifndef INCREMENTORCONSTANT_HXX_
#define INCREMENTORCONSTANT_HXX_

#include "Incrementor.hxx"
#include "IncrementorParameter.hxx"
#include "IncrementorConstant.h"
#include "Parameter.h"
#include <set>
#include <iostream>

template<typename T>
IncrementorConstant<T>::IncrementorConstant(const std::string& name,
		template_type t) :
	Incrementor<T>::Incrementor(name, t), actualPos(this)
{
	this->_addOutputSlot(actualPos, "actualPos", "actual Parameter Position",
			"IncrementorConstant");
	typename std::set<AbstractSlot<IncrementorParameter<T>*>*>::const_iterator
			it;
	for (it = paramList.begin(); it != paramList.end(); it++)
	{
		if (this->paramList()->compareNames("x"))
		{
			this->x = this->paramList()->getMin();
		}
		if (this->paramList()->compareNames("y"))
		{
			this->y = this->paramList()->getMin();
		}
		if (this->paramList()->compareNames("z"))
		{
			this->z = this->paramList()->getMin();
		}
	}
}

template<typename T>
bool IncrementorConstant<T>::doStep()
{
	/// preinitialization of the return value
	bool ret = false;
	typename std::set<AbstractSlot<IncrementorParameter<T>*>*>::const_iterator
			it;
	/// itterates the parameters with stepsize
	for (it = paramList.begin(); it != paramList.end(); it++)
	{
		if (this->paramList()->compareNames("x"))
		{
			this->x = this->x + this->paramList()->getStepsize();
		}

		if (this->paramList()->compareNames("y"))
		{
			this->y = this->y + this->paramList()->getStepsize();
		}

		if (this->paramList()->compareNames("z"))
		{
			this->z = this->z + this->paramList()->getStepsize();
		}
	}

	for (it = paramList.begin(); it != paramList.end(); it++)
	{
		///check if x < x-max and y < y-max and z < z-max
		if (this->x >= this->paramList()->getMax() && this->y
				>= this->paramList()->getMax() && this->z
				>= this->paramList()->getMax())
		{
			ret = true;
		}
		else
		{
			ret = false;
		}
	}
	return ret;
}

template<typename T>
std::vector<std::string> & IncrementorConstant<T>::getModifier()
{
	std::stringstream str;
	str << x;
	this->modifier.push_back(str.str());
	return this->modifier;
}

#endif /* INCREMENTORCONSTANT_HXX_ */
