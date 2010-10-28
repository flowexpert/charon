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
/// @file FlowFunctor.h
/// defines class FlowFunctor
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _flowFunctor_h_
#define _flowFunctor_h_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef flowfunctor_EXPORTS
///Visual C++ specific code
#define flowfunctor_DECLDIR __declspec(dllexport)
#else
#define flowfunctor_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define flowfunctor_DECLDIR
#endif

#include "main.h"
#include "FlowFunctorInterface.h"

/// a general class for the flowfunctors of the motion models
class flowfunctor_DECLDIR FlowFunctor : public FlowFunctorInterface {
private:
	// 2D parameters
	float a1, a2, b11, b12, b21, b22, c1, c2;
	// 3D parameters
	float a3, b13, b23, b31, b32, b33, c3;
	
	bool is3d;
public:
	/// default constructor
	FlowFunctor() {}
	// FlowFunctor(
	//		float a1, float a2,
	//		float b11, float b12, float b21, float b22,
	//		float c1, float c2
	// );
	/** set parameters for 2D
	*
	* the flow is computed by
	* \f[
		\vec f \left( \begin{array}{c} x \\ y \end{array} \right) =
			\left( \begin{array}{c} a_1 \\ a_2 \end{array} \right) +
			\left( \begin{array}{cc} b_{11} & b_{12} \\
				b_{21} & b_{22}  \end{array} \right)
			\left( \begin{array}{c} x \\ y \end{array} \right) +
			\left( \begin{array}{cc} x^2 & xy \\ xy & y^2  \end{array} \right)
			\left( \begin{array}{c} c_1 \\ c_2 \end{array} \right)
	  \f]
	*/
	void setParams(
			const float a1, const float a2,
			const float b11, const float b12,
			const float b21, const float b22,
			const float c1, const float c2);
	
	/** set parameters for 3D
	 *
	 * the flow is computed by
	 * \f[
		\vec f \left( \begin{array}{c} x \\ y \\ z \end{array} \right) =
			\left( \begin{array}{c} a_1 \\ a_2 \\ a_3 \end{array} \right) +
			\left( \begin{array}{ccc} b_{11} & b_{12} & b_{13} \\
				b_{21} & b_{22} & b_{23} \\
				b_{31} & b_{32} & b_{33} \end{array} \right)
			\left( \begin{array}{c} x \\ y \\ z \end{array} \right) +
			\left( \begin{array}{ccc} x^2 & xy & xz \\ xy & y^2 & yz \\
				xz & yz & z^2 \end{array} \right)
			\left( \begin{array}{c} c_1 \\ c_2 \\ c_3 \end{array} \right)
	  \f]
	 */
	void setParams3d(
			const float a1, const float a2, const float a3,
			const float b11, const float b12, const float b13,
			const float b21, const float b22, const float b23,
			const float b31, const float b32, const float b33,
			const float c1, const float c2, const float c3);
	/**
	 *        computes the 2D flow
	 * @param[in] x relative x-coordinate
	 * @param[in] y relative y-coordinate
	 * @param[out] f array in which flow is returned
	 */
	virtual void operator() (const float x, const float y,float* f) const;
	
	/**
	 *        computes the 3D flow
	 * @param[in] x relative x-coordinate
	 * @param[in] y relative y-coordinate
	 * @param[in] z relative z-coordinate	
	 * @param[out] f array in which flow is returned
	 */
	virtual void get3d(
			const float x, const float y, const float z, float* f) const;
	//virtual float* operator() ();
};

#endif
