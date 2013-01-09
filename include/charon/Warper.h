/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

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

/** @file Warper.h
 *  Declaration of the parameter class Warper.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *
 *  @date 09.01.2013
 */

#ifndef _WARPER_H_
#define _WARPER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef warper_EXPORTS
///Visual C++ specific code
#define warper_DECLDIR __declspec(dllexport)
#else
#define warper_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define warper_DECLDIR
#endif

#include <charon-utils/CImg.h>
#include <charon-core/ParameteredObject.h>

/// Base class for Warpers
/** A warper maps a simple position (e.g. an out of bounds position)
 *  to its effective position (e.g. boundary handling is performed).
 *
 *  The warper base class implements basic boundary handling using
 *  radial interpolation.
 */
template <class T>
class warper_DECLDIR Warper : public TemplatedParameteredObject<T> {
public:

	/// Input slot containing the motion
	InputSlot< cimg_library::CImgList<T> > motion;

	/// Output slot containing the this-pointer of the object
	OutputSlot<Warper<T>*> out;

	/// default constructor
	Warper(const std::string& name = "");

	/// functions yielding effective N, X, Y, Z, and C values
	//\{
	unsigned int getN( int N );
	unsigned int getX( int X );
	unsigned int getY( int Y );
	unsigned int getZ( int Z );
	unsigned int getC( int C );
	//\}

protected:
	/// \implements ParameteredObject::execute()
	virtual void execute();

	/// cache members
	//\{
	unsigned int _dimN;
	unsigned int _dimX;
	unsigned int _dimY;
	unsigned int _dimZ;
	unsigned int _dimC;

	cimg_library::CImgList<T> _motion;
	//\}
};

#endif

