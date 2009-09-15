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

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef l2norm_EXPORTS
///Visual C++ specific code
#define l2norm_DECLDIR __declspec(dllexport)
#else
#define l2norm_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define l2norm_DECLDIR
#endif

#include <Stencil.h>

template <class T>
class l2norm_DECLDIR L2Norm : public Stencil<T>
{
	private:
		Parameter<unsigned int> dimensions;
		ParameterList<std::string> pUnknowns;	//parameter list of unknowns
	
	public:
		L2Norm(const std::string& name = "");
		
		virtual void execute();
		
		virtual void updateStencil(const unsigned int x, const unsigned int y,
		                   const unsigned int z=0, const unsigned int t=0,
		                   const unsigned int v=0);
		
		virtual ~L2Norm();
};

#endif // _L2NORM_H_