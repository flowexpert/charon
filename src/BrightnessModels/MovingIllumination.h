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
/// @file MovingIllumination.h
/// defines class BrightnessModels::MovingIllumination
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 15.06.2009


#ifndef _BRIGHTNESSMODELS_MOVINGILLUMINATION_H_
#define _BRIGHTNESSMODELS_MOVINGILLUMINATION_H_

#include "../main.h"
#include "../BrightnessModel.h"
#include "../functions.h"
#include "BrightnessFunctorInterface.h"

namespace BrightnessModels
{

/// @brief BrightnessModel for a moving illumination envelope
/// @details Computes the bcce-term for \f$ a_1 \f$ and \f$ a_2 \f$ as described in paragraph 3.3 of "H. Haussecker, D. Fleet. Computing Optical Flow with Physical Models of Brightness Variation. Proc. IEEE CVPR, Hilton Head, 2000"
template <class T>
class MovingIllumination : public BrightnessModel<T>
{
private:
	std::set<std::string> unknowns;
	class Functor : public BrightnessFunctorInterface<T>
	{
	public:
		virtual void operator() (cimg_library::CImg<T>& sequence) const ;
		virtual void get3d(cimg_library::CImgList<T>& sequence) const
			{throw "BrightnessModels::MovingIllumination has no 3D support";}
		float a1, a2;
	} functor;
	
protected:
//	virtual ParameteredObject* _newInstance(const std::string& name) const
//		{return new MovingIllumination(name);}

public:
	/// default constructor
	MovingIllumination(const std::string& name ="");
	InputSlot<cimg_library::CImgList<T> > sequence;
	virtual void compute(const int xs, const int ys, const int zs, const int t, const int v,
			     std::map<std::string, T>& term,
			     T& rhs);
	
	virtual std::set<std::string>& getUnknowns(); // names of unknowns
	//virtual void initialize();
	///sets the parameters for sequence generation
	void setFunctorParams(float a1, float a2);
	//virtual BrightnessFunctorInterface& getFunctor() {return functor;}
	virtual std::vector<T> apply(const std::vector<Parameter<T>* > & modifier);

};

}

#endif
