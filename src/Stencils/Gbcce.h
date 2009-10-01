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
/** @file Gbcce.h
 *  Implementation of class Gbcce.
 *  This is the General Brightness Change Constraint Equation stencil which is
 *  derived from the stencil class.
 *  @see Stencil.h
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#ifndef _GBCCE_H_
#define _GBCCE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef gbcce_EXPORTS
///Visual C++ specific code
#define gbcce_DECLDIR __declspec(dllexport)
#else
#define gbcce_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define gbcce_DECLDIR
#endif

#include <Stencil.h>
#include <BrightnessModel.h>
#include <MotionModel.h>

template <class T>
class gbcce_DECLDIR Gbcce : public Stencil<T>
{
	public:
		///InputSlot for the brightness model to use.
		InputSlot<BrightnessModel<T>* > brightnessIn;
		
		///InputSlot for the motion model to use.
		InputSlot<MotionModel<T>* > motionIn;
		
		///default constructor
		Gbcce(const std::string& name = "");
		
		///main function.
		virtual void execute();
		
		///update the Stencil to contain the data that corresponds to the given coordinates.
		virtual void updateStencil(const unsigned int x, const unsigned int y,
		                   const unsigned int z=0, const unsigned int t=0,
		                   const unsigned int v=0);
		
		/**
		 * apply the stencil to a sequence.
		 * @remark not yet implemented.
		 */
		virtual cimg_library::CImg<T> apply(const cimg_library::CImgList<T>& seq,
		                                    const unsigned int frame) const;

		
		virtual ~Gbcce();
};

#endif //_GBCCE_H_
