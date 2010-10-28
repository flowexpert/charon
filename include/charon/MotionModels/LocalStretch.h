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
/// @file LocalStretch.h
/// defines class MotionModels::LocalStretch
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 03.06.2009

#ifndef _MotionModels_LocalStretch_h_
#define _MotionModels_LocalStretch_h_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef motionmodels_localstretch_EXPORTS
///Visual C++ specific code
#define motionmodels_localstretch_DECLDIR __declspec(dllexport)
#else
#define motionmodels_localstretch_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define motionmodels_localstretch_DECLDIR
#endif

#include <main.h>
#include <MotionModel.h>
#include <FlowFunctor.h>

namespace MotionModels
{


/** @brief motion model with a local stretch regarding to a center of motion
* @details only 2D implemented yet! \n
* Computes the bcce-term for the parameters @f$ a_i @f$ and @f$ s_i @f$ for an optical flow of the form
*  @f[
*	\vec f \left( \begin{array}{c} x \\ y \end{array} \right) =
*		\left( \begin{array}{c} a_1 \\ a_2  \end{array} \right) +
*		\left( \begin{array}{ccc} s_1 &  s_2  \\ 
*				   	  s_2 &  -s_1  \end{array} \right)
*		\left( \begin{array}{c} x-x_c \\ y-y_c  \end{array} \right)
*  @f]
*/
template <class T>
class motionmodels_localstretch_DECLDIR LocalStretch : public MotionModel <T>
{
private:
	FlowFunctor flowfunc;
	bool is3d;

public:
	/// default constructor
	LocalStretch(const std::string& name = "");

	/// @name input slots for the derivatives
	//@{
	InputSlot<cimg_library::CImgList <T> > dx,dy,dz,dt;
	//@}

	virtual void execute();
	virtual void compute(	const int xs, const int ys, const int zs,
							const int t, const int v,
							std::map<std::string, T>& term,
							T& rhs);

	virtual void computeEnergy(
				const int xs, const int ys, const int zs, const int t, const int v,
				const cimg_library::CImgList<T>& parameterList, double& energy);

	virtual std::set<std::string>& getUnknowns() ;
	
	/** @brief set the parameters of the flow functor
	 *
	 *  @details flow will be computed:
	 *  @f[
	 *	\vec f \left( \begin{array}{c} x \\ y \end{array} \right) =
	 *		\left( \begin{array}{c} a_1 \\ a_2  \end{array} \right) +
	 *		\left( \begin{array}{ccc} s_1 &  s_2  \\ 
	 *				   	  s_2 &  -s_1  \end{array} \right)
	 *		\left( \begin{array}{c} x-x_c \\ y-y_c  \end{array} \right)
	 *  @f]
	 * 
	 */
	void setFlowFunctorParams(float a1, float a2, float s1, float s2);

	/** 
	 *  compute motion changes with the inserted Parameters
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
