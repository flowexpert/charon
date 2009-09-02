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

#ifndef _Derivatives_bin2_h_
#define _Derivatives_bin2_h_

#include "../main.h"
#include "../Derivative.h"

/// namespace for the derivatives
namespace Derivatives
{

/// @brief class implementing a simple derivative filter.
/// @details this class uses a @f$ \left[ \begin{array}{ccc} 1 & 0 & -1 \end{array} \right] @f$ filter
class bin2 : public Derivative
{
protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const
		{return new bin2(name);}
	virtual void calculateDerivatives();
private:
	//cimg_library::CImg<> dx,dy,dz;
public:
	/// default constructor
	bin2(const std::string& name = "");
	
	/// @name output slots
	/// @brief containing the derivatives in the different directions
	//@{
	OutputSlot<cimg_library::CImgList<> > dx,dy,dz,dt;
	//@}
	
	/*virtual void getX(const cimg_library::CImg<>& img, cimg_library::CImg<>& res);
	virtual void getY(const cimg_library::CImg<>& img, cimg_library::CImg<>& res);
	virtual void getZ(const cimg_library::CImg<>& img, cimg_library::CImg<>& res); // */
};

}  //end namespace


#endif



