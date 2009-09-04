/*
 * IncrementorExponential.hxx
 *
 *  Created on: 27.08.2009
 *      Author: andreas
 */
/** @file IncrementorExponential.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef INCREMENTOREXPONENTIAL_HXX_
#define INCREMENTOREXPONENTIAL_HXX_

#include "Incrementor.hxx"
#include "IncrementorExponential.h"

template<typename T>
IncrementorExponential<T>::IncrementorExponential(const std::string& name) :
	Incrementor<T>::Incrementor(name)
{
	this->_addOutputSlot(actualPos, "actualPos", "actual Parameter Position",
			"IncrementorExponential");
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
bool IncrementorExponential<T>::doStep()
{
	/// preinitialization of the return value
	bool ret = false;
	typename std::set<AbstractSlot<IncrementorParameter<T>*>*>::const_iterator
			it;
	/// itterates the parameters with stepsize
	for (it = paramList.begin(); it != paramList.end(); it++)
	{
		if (this->paramList()->compareNames("x") && this->x
				>= this->paramList()->getMax())
		{
			this->x = this->x + this->paramList()->getStepsize();
		}

		if (this->paramList()->compareNames("y") && this->y
				>= this->paramList()->getMax())
		{
			this->y = this->y + this->paramList()->getStepsize();
		}

		if (this->paramList()->compareNames("z") && this->z
				>= this->paramList()->getMax())
		{
			this->z = this->z + this->paramList()->getStepsize();
		}
		if (this->paramList()->compareNames("t") && this->t
				>= this->paramList()->getMax())
		{
			this->t = this->t + this->paramList()->getStepsize();
		}
	}

	for (it = paramList.begin(); it != paramList.end(); it++)
	{
		///check if x < x-max and y < y-max and z < z-max
		if (this->x >= this->paramList()->getMax() || this->y
				>= this->paramList()->getMax() || this->z
				>= this->paramList()->getMax() || this->t
				>= this->paramList()->getMax())
		{
			ret = true;
		}
		else
		{
			return false;
		}
	}
	return ret;
}

template<typename T>
Parameter<T>& IncrementorExponential<T>::getX()
{
	return this->x;
}

template<typename T>
Parameter<T>& IncrementorExponential<T>::getY()
{
	return this->y;
}

template<typename T>
Parameter<T>& IncrementorExponential<T>::getZ()
{
	return this->z;
}

template<typename T>
Parameter<T>& IncrementorExponential<T>::getT()
{
	return this->t;
}

/*
 template<typename T>
 std::vector<std::string> & IncrementorExponential<T>::getModifier()
 {
 ///@TODO implement
 return this->modifier;
 }
 */

#endif /* INCREMENTOREXPONENTIAL_HXX_ */
