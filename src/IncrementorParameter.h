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
 *  @file IncrementorParameter.h
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 31.08.2009
 */
#ifndef INCREMENTORPARAMETER_H_
#define INCREMENTORPARAMETER_H_

#include <ParameteredObject.h>
#include <CImg.h>
#include <vector>

template<typename T>
class IncrementorParameter: public TemplatedParameteredObject<T>
{
private:
	std::string parameterName;
	Parameter<T> min;
	Parameter<T> max;
	Parameter<T> step;
	Parameter<T> current;

public:
	/// stadard constructor
	IncrementorParameter(const std::string& name);
	OutputSlot<IncrementorParameter<T>*> paramList;
	/// returns true if inserted name and parameterName are equivalent
	std::string& getName();
	/// sets name of InrementorParameter
	void setName(std::string& newName);
	/// returns the Parameter min
	Parameter<T>& getMin();
	/// sets minimum Parameter
	void setMin(const T newMin);
	/// returns the Parameter max
	Parameter<T>& getMax();
	/// sets maximum Parameter
	void setMax(const T newMax);
	/// returns the Parameter step
	Parameter<T>& getStepSize();
	/// sets StepSize Parameter
	void setStepSize(const T newStep);
	/// returns the Parameter current
	Parameter<T>& getCurrent();
	/// sets current Parameter
	void setCurrent(const T newCurrent);
};

#endif /* INCREMENTORPARAMETER_H_ */
