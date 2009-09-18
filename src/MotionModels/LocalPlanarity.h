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
/// @file LocalPlanarity.h
/// defines class MotionModels::LocalPlanarity
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 03.06.2009

#ifndef _MotionModels_LocalPlanarity_h_
#define _MotionModels_LocalPlanarity_h_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef motionmodels_localplanarity_EXPORTS
///Visual C++ specific code
#define motionmodels_localplanarity_DECLDIR __declspec(dllexport)
#else
#define motionmodels_localplanarity_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define motionmodels_localplanarity_DECLDIR
#endif

#include "../main.h"
#include "../MotionModel.h"
#include "../FlowFunctor.h"

namespace MotionModels
{

/** @brief motion model for local planarity
*  @details Computes the bcce-term for the parameters @f$ a_i @f$ ,@f$ b_{ij} @f$ and @f$ c_i @f$ for an optical flow of the form
*  @f[
*	\vec f \left( \begin{array}{c} x \\ y \\ z \end{array} \right) =
*		\left( \begin{array}{c} a_1 \\ a_2 \\ a_3 \end{array} \right) +
*		\left( \begin{array}{ccc} b_{11} &  b_{12} & b_{13} \\ 
*				   	  b_{21} &  b_{22} & b_{23} \\
*					  b_{31} &  b_{32} & b_{33} \end{array} \right)
*		\left( \begin{array}{c} x-x_c \\ y-y_c \\ z-z_c \end{array} \right) +
*		\left( \begin{array}{ccc} (x-x_c)^2 	 & (x-x_c)(y-y_c) & (x-x_c)(z-z_c) \\ 
*					  (x-x_c)(y-y_c) & (y-y_c)^2 	  & (y-y_c)(z-z_c) \\
*					  (x-x_c)(z-z_c) & (y-y_c)(z-z_c) & (z-z_c)^2 \end{array} \right)
*		\left( \begin{array}{c} c_1 \\ c_2 \\ c_3 \end{array} \right)
*  @f]
*/
template <class T>
class motionmodels_localplanarity_DECLDIR LocalPlanarity : public MotionModel
{
private:
	FlowFunctor flowfunc;
	bool is3d;

protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const
	{return new LocalPlanarity(name);}	

public:
	/// default constructor
	LocalPlanarity(const std::string& name = "");

	/// @name input slots for the derivatives
	//@{
	InputSlot<cimg_library::CImgList <T> > dx,dy,dz,dt;
	//@}

	virtual void update() {is3d = !(dx().is_sameN(1)); _update();}
	virtual void compute(	const int xs, const int ys, const int zs,
							const int t, const int v,
							std::map<std::string, T>& term,
							T& rhs);

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
		 *		\left( \begin{array}{c} x-x_c \\ y-y_c \\ z-z_c \end{array} \right) +
		 *		\left( \begin{array}{ccc} (x-x_c)^2 	 & (x-x_c)(y-y_c) & (x-x_c)(z-z_c) \\ 
		 *					  (x-x_c)(y-y_c) & (y-y_c)^2 	  & (y-y_c)(z-z_c) \\
		 *					  (x-x_c)(z-z_c) & (y-y_c)(z-z_c) & (z-z_c)^2 \end{array} \right)
		 *		\left( \begin{array}{c} c_1 \\ c_2 \\ c_3 \end{array} \right)
		 *  @f]
		 * 
	 */
	void setFlowFunctorParams(	const float a1, const float a2, const float a3,
								const float b11, const float b12, const float b13,
								const float b21, const float b22, const float b23,
								const float b31, const float b32, const float b33,
								const float c1, const float c2, const float c3);

	virtual void apply(const std::vector<std::string> &, 
	                   cimg_library::CImg<T>& image);

};

}

#endif

