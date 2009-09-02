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
/// @file MotionEstimation.h
/// defines class MotionEstimation
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _MotionEstimation_h_
#define _MotionEstimation_h_

#include "MotionModel.h"
#include "DataAssembler.h"
#include "Derivative.h"
#include "BrightnessModel.h"
#include "Fitter.h"
#include "main.h"

#include <Roi.hxx>

/// class which compute the bcce-terms
class MotionEstimation : public ParameteredObject
{

	protected:
		virtual ParameteredObject* _newInstance ( const std::string& name ) const
		{return new MotionEstimation ( name );}
	private:
		//cimg_library::CImg<> result;

		//cimg_library::CImg<> getTensor(int x, int y, int t);
	

		/**
		 *        computes the bcce-terms
		 * @param x x-coordinate of pixel
		  * @param y y-coordinate of pixel
		  * @param z z-coordinate of pixel
		 * @param t time
		 */
		void computeTerms ( const int x, const int y, const int z, const int t, cimg_library::CImg<>& res );
		//cimg_library::CImg<float> getParams();
		//void initialise();
		//float getScalar(int x, int y, int t, const cimg_library::CImg<>& params);
		//void getScalars(int x, int y, int t, const cimg_library::CImg<>& params, cimg_library::CImg<>& result);
	public:
		MotionEstimation ( const std::string& name = "" );
		virtual void update();

		/// input slot for pointer to the motion model
		InputSlot<MotionModel*> model;

		/// input slot for pointer to the brightness model
		InputSlot<BrightnessModel*> brightModel;

		/// input slot for pointer to the data assembler
		InputSlot<DataAssembler*> assembler;
		
		/// input slot for pointer to the fitter
		InputSlot<Fitter*> fitter;

		/// input slot for pointer to the Range of Interest
		InputSlot<Roi<int>* > roi;
		
		/// output slot for the results
		OutputSlot<cimg_library::CImgList<> > result;

		/// starting time at which the bcce-terms are computed
		Parameter<int> tbegin;
		
		/// ending time at which the bcce-terms are computed
		Parameter<int> tend;
		
		/// if true center of motion is relative to current pixel, otherwise its absolute
		Parameter<bool> centerrelative;
		
		/// x-coordinate of the center of motion
		Parameter<int> xc;

		/// y-coordinate of the center of motion
		Parameter<int> yc;

		/// z-coordinate of the center of motion
		Parameter<int> zc;
		


		/*MotionModel* model;
		DataAssembler* assembler;
		cimg_library::CImg<>* image;
		BrightnessModel* brightModel;
		Derivative* deriv;
		Fitter* fitter; //*/


};


#endif
