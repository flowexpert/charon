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
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">
/// René Steinbrügge</a>
/// @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
/// @date 27.05.2009

#ifndef _BrightnessModels_Constant_H_
#define _BrightnessModels_Constant_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef brightnessmodels_constant_EXPORTS
///Visual C++ specific code
#define brightnessmodels_constant_DECLDIR __declspec(dllexport)
#else
#define brightnessmodels_constant_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define brightnessmodels_constant_DECLDIR
#endif

#include <CImg.h>
#include "../BrightnessModel.h"
#include <string>
#include "../functions.h"

/// namespace including the different brightness models
namespace BrightnessModels
{

	/// a brightness model for no brightness change
	template<class T>
	class brightnessmodels_constant_DECLDIR Constant: public BrightnessModel<T>
	{
	private:
		/// the brightness functor for no brightness change
		class Functor: public BrightnessFunctorInterface<T>
		{
		public:
			virtual void operator()(cimg_library::CImg<T>&) const
			{
			}
			virtual void get3d(cimg_library::CImgList<T>&) const
			{
			}
		} functor;

	public:
		/// default constructor
		Constant(const std::string& name = "");

		virtual void execute();

		virtual void compute(
				const int xs, const int ys, const int zs, const int t,
				const int v, std::map<std::string, T>& term, T& rhs,
				const std::string& unknown = "");

	virtual void computeEnergy(
		const int xs, const int ys, const int zs, const int t, const int v,
		const cimg_library::CImgList<T> flowList, double& energy);

		virtual std::set<std::string>& getUnknowns();
		//virtual BrightnessFunctorInterface& getFunctor() {return functor;}

		/**
		 *  compute brightness changes with the inserted Parameters
		 *  @param inPixel   insert pixel for which is the birghtness changes
		 *                   has to be done
		 *  @param modifier  vector of Parameters to compute the modification
		 *  @param outPixel  return value of Pixel type
		 */
		virtual void apply(
				const Pixel<T>& inPixel,
				const std::vector<IncrementorParameter<T>*>& modifier,
				Pixel<T>& outPixel);
	};

}

#endif
