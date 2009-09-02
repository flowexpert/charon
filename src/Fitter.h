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
/// @file Fitter.h
/// defines abstract class Fitter
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _fitter_h_
#define _fitter_h_

#include "main.h"

/// abstract class for the fitters
/// @details fitters are used to compute the parameters of the models from the bcce terms
class Fitter : public ParameteredObject
{
protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const = 0;
	
public:
	/// default constructor
	Fitter(const std::string& classname,const std::string& name="") : 
		ParameteredObject(classname,name,"computes the parameters")
	{
		//_addInputSlot(tensor,"tensor","tensors from MotionEstimation","image");
		//_addOutputSlot(params, "params", "calculated parameters", "image");
		_addOutputSlot(out,"this", "pointer to itself", "Fitter*");
		
		out = this;
	}
	
	
	/**
	 *        compute the parameters from the bcce-terms
	 * @param img image containing bcce terms
	 * @param res image to write the parameters in
	 */
	virtual void  computeParams(const cimg_library::CImg<>& img, cimg_library::CImg<float>& res)=0;
	
// 	virtual void update() {computeParams(tensor(),params()); _outDataChanged(params);}
// 	
// 	/// input slot for the bcce terms
// 	InputSlot<cimg_library::CImg<float> > tensor;
// 	
// 	/// output slot for the parameters
// 	OutputSlot<cimg_library::CImg<float> > params;
	
	/// output slot with pointer to itself
	OutputSlot<Fitter*> out;
	
	
};

#endif
