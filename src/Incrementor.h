/**
 *  @file Incrementor.h
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef INCREMENTOR_H_
#define INCREMENTOR_H_

#include "ParameteredObject.h"
#include "CImg.h"
#include "BlockMatching.h"
#include "ListedPixelSelection.h"
#include "IncrementorParameter.h"
#include <vector>

template<typename T>
class Incrementor: public TemplatedParameteredObject<T>
{
protected:
	std::vector<T> listOfParams;
public:
	/// standard constructor
	Incrementor(const std::string& name);
	InputSlot<cimg_library::CImg<T>&> image;
	InputSlot<IncrementorParameter<T>*> paramList;
	OutputSlot<Incrementor<T>*> actualPos;
	void execute();
	/** computes the next Step
	 *  the return is 0 if all max not reached and 1 if all max are reached
	 */
	virtual bool doStep() =0;
	///returns a list of the computed Parameters
	std::vector<T>& getListOfParams();

};

#endif /* INCREMENTOR_H_ */
