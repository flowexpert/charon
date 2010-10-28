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
/// @file SequenceGeneration.h
/// defines class SequenceGeneration
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _SequenceGeneration_SequenceGeneration_h
#define _SequenceGeneration_SequenceGeneration_h

#include "../main.h"
#include <cmath>
#include "../functions.h"
#include "../FlowFunctorInterface.h"
#include "../BrightnessModels/BrightnessFunctorInterface.h"
#include "../MotionModel.h"
#include "../BrightnessModel.h"

/** @brief class which generates test sequences
* @details this class generates sequences using the flow functor and the brightness functor
* given by the motion and brightness model
*/
class SequenceGeneration :  public ParameteredObject
{
private:
	//cimg_library::CImg<> img;
	//float* getFlow(int x, int y)
	/// calculates the value of a pixel regarding given flow
	float imageAt(float x, float y, int t, const FlowFunctorInterface& flowfunc);
	/// calculates the value of a pixel regarding given flow
	float imageAt3d(float x, float y, float z, int t, const FlowFunctorInterface& flowfunc);
	//void brightnessChange(cimg_library::CImg<>& sequence);
	//FlowFunctorInterface& flowfunc; 
	
	void generateSequence(int const& x, int const& y, int const& z, int const& t, int const& channels, int const& xc, int
			      const& yc, int const& zc, const FlowFunctorInterface& flowfunc,
	 		      const BrightnessFunctorInterface& brfunc, cimg_library::CImgList<>& ret );

protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const
		{return new SequenceGeneration(name);}		
	
public:
	/// default constructor
	SequenceGeneration(const std::string& name = "");
	//cimg_library::CImg<> generateImage(int x, int y);
	
	/// input slot for flow functor from motion model
	InputSlot<FlowFunctorInterface*> flowFunctor;
		
	/// input slot for brightness functor from brightness model
	InputSlot<BrightnessFunctorInterface*> brightFunctor;
	
	/// output slot for the generated sequence
	OutputSlot<cimg_library::CImgList<> > result;
	
	/**
	 *        sets the parameters of the genrated sequence
	 * @param w width
	 * @param h height
	 * @param d depth
	 * @param t duration
	 * @param v number of channels
	 * @param xc x-coordinate of center of motion
	 * @param yc y-coordinate of center of motion
	 * @param zc z-coordinate of center of motion
	 */
	void setParams(int w, int h, int d, int t, int v, int xc, int yc, int zc);
	
	
	/// @name parameters of the genrated sequence
	//@{
	
	/// width
	Parameter<int> width;
	
	/// height
	Parameter<int> height;
	
	/// depth
	Parameter<int> depth;
	
	/// duration
	Parameter<int> duration;
	
	/// number of channels
	Parameter<int> channels;
	
	/// x-coordinate of center of motion
	Parameter<int> xc;
	
	/// x-coordinate of center of motion
	Parameter<int> yc;
	
	/// x-coordinate of center of motion
	Parameter<int> zc;
	
	/// if set to true, a 3D sequence is generated
	Parameter<bool> is3d;
	
	//@}
	
	virtual void update ();
	
};


#endif
