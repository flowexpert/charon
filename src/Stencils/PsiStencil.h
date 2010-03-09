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
/** @file PsiStencil.h
 *  Implementation of class PsiStencil.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 03.03.2010
 */

#ifndef _PSISTENCIL_H_
#define _PSISTENCIL_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef psistencil_EXPORTS
///Visual C++ specific code
#define psistencil_DECLDIR __declspec(dllexport)
#else
#define psistencil_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define psistencil_DECLDIR
#endif

#include "../Stencil.h"
#include "../BrightnessModel.h"
#include "../MotionModel.h"
#include "../RobustnessTerm.h"
//#include "../RobustnessTerm.hxx"

//class RobustnessTerm;

/// PsiStencil
/*
 *	This is the PsiStencil which is
 *	derived from class Stencil.
 *  It is used to apply a robustness term on a stencil
 */
template <class T>
class psistencil_DECLDIR PsiStencil : public Stencil<T>
{
	public:
		
		/// \name pointers used by the IteratorHelper
		//  \{
		/// InputSlot for the stencil.
		InputSlot<Stencil<T>*> stencilIn;
		/// CImgList for flow
		InputSlot<cimg_library::CImgList<T> > flowListIn;
		/// InputSlot for the RobustnessTerm.
		InputSlot<RobustnessTerm*> robustnessTermIn;
		//  \}

	
		/// default constructor
		PsiStencil(const std::string& name = "" /**[in] instance name*/);

		/// main function
		virtual void execute();

		virtual void updateStencil(
				const std::string& unknown,
				const unsigned int x=0,
				const unsigned int y=0,
				const unsigned int z=0,
				const unsigned int t=0,
				const unsigned int v=0);


		virtual void updateEnergy(
			const unsigned int x,
			const unsigned int y,
			const unsigned int z,
			const unsigned int t,
			const unsigned int v,
			const cimg_library::CImgList<T> flowList){};

		virtual cimg_library::CImg<T> apply(
				const cimg_library::CImgList<T>& seq,
				const unsigned int frame) const;


		virtual ~PsiStencil();

};

#endif //_PSISTENCIL_H_
