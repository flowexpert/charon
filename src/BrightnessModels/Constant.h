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
/// @file Constant.h
/// defines class BrightnessModels::Constant
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _BrightnessModels_Constant_h_
#define _BrightnessModels_Constant_h_

#include "../main.h"
#include "../BrightnessModel.h"
#include "BrightnessFunctorInterface.h"

/// namespace including the different brightness models
namespace BrightnessModels
{

/// a brightness model for no brightness change
template <class T>
class Constant : public BrightnessModel <T>
{
private:
	/// the brightness functor for no brightness change
	class Functor : public BrightnessFunctorInterface <T>
	{
	public:
		virtual void operator() (cimg_library::CImg<T>& sequence) const {}
		virtual void get3d (cimg_library::CImgList<T>& sequence) const {}
	} functor;

protected:
//	virtual ParameteredObject* _newInstance(const std::string& name) const
//		{return new Constant(name);}
	
public:
	/// default constructor
//	Constant(const std::string& name = "") : BrightnessModel("brightnessmodels_constant",name), functor()
//		 {brightnessFunctor = &functor;}
	Constant(const std::string& name = "");
	InputSlot<cimg_library::CImgList<T> > sequence;
	virtual void compute(	const int xs, const int ys, const int zs,
							const int t, const int v,
							std::map<std::string, T>& term,
							T& rhs);
	virtual std::set<std::string>& getUnknowns() const;
	//virtual BrightnessFunctorInterface& getFunctor() {return functor;}

	virtual void apply(const std::vector<std::string> & modifier,
	                   cimg_library::CImg<T>& image);
};

}

#endif
