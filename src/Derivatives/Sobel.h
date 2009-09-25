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
/// @file sobel.h
/// defines class Derivatives::Sobel
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 11.06.2009

#ifndef _DERIVATIVES_SOBEL_H_
#define _DERIVATIVES_SOBEL_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef sobel_EXPORTS
///Visual C++ specific code
#define sobel_DECLDIR __declspec(dllexport)
#else
#define sobel_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define sobel_DECLDIR
#endif

#include <Derivative.h>

/// @brief class implementing a sobel filter
/// @details this class uses a @f$ \frac{1,32} \left[ \begin{array}{ccc} 3 & 0 & -3 \\
///									10 & 0 & -10 \\
///									 3 & 0 & -3
///						 \end{array} \right] @f$ filter.
/// Only implemented for x and y direction
template <class T>
class sobel_DECLDIR Sobel : public Derivative<T>
{
private:
	//cimg_library::CImg<> dx,dy,dz;
	cimg_library::CImg<T> filterx, filtery, filterz;
public:
	/// default constructor
	Sobel(const std::string& name = "");
	
	virtual void execute();
	
	/// @name output slots
	/// @brief containing the derivatives in the different directions
	//@{
	OutputSlot<cimg_library::CImgList<T> > dx,dy;
	//@}
};

#endif
