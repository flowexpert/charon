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
/// @file LocalAffiness.h
/// defines class MotionModels::LocalAffiness
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 03.06.2009


#ifndef _MotionModels_LocalAffiness_h_
#define _MotionModels_LocalAffiness_h_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef motionmodels_localaffiness_EXPORTS
///Visual C++ specific code
#define motionmodels_localaffiness_DECLDIR __declspec(dllexport)
#else
#define motionmodels_localaffiness_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define motionmodels_localaffiness_DECLDIR
#endif

#include <main.h>
#include <MotionModel.h>
#include <FlowFunctor.h>

namespace MotionModels
{

/** @brief motion model for a local affine flow
 *  @details Computes the bcce-term for the parameters @f$ a_i @f$ and @f$ b_{ij} @f$ for an optical flow of the form
 *  @f[
 *	\vec f \left( \begin{array}{c} x \\ y \\ z \end{array} \right) =
 *		\left( \begin{array}{c} a_1 \\ a_2 \\ a_3 \end{array} \right) +
 *		\left( \begin{array}{ccc} b_{11} &  b_{12} & b_{13} \\
 *				   	  b_{21} &  b_{22} & b_{23} \\
*					  b_{31} &  b_{32} & b_{33} \end{array} \right)
 *		\left( \begin{array}{c} x-x_c \\ y-y_c \\ z-z_c \end{array} \right)
 *  @f]
 *
 */
template<class T>
class motionmodels_localaffiness_DECLDIR LocalAffiness: public MotionModel<T>
{
private:
	FlowFunctor flowfunc;
	bool is3d;

protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const
	{
		return new LocalAffiness(name);
	}

public:
	/// default constructor
	LocalAffiness(const std::string& name = "");

	/// @name input slots for the derivatives
	//@{
	InputSlot<cimg_library::CImgList<T> > dx, dy, dz, dt;
	//@}

	virtual void update()
	{
		is3d = !(dx().is_sameN(1));
	}
	virtual void compute(const int xs, const int ys, const int zs, const int t,
			const int v, std::map<std::string, T>& term, T& rhs);

	virtual std::set<std::string>& getUnknowns();

	/** @brief set the parameters of the flow functor
	 *
	 *  @details flow will be computed:
	 *  @f[
	 *	\vec f \left( \begin{array}{c} x \\ y \\ z \end{array} \right) =
	 *		\left( \begin{array}{c} a_1 \\ a_2 \\ a_3 \end{array} \right) +
	 *		\left( \begin{array}{ccc} b_{11} &  b_{12} & b_{13} \\ 
	 *				   	  b_{21} &  b_{22} & b_{23} \\
	 *					  b_{31} &  b_{32} & b_{33} \end{array} \right)
	 *		\left( \begin{array}{c} x-x_c \\ y-y_c \\ z-z_c \end{array} \right)
	 *  @f]
	 * 
	 */
	void setFlowFunctorParams(const float a1, const float a2, const float a3,
			const float b11, const float b12, const float b13, const float b21,
			const float b22, const float b23, const float b31, const float b32,
			const float b33);

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
