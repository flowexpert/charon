/*  This file is part of Charon.

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
/** @file SampleIterativeSolver.hxx
 *  Implementation of class SampleIterativeSolver.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 23.11.2009
 */

#ifndef _SAMPLEITERATIVESOLVER_HXX_
#define _SAMPLEITERATIVEVER_HXX_

#include "../IterativeSolver.hxx"
#include "SampleIterativeSolver.h"
#include <charon-utils/ImgTool.hxx>
#include <sstream>


using namespace std;
using namespace cimg_library;

//class iterativeSolverException   {   }; 

template <typename T>
SampleIterativeSolver<T>::SampleIterativeSolver(const std::string& name) : 
		IterativeSolver<T>("SampleIterativeSolver", name, "solves iteratively")
{

}

template <typename T>
void SampleIterativeSolver<T>::execute() {

	do {

		ParameteredObject::execute();

		int step = this->iteratorHelper()->getCurrentStep();
		int max = this->iteratorHelper()->getMaxIterations();

		sout << "Iterative Solver, iteration " << step << std::endl;;

		//first iteration step: assign globalFlowOut and set all entries to 0
		if(step == 1)
		{
			globalFlowOut.assign(this->flowListIn().size(),this->flowListIn()[0].width(),
				this->flowListIn()[0].height(),this->flowListIn()[0].depth(), this->flowListIn()[0].spectrum(), 0);
			this->imgListOut.assign(this->imgListIn().size(),this->imgListIn()[0].width(),
				this->imgListIn()[0].height(),this->imgListIn()[0].depth(), this->imgListIn()[0].spectrum(), 0);
		}


		///	flowListIn is the CImgList (the flow) gained from the solver, e.g. from PetscSolver

		float flowx, flowy;
		// just need read-only access to ImageList
		const cimg_library::CImgList<T>& globalImgListIn = this->imgListIn();
		const cimg_library::CImgList<T>& globalFlowListIn = this->flowListIn();

		///	is warping image 2 with the flow gained from the solver to image 1
		///	and puts the result into imgout

		/*
		 access to first time step: imgList[0](x,y,z,0)
		 access to second time step: imgList[0](x,y,z,1)
		 access to first flow component: flowList[0](x,y,z,t)
		 access to second flow component: flowList[1](x,y,z,t)
		*/

		// check preconditions:

		int spec = globalImgListIn[0].spectrum();
		//throw exception if there are more or less than 2 images in the list
		if (globalImgListIn[0].spectrum()!=2)
		{
			//welche art von Fehlerbehandlung erwuenscht?
			//throw iterativeSolverException();
		}

		//check if both images are not the same
		else {

			bool isSame=1;
			//	if (imgListOut[0].channel(0)!=imgListOut[0].channel(1) )
				{	
					isSame=0;
				}

			if(isSame==0)  { 
			
				cimg_library::CImg<T> img1;
				img1.assign(globalImgListIn[0].get_channel(1));

				//	ImgTool::warp2D(imgListOut[0].get_channel(1), globalFlowListIn, imgListOut[0].channel(1), this->interpolator());
				ImgTool::warp2D(img1, globalFlowListIn, img1, this->interpolator());

				this->imgListOut = globalImgListIn;
				this->imgListOut[0].channel(0); // jetzt ist gerade nur noch das erste Bild da
				this->imgListOut[0].append(img1,'c'); // hier wir das zweite Bild hinzugefügt
				
				int spectrum = this->imgListOut[0].spectrum();

				//sum calculated flows
				globalFlowOut[0]+=globalFlowListIn[0];
				globalFlowOut[1]+=globalFlowListIn[1];

				// if maximum iteration step reached, output flow

				if(step == max)
				{
					this->flowListOut() = globalFlowOut;

				}
				else //prepare next iteration step
				{
					//updates the iteratorHelper with imgListOut and globalFlowOut
					this->iteratorHelper()->update(this->imgListOut, globalFlowOut);
				}
			}
			else // flow hasn't changed
			{
				this->flowListOut() = globalFlowOut;

			}
		}
		//next iteration step
		this->iteratorHelper()->nextStep();

	}while(this->iteratorHelper()->getCurrentStep()<=this->iteratorHelper()->getMaxIterations());

}

#endif // _SAMPLEITERATIVESOLVER_HXX_
