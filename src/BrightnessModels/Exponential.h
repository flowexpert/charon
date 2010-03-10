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

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef brightnessmodels_exponential_EXPORTS
///Visual C++ specific code
#define brightnessmodels_exponential_DECLDIR __declspec(dllexport)
#else
#define brightnessmodels_exponential_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define brightnessmodels_exponential_DECLDIR
#endif

#include <BrightnessModel.h>
#include <CImg.h>

namespace BrightnessModels
{

/** @brief BrightnessModel for exponential decay of brightness
 * @details computes the bcce-term of the decay constant \a k defined by
 *   @f[
 *	g\left(\vec x\left( t \right) , t \right) = g\left(\vec x\left( t_0 \right) , t_0 \right) * exp \left( -k t \right)
 *   @f]
 *  where @f$ g\left(\vec x\left( t \right) , t \right) @f$ is the image and @f$ t_0 @f$ the starting time of the sequence.
 */
template<class T>
class brightnessmodels_exponential_DECLDIR Exponential: public BrightnessModel<T>
{
private:
	class Functor: public BrightnessFunctorInterface<T>
	{
	public:
		virtual void operator()(cimg_library::CImg<T>& sequence) const;
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

	virtual void compute(const int xs, const int ys, const int zs, const int t,
			const int v, std::map<std::string, T>& term, T& rhs);

	virtual void computeEnergy(
				const int xs, const int ys, const int zs, const int t, const int v,
				const cimg_library::CImgList<T>& parameterList, double& energy);

	virtual std::set<std::string>& getUnknowns(); // names of unknowns
	/**
	 *        set parameter for sequence generation
	 * @param k decay constant
	 */
	void setFunctorParams(float k);
	//virtual BrightnessFunctorInterface& getFunctor() {return functor;}

	/** 
	 *  compute brightness changes with the inserted Parameters
	 *  @param inPixel insert pixel for which is the birghtness changes has to 
	 *  be done
	 *  @param modifier vector of Parameters to compute the modification
	 *  @param outPixel return value of Pixel type
	 */
	virtual void apply(const Pixel<T> & inPixel, const std::vector<
			IncrementorParameter<T>*> & modifier, Pixel<T> & outPixel);
};

}

#endif
