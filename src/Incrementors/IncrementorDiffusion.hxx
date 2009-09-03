/*
 * IncrementorDiffusion.hxx
 *
 *  Created on: 24.08.2009
 *      Author: andreas
 */
/** @file IncrementorDiffusion.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef INCREMENTORDIFFUSION_HXX_
#define INCREMENTORDIFFUSION_HXX_

#include "Incrementor.hxx"
#include "IncrementorParameter.hxx"
#include "IncrementorDiffusion.h"
#include "Parameter.h"
#include <set>
#include <iostream>

template<typename T>
IncrementorDiffusion<T>::IncrementorDiffusion(const std::string& name) :
	Incrementor<T>::Incrementor(name), actualPos(this)
{
	this->_addOutputSlot(actualPos, "actualPos", "actual Parameter Position",
			"IncrementorDiffusion");
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
		if (this->paramList()->compareNames("D"))
		{
			this->D = this->paramList()->getMin();
		}
	}
}

template<typename T>
bool IncrementorDiffusion<T>::doStep()
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
		if (this->paramList()->compareNames("D") && this->D
				>= this->paramList()->getMax())
		{
			this->D = this->D + this->paramList()->getStepsize();
		}
	}

	for (it = paramList.begin(); it != paramList.end(); it++)
	{
		///check if x < x-max and y < y-max and z < z-max
		if (this->x >= this->paramList()->getMax() || this->y
				>= this->paramList()->getMax() || this->z
				>= this->paramList()->getMax() || this->t
				>= this->paramList()->getMax() || this->D
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
Parameter<T>& IncrementorDiffusion<T>::getX()
{
	return this->x;
}

template<typename T>
Parameter<T>& IncrementorDiffusion<T>::getY()
{
	return this->y;
}

template<typename T>
Parameter<T>& IncrementorDiffusion<T>::getZ()
{
	return this->z;
}

template<typename T>
Parameter<T>& IncrementorDiffusion<T>::getT()
{
	return this->t;
}

template<typename T>
Parameter<T>& IncrementorDiffusion<T>::getD()
{
	return this->D;
}

/*
 template<typename T>
 std::vector<std::string> & IncrementorDiffusion<T>::getModifier()
 {
 std::stringstream strx;
 std::stringstream stry;
 std::stringstream strz;
 std::stringstream strD;
 strx << x;
 stry << y;
 strz << z;
 strD << D;
 this->modifier.push_back(strx.str());
 this->modifier.push_back(stry.str());
 this->modifier.push_back(strz.str());
 this->modifier.push_back(strD.str());
 return this->modifier;
 }
 */

#endif /* INCREMENTORDIFFUSION_HXX_ */
