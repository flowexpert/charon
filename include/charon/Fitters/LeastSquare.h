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
/// @file LeastSquare.h
/// defines class Fitters::LeastSquare
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _Fitters_LeastSquare_h_
#define _Fitters_LeastSquare_h_

#include "../main.h"
#include "../Fitter.h"
#include "../functions.h"

/// namespace including the fitters
namespace Fitters
{

/// a fitter using total least square algorithm
class LeastSquare : public Fitter 
{
protected:
	virtual ParameteredObject* _newInstance(const std::string& name) const
		{return new LeastSquare(name);}	
public:
	/// default constructor
	LeastSquare(const std::string& name ="") : Fitter("fitters_leastsquare",name) {}
	
	virtual void computeParams(const cimg_library::CImg<>& img, cimg_library::CImg<float>& res);
};

}

#endif

