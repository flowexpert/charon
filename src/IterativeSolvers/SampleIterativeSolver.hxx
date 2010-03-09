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

	/*
		imgListIn is the CImgList from file or from last iteration
		flowListIn is the CImgList (the flow) gained from the solver, e.g. from PetscSolver
	
	*/

	float flowx, flowy;
	// just need read-only access to ImageList
	const cimg_library::CImgList<T>& globalImgListIn = this->imgListIn();
	const cimg_library::CImgList<T>& globalFlowListIn = this->flowListIn();

	cimg_library::CImgList<T>& globalImgListOut = this->imgListOut();
	cimg_library::CImg<T>& globalFlowOut = this->flowOut();

	/* 	
		is warping image 2 with the flow gained from the solver to image 1
		and puts the result into imgout
	*/

	//throw exception if there are more or less than 2 images in the list
	if (globalImgListIn.size()!=2)
	{
		//welche art von Fehlerbehandlung erwuenscht?
		//throw iterativeSolverException();
	}


	/*
	// check preconditions
	assert(globalImgListIn().size() >= 2);
	assert(globalImgListIn[0].is_sameXYZC(globalImgListIn[1]));
	
	*/

	else
	{
		if(!globalImgListIn[0].is_sameXYZC(globalImgListIn[1]))
				// 1. create new image 2' which contains the interpolated values
				// 2. read flow F at position X 
				// 3. read intensity from position X+F in picture 2
				// 4. write the intensity at position x in picture 2'
				// 5. image 1' is the same as image 1
				cimg_forXYZC(globalImgListOut[1], x,y,z,t) 
				{
					flowx=float (T(x)+globalFlowListIn[0](x,y,z,0));
					flowy=float (T(y)+globalFlowListIn[0](x,y,z,1));
					globalImgListOut[1](x,y,z,t) = this->interpolator()->interpolate(
													globalImgListIn[1], 
													flowx,
													flowy,
													int(z), int(t));

					globalImgListOut[0](x,y,z,t)=globalImgListIn[0](x,y,z,t);

					this->imgListOut() = globalImgListOut;

					//add to global flow
					globalFlowOut(x,y,z,0)+=flowx;
					globalFlowOut(x,y,z,0)+=flowy;
				}
		else
			{
				this->flowOut() = globalFlowOut;
			}
	}



}

#endif // _SAMPLEITERATIVESOLVER_HXX_
