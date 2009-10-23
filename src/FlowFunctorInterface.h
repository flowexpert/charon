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
/// @file FlowFunctorInterface.h
/// defines interface class FlowFunctorInterface
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _FlowFunctorInterface_h_
#define _FlowFunctorInterface_h_

#include "main.h"

/// interface class for the flow functors of the motion models
class FlowFunctorInterface
{
public:
	/**
 	*        computes the 2D flow
 	* @param[in] x relative x-coordinate
 	* @param[in] y relative y-coordinate
 	* @param[out] f array in which flow is returned
	*/
	virtual void operator() (const float x, const float y, float* f) const = 0 ;
	
	/**
	*        computes the 3D flow
	* @param[in] x relative x-coordinate
	* @param[in] y relative y-coordinate
	* @param[in] z relative z-coordinate	
	* @param[out] f array in which flow is returned
	*/	
	virtual void get3d(const float x, const float y, const float z, float* f) const = 0;
};

#endif
