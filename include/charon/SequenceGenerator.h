/*  Copyright (C) 2009 Jens-Malte Gottfried <jmgottfried@web.de>

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
/** \file SequenceGenerator.h
 *  Declaration of the parameter class SequenceGenerator.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 02.02.2010
 */

#ifndef _SEQUENCEGENERATOR_H_
#define _SEQUENCEGENERATOR_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef sequencegenerator_EXPORTS
/// Visual C++ specific code
#define sequencegenerator_DECLDIR __declspec(dllexport)
#else
#define sequencegenerator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define sequencegenerator_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Generate simple test sequences with ground truth
/** Currently, only sine sequences may be generated, but if needed,
 *  you can implement more.
 *  Dimension and frequencies may be provided.
 *
 *  \ingroup charon-flow-testing
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class sequencegenerator_DECLDIR SequenceGenerator :
		public TemplatedParameteredObject<T> {
public:

	/// test sequence dimensions (x;y;z;t) 
	ParameterList < unsigned int > size;
	/// sine wavelength along dimensions (in pixel) (lx;ly[;lz])
	ParameterList < T > wavelength;
	/// constant flow velocity (u;v[;w])
	ParameterList < T > velocities;
	/// operation type of processing (add, mul)
	Parameter < std::string > operation;

	/// generated test sequence 
	OutputSlot < cimg_library::CImgList<T> > sequence;
	/// ground truth 
	OutputSlot < cimg_library::CImgList<T> > flow;

	/// create a new SequenceGenerator object
	/// \param name          Instance name
	SequenceGenerator(const std::string& name);

	/// Update object.
	virtual void execute();
};

#endif // _SequenceGenerator_H_



