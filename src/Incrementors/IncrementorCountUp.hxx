/**
 *  @file IncrementorConstant.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef INCREMENTORCONSTANT_HXX_
#define INCREMENTORCONSTANT_HXX_

#include "Incrementor.hxx"
#include "IncrementorParameter.hxx"
#include "IncrementorCountUp.h"
#include "Parameter.h"
#include <set>
#include <iostream>

template<typename T>
IncrementorCountUp<T>::IncrementorCountUp(const std::string& name) :
	Incrementor<T>::Incrementor(name)
{
	this->modifier = 0;
	// sets the iterators to start
	this->itList = this->listOfParams.begin();
	this->itParams = this->paramList.begin();

	// set the params in listOfParams to minimum
	typename std::set<AbstractSlot<IncrementorParameter<T>*>*>::const_iterator
			it;
	typename std::vector<T>::iterator itLi = this->listOfParams.begin();
	for (it = this->paramList.begin(); it != this->paramList.end(); it++)
	{
		this->listOfParams.insert(itLi, this->paramList()->getMin());
		itLi++;
	}
}

template<typename T>
bool IncrementorCountUp<T>::doStep()
{
	// compute step
	if (*itList + this->paramList()->getStepSize()
			<= this->paramList()->getMax())
	{
		this->listOfParams.insert(itList, *itList
				+ this->paramList()->getStepSize());
	}
	else
	{
		this->itList++;
		this->modifier += 1;
		for (unsigned int i = 1; i <= modifier; i++)
		{
			this->listOfParams.insert(itList, this->paramList()->getMin());
			itParams++;
			if (*itList + this->paramList()->getStepSize()
					<= this->paramList()->getMax())
			{
				this->listOfParams.insert(itList, *itList
						+ this->paramList()->getStepSize());
				modifier -= 1;
			}
		}
	}
	if (modifier == this->listOfParams.size())
	{
		if (*itList + this->paramList()->getStepSize()
				<= this->paramList()->getMax())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	/*
	 typename std::set<AbstractSlot<IncrementorParameter<T>*>*>::const_iterator
	 it;
	 typename std::vector<T>::iterator itLi = this->listOfParams.begin();
	 /// preinitialization of the return value
	 std::vector<bool> ret;
	 for (it = this->paramList.begin(); it != this->paramList.end(); it++)
	 {
	 ret.push_back(false);
	 }
	 for (it = this->paramList.begin(); it != this->paramList.end(); it++)
	 {
	 if (*itLi + this->paramList()->getStepSize()
	 <= this->paramList()->getMax())
	 {
	 ret.push_back(false);
	 }
	 else
	 {
	 ret.push_back(true);
	 }
	 }
	 typename std::vector<bool>::iterator itRet;
	 for (itRet = ret.begin(); itRet != ret.end(); itRet++)
	 {
	 if (*itRet == false)
	 {
	 return false;
	 }
	 }
	 return false;
	 */
	/*	/// itterates the parameters with stepsize
	 for (it = paramList.begin(); it != paramList.end(); it++)
	 {

	 //list
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
	 */
}

/*
 template<typename T>
 std::vector<std::string> & IncrementorConstant<T>::getModifier()
 {
 std::stringstream strx;
 std::stringstream stry;
 std::stringstream strz;
 strx << x;
 stry << y;
 strz << z;
 this->modifier.push_back(strx.str());
 this->modifier.push_back(stry.str());
 this->modifier.push_back(strz.str());
 return this->modifier;
 }
 */

#endif /* INCREMENTORCOUNTUP_HXX_ */
