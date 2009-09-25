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
 *  @brief implementation of class IncrementorCountUp, child class of Incrementor
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef INCREMENTORCONSTANT_HXX_
#define INCREMENTORCONSTANT_HXX_

#include <Incrementor.hxx>
#include "IncrementorCountUp.h"
#include <set>

template<typename T>
IncrementorCountUp<T>::IncrementorCountUp(const std::string& name) :
	Incrementor<T>::Incrementor(name, "IncrementorCountUp")
{

}

template<typename T>
void IncrementorCountUp<T>::execute()
{
	typename std::set<AbstractSlot<IncrementorParameter<T>*>*>::const_iterator
			it = this->paramList.begin();
	typename std::vector<IncrementorParameter<T>*>::iterator itLi =
			this->listOfParams.begin();
	unsigned int j = 0;
	for (; it != this->paramList.end(); it++)
	{
		this->listOfParams.push_back(this->paramList[j]);
		this->listOfParams[j]->current =this->paramList[j]->min;
		this->listOfParams[j]->name =this->paramList[j]->name;
		j++;
	}
}

template<typename T>
bool IncrementorCountUp<T>::doStep()
{
	bool ret = false;
	//	typename std::set<AbstractSlot<IncrementorParameter<T>*>*>::const_iterator
	//			it = this->paramList.begin();
	//	unsigned int j = 0;
	//	check for return vallue
	//	for (; it != this->paramList.end(); it++)
	for (unsigned int j = 0; j != this->listOfParams.size(); j++)
	{
		if (this->listOfParams[j]->current
				+ this->paramList[j]->stepSize
				> this->paramList[j]->max)
		{
			ret = true;
		}
		else
		{
			ret = false;
			break;
		}
		//		j++;
	}

	// compute step
	if (!ret)
	{
		typename std::vector<IncrementorParameter<T>*>::iterator itLi =
				this->listOfParams.begin();
		unsigned int i = 0;
		for (; itLi != this->listOfParams.end(); itLi++)
		{
			if (this->listOfParams[i]->current
					+ this->paramList[i]->stepSize
					<= this->paramList[i]->max)
			{
				this->listOfParams[i]->current =
						this->listOfParams[i]->current
								+ this->paramList[i]->stepSize;
				break;
			}
			else
			{
				this->listOfParams[i]->current =this->paramList[i]->min;
				this->listOfParams[i + 1]->current =
						this->listOfParams[i + 1]->current
								+ this->paramList[i + 1]->stepSize;
				typename std::vector<IncrementorParameter<T>*>::iterator
						iterLi = itLi++;
				unsigned int k = i;
				for (; iterLi != this->listOfParams.end(); iterLi++)
				{
					if (this->listOfParams[k]->current
							> this->paramList[k]->max)
					{
						this->listOfParams[k]->current =
								this->paramList[k]->min;
						this->listOfParams[k + 1]->current =
								this->listOfParams[k + 1]->current
										+ this->paramList[k + 1]->stepSize;
					}
					k++;
				}
				break;
			}
			i++;
		}
	}
	//return return vallue
	return ret;
}

#endif /* INCREMENTORCOUNTUP_HXX_ */
