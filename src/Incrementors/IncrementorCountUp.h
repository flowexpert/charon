/** @file IncrementorCountUp.h
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 24.08.2009
 */

#ifndef INCREMENTORCOUNTUP_H_
#define INCREMENTORCOUNTUP_H_

#include "Incrementor.h"

template<typename T>
class IncrementorCountUp: public Incrementor<T>
{
private:
	//typename std::set<AbstractSlot<IncrementorParameter<T>*>*>::const_iterator
	//		itParams;
	//typename std::vector<Parameter<T> >::iterator itList;
	unsigned int modifier;

public:
	/// standart constructor
	IncrementorCountUp(const std::string& name);
	/**
	 *  computes the next Step
	 *  the return is 0 if all max not reached and 1 if all max are reached
	 */
	virtual bool doStep();

};

#endif /* INCREMENTORCOUNTUP_H_ */
