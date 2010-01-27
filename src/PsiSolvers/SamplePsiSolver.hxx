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
/** @file SamplePsiSolver.hxx
 *  Implementation of class SamplePsiSolver.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 23.11.2009
 */

#ifndef _SAMPLEPSISOLVER_HXX_
#define _SAMPLEPSIVER_HXX_

#include "../PsiSolver.hxx"
#include "SamplePsiPSolver.h"
#include <charon-utils/ImgTool.hxx>
#include <sstream>


template <typename T>
void PsiSolver<T>::execute() {
    ParameteredObject::execute();
    int errorCode;
    errorCode = psiSolverExecute();
    if (errorCode) {
        std::ostringstream msg;
        msg << __FILE__ << ":" << __LINE__ << std::endl;
        msg << "\tpsiSolver error occured" << std::endl;
        msg << "\tError code:\n\t\t" << errorCode;
        throw std::runtime_error(msg.str().c_str());
    }
}



template <typename T>
int SamplePsiSolver<T>::samplePsiSolverExecute() {
	
	/*
		imgin is the CImgList from file or from last iteration
		fluxin is the CImgList (the flux) gained from the solver, e.g. from PetscSolver
	
	*/

	// just need read-only access to ImageList
	const Img<int>& globalImg = *(this->imgin());
	const Img<int>& globalFlux = *(this->fluxin());

	/* 		
		wendet den vom PetscSolver ( aus Bild 1 und 2) erzeugten Fluss
		auf die beiden Input-Bildern 1 und 2 an und legt Ergebnis in imgout ab

	*/


	int x,y,z;

	while (globalFlux != 0) // solange von petscsolver berechnete fluss ungleich 0
    {

		

/*
	solange von petscsolver berechnete fluss ungleich 0
		für i <= anzahlBilderInImgin
			für jedes pixel j des imgout
				//Wert des Input-Pixels wird an die um den Fluss verschobene Position geschrieben
				imgout[i].pixel[j] = interpolate(imgin[i].pixel[j+Fluss[i].pixel[j]])			
*/


		/*
		FRAGE: Sollen im ImgOut nur die neuen Bilder 1', 2', ... stehen, oder alle Bilder 1, 1', 2', 2, ...?
		*/

		for (i=0, i<=this.imgListIn.size(), i++) // für i <= anzahlBilderInImgin
		{


			// 1. Neues Bild 1' erzeugen, das die interpolierten Werte enthalten soll
			// 2. Fluss F an der Position X aus dem Flussbild nehmen
			// 3. Intensität aus dem ersten Bild an der Stelle X+(F/2) auslesen
			// 4. Intensität an die Stelle X im Bild 1' schreiben

			cimg_forXYZ(imgListOut[i+(1-1)], x,y,z) //equivalent to : for (int x = 0; x<img.width(); x++), for (int y = 0; y<img.height(); y++), for (int z = 0; z<img.depth(); z++)
			{
				imgListOut[i+(i-1)].img(x,y,z) = this->interpolator().interpolate(
												imgListIn[i+1], 
												(float) x+(fluxListIn[i].getX())/2,
												(float) y+(fluxListIn[i].getY())/2,
												(float) z+(fluxListIn[i].getZ())/2)

			}

			// 1. Neues Bild 2' erzeugen, das die interpolierten Werte enthalten soll
			// 2. Fluss F an der Position X aus dem Flussbild nehmen
			// 3. Intensität aus dem zweiten Bild an der Stelle X+F auslesen
			// 4. Intensität an die Stelle X im Bild 2' schreiben

			cimg_forXYZ(imgListOut[2*i], x,y,z) //equivalent to : for (int x = 0; x<img.width(); x++), for (int y = 0; y<img.height(); y++), for (int z = 0; z<img.depth(); z++)
			{
				imgListOut[2*i].img(x,y,z) = this->interpolator().interpolate(
												imgListIn[i], 
												(float) x+fluxListIn[i+1].getX(),
												(float) y+fluxListIn[i+1].getY(),
												(float) z+fluxListIn[i+1].getZ())

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

	/*ierr = VecDestroy(x);CHKERRQ(ierr);
	ierr = VecDestroy(b);CHKERRQ(ierr);
	ierr = MatDestroy(A);CHKERRQ(ierr);
	ierr = KSPDestroy(ksp);CHKERRQ(ierr);*/


	return 0;
}

template <typename T>
void SamplePsiSolver<T>::execute() {
	ParameteredObject::execute();
	int errorCode;
	errorCode = samplePsiSolverExecute();
	if (errorCode) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << std::endl;
		msg << "\tSamplePsiSolver error occured" << std::endl;
		msg << "\tError code:\n\t\t" << errorCode;
		throw std::runtime_error(msg.str().c_str());
	}
}

#endif // _SAMPLEPSISOLVER_HXX_
