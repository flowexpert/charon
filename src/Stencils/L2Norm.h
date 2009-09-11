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
/** @file L2Norm.h
 *  Implementation of class L2Norm.
 *  This is the L2Norm stencil which is derived from the stencil class.
 *  @see Stencil.h
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */


#ifndef _L2NORM_H_
#define _L2NORM_H_

#include <Stencil.h>

template <class T>
class L2Norm : public Stencil
{
	private:
		Parameter<unsigned int> dimensions;
		ParameterList<std::string> pUnknowns;	//parameter list of unknowns
	
	public:
		L2Norm(const std::string& name = "");
		
		void update();
		
		void updateStencil(const unsigned int x, const unsigned int y,
		                   const unsigned int z=0, const unsigned int t=0,
		                   const unsigned int v=0);
		
		~L2Norm();
};

#endif // _L2NORM_H_