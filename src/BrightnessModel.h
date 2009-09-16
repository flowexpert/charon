/*  Copyright (C) 2009 René Steinbrügge

 This file is part of Charon.

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
/// @file BrightnessModel.h
/// defines abstract class BrightnessModel
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009


#ifndef _BrightnessModel_h_
#define _BrightnessModel_h_

#include "main.h"
#include <set>
#include "BrightnessModels/BrightnessFunctorInterface.h"
#include "Pixel.h"
#include "IncrementorParameter.h"

/// abstract base class for the different BrightnessModels
template<class T>
class BrightnessModel: public ParameteredObject
{
protected:
	//virtual ParameteredObject* _newInstance(const std::string& name) const = 0;

	/// does initial calculation
	virtual void initialize()
	{
	}

	/// Set of unknowns
	std::set<std::string> unknowns;

public:
	/// default constructor
	BrightnessModel(const std::string& classname, const std::string& name = "") :
		ParameteredObject(classname, name,
				"computes the vectors for brightness-modeling")
	{
		_addOutputSlot(out, "this", "Pointer to itself", "BrightnessModel*");
		_addOutputSlot(brightnessFunctor, "brightnessfunctor",
				"Pointer to BrightnessFunctor of the Model",
				"BrightnessFunctorInterface*");
		_addInputSlot(img, "image", "Image to work with", "CImgList");
		out = this;
	}

	/// OutputSlot containing pointer to this BrightnessModel
	OutputSlot<BrightnessModel*> out;

	/// OutputSlot containing pointer to the brightness functor of this BrightnessModel
	OutputSlot<BrightnessFunctorInterface<T>*> brightnessFunctor;

	/// imagelist to work on
	InputSlot<cimg_library::CImgList<T> > img;

	/// updates the motionmodel
	virtual void update() //FIXME virtual ??
	{
		initialize();
		//_outDataChanged(out); // TODO datenpfad
	}

	/** 
	 * 	computes the bcce term
	 * @param xs x coordinate
	 * @param ys y coordinate
	 * @param zs z coordinate
	 * @param t time coordinate
	 * @param v channel
	 * @param img image in which bcce term is written
	 */
	virtual void compute(const int xs, const int ys, const int zs, const int t,
			const int v, std::map<std::string, T>& term, T& rhs)=0;

	/// returns a vector of names of unknowns of the model
	/// by asking the vectors lenght, you get the number of unknowns
	virtual std::set<std::string>& getUnknowns() =0;

	/** 
	 *  compute brightness changes 
	 *  @param inPixel insert pixel for which is the birghtness changes has to 
	 *  be done
	 *  @param modifier vector of Parameters to compute the modification
	 *  @param outPixel return value of Pixel type
	 */
	virtual void apply(const Pixel<T> & inPixel, const std::vector<
			IncrementorParameter<T>*> & modifier, Pixel<T> & outPixel) =0;
};

#endif
