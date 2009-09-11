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
	std::vector<Parameter<T>*> listOfParams;
public:
	/// standard constructor
	Incrementor(const std::string& name);
	InputSlot<cimg_library::CImg<T>&> image;
	InputSlot<IncrementorParameter<T>*> paramList;
	OutputSlot<Incrementor<T>*> out;
	void execute();
	/** computes the next Step
	 *  the return is 0 if all max not reached and 1 if all max are reached
	 */
	virtual bool doStep() =0;
	///returns a list of the computed Parameters
	std::vector<Parameter<T>*>& getListOfParams();

};

#endif /* INCREMENTOR_H_ */
