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
#include "SampleIterativePSolver.h"
#include <charon-utils/ImgTool.hxx>
#include <sstream>
class iterativeSolverException   {   }; 

template <typename T>
void IterativeSolver<T>::execute() {
    ParameteredObject::execute();
   
	try  {
       iterativeSolverExecute();
	}    catch(const iterativeSolverException &Exception) {
         msg << "\titerativeSolver error occured" << std::endl;
		 throw std::runtime_error(msg.str().c_str());
   } 
}



template <typename T>
int SampleIterativeSolver<T>::sampleIterativeSolverExecute() {
	
	/*
		imgListIn is the CImgList from file or from last iteration
		flowListIn is the CImgList (the flow) gained from the solver, e.g. from PetscSolver
	
	*/

	// just need read-only access to ImageList
	const Img<t>& globalImg = *(this->imgListIn());
	const Img<t>& globalFlow = *(this->flowListIn());

	/* 	
		is warping image 2 with the flow gained from the solver to image 1
		and puts the result into imgout
	*/


	int x,y,z;

	//throw exception if there are more or less than 2 images in the list
	if (this.imgListIn.size()!=2)
	{
		throw iterativeSolverException();
	}

	else
	{
		while (imgListIn[1]!=imgListIn[2]) 
		{

				// 1. create new image 2' which contains the interpolated values
				// 2. read flow F at position X 
				// 3. read intensity from position X+F in picture 2
				// 4. write the intensity at position x in picture 2'

				cimg_forXYZ(imgListOut[1], x,y,z) //equivalent to : for (int x = 0; x<img.width(); x++), for (int y = 0; y<img.height(); y++), for (int z = 0; z<img.depth(); z++)
				{
					imgListOut[1].img(x,y,z) = this->interpolator().interpolate(
													imgListIn[2], 
													(float) x+flowListIn[1].img(x,y,z,0),
													(float) y+flowListIn[1].img(x,y,z,1),
													(float) z)}

				}

		}
}
/*
cimg_forX(img,x) : equivalent to : for (int x = 0; x<img.width(); x++).
cimg_forY(img,y) : equivalent to : for (int y = 0; y<img.height(); y++).
cimg_forZ(img,z) : equivalent to : for (int z = 0; z<img.depth(); z++).

cimg_forXYZ(img,x,y,z) : equivalent to : cimg_forZ(img,z) cimg_forXY(img,x,y).
CImg<unsigned char> img(256,256,1,3);       // Define a 256x256 color image
  cimg_forXYC(img,x,y,v) { img(x,y,v) = (x+y)*(v+1)/6; }
  img.display("Color gradient");
*/

	/*
		am ende der schleife, wenn von petscsolver berechnete fluss gleich 0:
		zähle flüsse zusammen, um gesamtfluss zu erhalten
	*/

	// clean up
	for(usIt = unknownSizes.begin() ; usIt != unknownSizes.end() ; usIt++) {
		delete usIt->second;
	}

	return 0;
}

#endif // _SAMPLEITERATIVESOLVER_HXX_
