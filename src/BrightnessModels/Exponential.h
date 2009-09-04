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
/// @file Exponential.h
/// defines class BrightnessModels::Exponential
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 15.06.2009

#ifndef _BrightnessModels_Exponential_h_
#define _BrightnessModels_Exponential_h_

#include "../main.h"
#include "../BrightnessModel.h"

namespace BrightnessModels
{

/** @brief BrightnessModel for exponential decay of brightness
* @details computes the bcce-term of the decay constant \a k defined by
*   @f[
*	g\left(\vec x\left( t \right) , t \right) = g\left(\vec x\left( t_0 \right) , t_0 \right) * exp \left( -k t \right)
*   @f]
*  where @f$ g\left(\vec x\left( t \right) , t \right) @f$ is the image and @f$ t_0 @f$ the starting time of the sequence.
*/
template <class T>
class Exponential : public BrightnessModel <T>
{
private:
	class Functor : public BrightnessFunctorInterface <T>
	{
	public:
		virtual void operator() (cimg_library::CImg<T>& sequence) const;
		virtual void get3d(cimg_library::CImgList<T>& sequence) const;
		float k;
	} functor;
	
protected:
//	virtual ParameteredObject* _newInstance(const std::string& name) const
//		{return new Exponential(name);}

public:
	/// default constructor
	Exponential(const std::string& name = "");

	InputSlot<cimg_library::CImgList<T> > sequence;

	virtual void compute(	const int xs, const int ys, const int zs,
							const int t, const int v,
							std::map<std::string, T>& term,
							T& rhs);
	
	virtual std::set<std::string>& getUnknowns(); // names of unknowns
	/**
	 *        set parameter for sequence generation
	 * @param k decay constant
	 */
	void setFunctorParams(float k);
	//virtual BrightnessFunctorInterface& getFunctor() {return functor;}
	
	virtual void apply(const std::vector<std::string> & modifier, 
	                   cimg_library::CImg<T>& image);
};

}

#endif
