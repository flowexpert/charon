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
/** @file Rfmc.h
 *  Declaration of class Rfmc.
 *  @author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  @date 28.10.2010
 */

#ifndef _RFMC_H_
#define _RFMC_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef rfmc_EXPORTS
///Visual C++ specific code
#define rfmc_DECLDIR __declspec(dllexport)
#else
#define rfmc_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define rfmc_DECLDIR
#endif

#include "../Stencil.h"

/// Range Flow Motion Constraint equation
/**
 *  This is the motion constraint equation that is derived as follows:
 *  Depth is considered as a function of time and space:
 *  \f[z=z(x,y,t)\f]
 *  The total derivative wrt time leads to the
 *  <b>range flow motion constraint equation</b>
 *  (RFMC): \f[z_x u + z_y v + w + z_t = 0\f]
 *  \f$u,v,w\f$ are the components of the range flow vector consisting of
 *  the 2D optical flow \f$(u,v)\f$ and the motion \f$w\f$ in z direction.
 */
template <class T>
class rfmc_DECLDIR Rfmc : public Stencil<T>
{
public:
	/// select which channel represents depth data
	/// (useful if using combined greyvalue and depth images)
	Parameter<unsigned int> channelSelect;
	/// \name derivatives of the depth data wrt coordinate axes
	//@{
	InputSlot< cimg_library::CImgList<T> > zx;
	InputSlot< cimg_library::CImgList<T> > zy;
	InputSlot< cimg_library::CImgList<T> > zt;
	//@}

	/// default constructor
	Rfmc(const std::string& name = "" /**[in] instance name*/);

	/// main function
	virtual void execute();

	virtual void updateStencil(
		const std::string& unknown,
		const Point4D<int>& p=Point4D<int>(), const int& v=0);

	virtual cimg_library::CImg<T> apply(
			const cimg_library::CImgList<T>& seq,
			const unsigned int frame) const;

	virtual ~Rfmc();
};

#endif //_RFMC_H_
