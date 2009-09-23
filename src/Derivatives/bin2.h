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
/// @file bin2.h
/// defines class Derivatives::bin2
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _DERIVATIVES_BIN2_H_
#define _DERIVATIVES_BIN2_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef bin2_EXPORTS
///Visual C++ specific code
#define bin2_DECLDIR __declspec(dllexport)
#else
#define bin2_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define bin2_DECLDIR
#endif

#include <Derivative.h>

/// @brief class implementing a simple derivative filter.
/// @details this class uses a @f$ \left[ \begin{array}{ccc} 1 & 0 & -1 \end{array} \right] @f$ filter
template <class T>
class bin2_DECLDIR bin2 : public Derivative<T>
{
public:
	/// default constructor
	bin2(const std::string& name = "");
	
	virtual void execute();

	/// @name output slots
	/// @brief containing the derivatives in the different directions
	//@{
	OutputSlot<cimg_library::CImgList<T>* > dx,dy,dz,dt;
	//@}
};

#endif