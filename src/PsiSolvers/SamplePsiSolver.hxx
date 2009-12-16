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
	
	// just need read-only access to Image
	const Img<int>& globalImg = *(this->imgin());

	sout << "\tglobal Roi:\n\t\t" << globalImg << std::endl;

	/* 
		does something...

		write in imgout
	
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
