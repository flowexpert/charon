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
	ParameteredObject::execute();

	int step = this->iteratorHelper()->getCurrentStep();

	sout << "Iterative Solver, iteration " << step << std::endl;;

	//first iteration step: assign globalFlowOut and set all entries to 0
	if(step == 1)
	{
		globalFlowOut.assign(this->flowListIn().size(),this->flowListIn()[0].width(),
			this->flowListIn()[0].height(),this->flowListIn()[0].depth(), this->flowListIn()[0].spectrum(), 0);
		imgListOut.assign(this->imgListIn().size(),this->imgListIn()[0].width(),
			this->imgListIn()[0].height(),this->imgListIn()[0].depth(), this->imgListIn()[0].spectrum(), 0);
	}

	
	///	imgListIn is the CImgList from file or from last iteration
	///	flowListIn is the CImgList (the flow) gained from the solver, e.g. from PetscSolver
	
	float flowx, flowy;
	// just need read-only access to ImageList
	const cimg_library::CImgList<T>& globalImgListIn = this->imgListIn();
	const cimg_library::CImgList<T>& globalFlowListIn = this->flowListIn();
		
	///	is warping image 2 with the flow gained from the solver to image 1
	///	and puts the result into imgout

	/*
	 Zugriff auf ersten Zeitschritt: imgList[0](x,y,z,0)
	 Zugriff auf zweiten Zeitschritt: imgList[0](x,y,z,1)
	 Zugriff auf erste Flusskomponente: flowList[0](x,y,z,t)
	 Zugriff auf zweite Flusskomponente: flowList[1](x,y,z,t)
	*/

	//throw exception if there are more or less than 2 images in the list
	if (globalImgListIn[0].spectrum()!=2)
	{
		//welche art von Fehlerbehandlung erwuenscht?
		//throw iterativeSolverException();
	}

	// check preconditions
	//assert(globalImgListIn().size() >= 2);
	//assert(globalImgListIn[0].is_sameXYZC(globalImgListIn[1]));
	
	else {

		bool isSame=1;
/*
			cimg_forXYZ(globalImgListIn[0], x,y,z) {
				if (globalImgListIn[0](x,y,z,0)!=globalImgListIn[0](x,y,z,1))
				{
					isSame=0;
				}
			}

	*/

		isSame=0;

		if(isSame==0)  { //wenn 	imgList[0](x,y,z,0) !=imgList[0](x,y,z,1)
			// 1. create new image 2' which contains the interpolated values
			// 2. read flow F at position X
			// 3. read intensity from position X+F in picture 2
			// 4. write the intensity at position x in picture 2'
			
			cimg_forXYZC(this->imgListOut[0], x,y,z,t) {
				flowx=float (T(x)+globalFlowListIn[0](x,y,z,0));
				flowy=float (T(y)+globalFlowListIn[1](x,y,z,0));
				this->imgListOut[0](x,y,z,t) =
						this->interpolator()->interpolate(
								globalImgListIn[0],
								flowx, flowy,
								int(z), int(t));

				//add to global flow
				globalFlowOut[0](x,y,z,0)+=flowx;
				globalFlowOut[1](x,y,z,0)+=flowy;
			}


			// if maximum iteration step reached, output flow

			if(step == this->iteratorHelper()->getMaxIterations())
			{
				this->flowListOut() = globalFlowOut;
			}
			else //prepare next iteration step
			{
				//updates the iteratorHelper with imgListOut and globalFlowOut
				this->iteratorHelper()->update(this->imgListOut, globalFlowOut);
				//next iteration step
				this->iteratorHelper()->nextStep();
			}

			// just for testing!!!
			//this->flowListOut() = globalFlowOut;

		}
		else // flow hasn't changed
		{
			this->flowListOut() = globalFlowOut;

		}
	}

}

#endif // _SAMPLEITERATIVESOLVER_HXX_
