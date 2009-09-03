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
Parameter<T>& IncrementorMovingIllumination<T>::getX()
{
	return this->x;
}

template<typename T>
Parameter<T>& IncrementorMovingIllumination<T>::getY()
{
	return this->y;
}

template<typename T>
Parameter<T>& IncrementorMovingIllumination<T>::getZ()
{
	return this->z;
}

template<typename T>
Parameter<T>& IncrementorMovingIllumination<T>::getT()
{
	return this->t;
}

/*
 template<typename T>
 std::vector<std::string> & IncrementorMovingIllumination<T>::getModifier()
 {
 ///@TODO implement
 return this->modifier;
 }
 */

#endif /* INCREMENTORMOVINGILLUMINATION_HXX_ */
