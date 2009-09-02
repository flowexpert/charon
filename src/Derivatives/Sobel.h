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

#ifndef _Derivatives_Sobel_h_
#define _Derivatives_Sobel_h_

#include "../main.h"
#include "../Derivative.h"

namespace Derivatives
{

/// @brief class implementing a sobel filter
/// @details this class uses a @f$ \frac{1,32} \left[ \begin{array}{ccc} 3 & 0 & -3 \\
///									10 & 0 & -10 \\
///									 3 & 0 & -3
///						 \end{array} \right] @f$ filter.
/// Only implemented for x and y direction
class Sobel : public Derivative
{
protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const
		{return new Sobel(name);}
	virtual void calculateDerivatives();
private:
	//cimg_library::CImg<> dx,dy,dz;
	cimg_library::CImg<> filterx, filtery, filterz;
public:
	/// default constructor
	Sobel(const std::string& name = "");
	
	/// @name output slots
	/// @brief containing the derivatives in the different directions
	//@{
	OutputSlot<cimg_library::CImgList<> > dx,dy;
	//@}


};

}  //end namespace


#endif
