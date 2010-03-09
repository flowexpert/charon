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
/// @file MotionModel.h
/// defines abstract class MotionModel
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _MotionModel_h_
#define _MotionModel_h_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef motionmodel_EXPORTS
///Visual C++ specific code
#define motionmodel_DECLDIR __declspec(dllexport)
#else
#define motionmodel_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define motionmodel_DECLDIR
#endif

//#include "Derivative.h"
#include "FlowFunctorInterface.h"
#include "IncrementorParameter.h"
#include "Pixel.h"
#include <set>

/// abstract base class for the different motion models
/** If you need some input slots implementing your own MotionModel,
 *  you have to add them in your derived classs.
 */
template<class T>
class MotionModel: public TemplatedParameteredObject<T>
{
protected:
	/// unknowns handled by this motion model
	std::set<std::string> unknowns;

public:
	/// default constructor
	MotionModel(const std::string& classname, const std::string& name = "") :
		TemplatedParameteredObject<T>(classname, name, "computes the vectors")
	{
		this->_addOutputSlot(out, "this",
				"Pointer to itself", "MotionModel<T>");
		this->_addOutputSlot(flowFunctor, "flowfunctor",
				"flowFunctor of MotionModel", "FlowFunctorInterface*");
		this->_addParameter(x, "x",
				"x-coordinate of the center of the stencil", 0);
		this->_addParameter(y, "y",
				"y-coordinate of the center of the stencil", 0);
		this->_addParameter(z, "z",
				"z-coordinate of the center of the stencil", 0);
		out = this;
	}

	//InputSlot<cimg_library::CImg<> > img;
	//InputSlot<Derivative*> deriv;

	/// output slot containing pointer to this class
	OutputSlot<MotionModel<T>*> out;

	/// output slot containing pointer to the flow functor of the motion model
	OutputSlot<FlowFunctorInterface*> flowFunctor;

	/// \name coordinates of center of mask
	//\{
	Parameter<int> x, y, z;
	//\}

	/// compute the bcce-term of the motion model
	/**
	 *  \param[in]  xs,ys,zs  coordinates of the current pixel
	 *  \param[in]  t         current time
	 *  \param[in]  v         channel
	 *  \param[out] term      pre factor for each unknown
	 *  \param[out] rhs       T in which the bcce-term is written
	 *  \param[in]  unknown   name of the unknown to compute the term for
	 *                        (this is used for global methods)
	 */
	virtual void compute(
			const int xs, const int ys, const int zs, const int t,
			const int v, std::map<std::string, T>& term, T& rhs,
			const std::string& unknown = "") = 0;

	virtual void computeEnergy(
		const int xs, const int ys, const int zs, const int t, const int v,
		const cimg_library::CImgList<T> flowList, double& energy) = 0;

	// returns the width of the bcce-terms
	// @details = number of parameters of the model + 1
	//virtual int count()=0; // Anzahl der Parameter + 1
	//virtual FlowFunctorInterface& getFlowFunctor()=0;

	/// returns a vector of names of unknowns of the model
	/// by asking the vectors length, you get the number of unknowns
	virtual std::set<std::string>& getUnknowns() = 0;

	/** 
	 *  compute motion changes with the inserted Parameters
	 *  @param inPixel insert pixel for which is the birghtness changes has to 
	 *  be done
	 *  @param modifier vector of Parameters to compute the modification
	 *  @param outPixel return value of Pixel type
	 */
	virtual void apply(const Pixel<T> & inPixel, const std::vector<
			IncrementorParameter<T>*> & modifier, Pixel<T> & outPixel) = 0;

};

#endif
