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
/// @file Diffusion.h
/// defines class BrightnessModels::Diffusion
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 15.06.2009

#ifndef _BrightnessModels_Diffusion_h_
#define _BrightnessModels_Diffusion_h_

#include "../main.h"
#include "../BrightnessModel.h"
#include "../functions.h"
#include "BrightnessFunctorInterface.h"

namespace BrightnessModels
{

/** @brief BrightnessModel for an isotropic diffusion in brightness
*  @details Only tested in 2D. 3D image generation takes VERY long time! \n
*  Computes the bcce-term of the diffusion constant \a D  defined by
*   @f[
*	\frac{dg}{dt} = D
*	\left( \frac{\partial^2 g}{\partial x^2} + \frac{\partial^2 g}{\partial y^2} \right)
*   @f]
*  where @f$ g\left(\vec x\left( t \right) , t \right) @f$ is the image.
*/
template <class T>
class Diffusion : public BrightnessModel<T>
{
private:
	std::set<std::string> unknowns;
	class Functor : public BrightnessFunctorInterface <T>
	{
	public:
		virtual void operator() (cimg_library::CImg<T>& sequence) const;
		virtual void get3d(cimg_library::CImgList<T>& sequence) const;
		float d;
	} functor;

protected:
	//virtual ParameteredObject* _newInstance(const std::string& name) const
	//	{return new Diffusion(name);}

public:
	/// default constructor
	Diffusion(const std::string& name = "");
	
	InputSlot<cimg_library::CImgList<T> > dxx, dyy, dzz;  //FIXME basic image not needed as input -> modify BrightnessModel ?
	
	virtual void compute(	const int xs, const int ys, const int zs,
							const int t, const int v,
							std::map<std::string, T>& term,
							T& rhs);
	
	virtual std::set<std::string>& getUnknowns(); // names of unknowns
	
	/**
	 *        set the parameter for sequence generation
	 * @param d diffusion constant
	 */
	void setFunctorParams(float d);
	//virtual BrightnessFunctorInterface& getFunctor() {return functor;}

	virtual void apply(const std::vector<Parameter<T>* > & modifier);
};

}

#endif
