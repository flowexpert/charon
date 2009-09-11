/*  This file is part of Charon.

 Charon is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Charon is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with Charon.  If not, see <http://www.gnu.org/licenses/>.
 */
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
	//itList = this->listOfParams.begin();
	//this->itParams = this->paramList.begin();

	// set the params in listOfParams to minimum
	typename std::set<AbstractSlot<IncrementorParameter<T>*>*>::const_iterator
			it = this->paramList.begin();
	typename std::vector<Parameter<T>*>::iterator itLi =
			this->listOfParams.begin();
	unsigned int j = 0;
	for (; it != this->paramList.end(); it++)
	{
		this->listOfParams.push_back(&(this->paramList[j]->getMin()));
		j++;
	}
}

template<typename T>
bool IncrementorCountUp<T>::doStep()
{
	bool ret = false;
	//find the return value
	typename std::set<AbstractSlot<IncrementorParameter<T>*>*>::const_iterator
			it = this->paramList.begin();
	unsigned int j = 0;
	for (; it != this->paramList.end(); it++)
	{
		if (*(this->listOfParams[j]) + this->paramList[j]->getStepSize()
				>= this->paramList[j]->getMax())
		{
			ret = true;
		}
		else
		{
			ret = false;
			break;
		}
		j++;
	}

	// compute step
	if (!ret)
	{
		typename std::vector<Parameter<T>*>::iterator itLi =
				this->listOfParams.begin();
		unsigned int i = 0;
		for (; itLi != this->listOfParams.end(); itLi++)
		{
			if (*(this->listOfParams[i]) + this->paramList[i]->getStepSize()
					<= this->paramList[i]->getMax())
			{
				(*(this->listOfParams[i]))()
						+= this->paramList[i]->getStepSize()();
				break;
			}
			else
			{
				*(this->listOfParams[i]) = this->paramList[i]->getMin();
				(*(this->listOfParams[i + 1]))()
						+= this->paramList[i + 1]->getStepSize()();
				typename std::vector<Parameter<T>*>::iterator iterLi = itLi++;
				unsigned int k = i;
				for (; iterLi != this->listOfParams.end(); iterLi++)
				{
					if (*(this->listOfParams[k]) > this->paramList[k]->getMax())
					{
						*(this->listOfParams[k]) = this->paramList[k]->getMin();
						(*(this->listOfParams[k + 1]))() += this->paramList[k
								+ 1]->getStepSize()();
					}
					k++;
				}
				break;
			}
			i++;
		}
	}

	//return the return vallue
	return ret;
}

#endif /* INCREMENTORCOUNTUP_HXX_ */
