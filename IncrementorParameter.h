/*
 * IncrementorParameter.h
 *
 *  Created on: 31.08.2009
 *      Author: andreas
 */
/** @file IncrementorParameter.h
 * @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 * @date 31.08.2009
 */
#ifndef INCREMENTORPARAMETER_H_
#define INCREMENTORPARAMETER_H_

#include "ParameteredObject.h"
#include "CImg.h"
#include <vector>

template <typename T>
class IncrementorParameter : public ParameteredObject
{
private:
	std::string parameterName;
	Parameter<T> min;
	Parameter<T> max;
	Parameter<T> step;

public:
	/// stadard constructor
	IncrementorParameter (const std::string& name);
	OutputSlot<IncrementorParameter<T>* > paramList;
	/// returns true if inserted name and parameterName are equivalent
	bool compareNames(const std::string & nameToCompareWith) const;
	/// returns the Parameter min
	Parameter<T>& getMin ();
	/// returns the Parameter max
	Parameter<T>& getMax ();
	/// returns the Parameter step
	Parameter<T>& getStepsize();
};


#endif /* INCREMENTORPARAMETER_H_ */
