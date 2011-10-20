/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/** \file EdgySequenceGenerator.h
 *  Declaration of the parameter class EdgySequenceGenerator.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 23.04.2010
 */

#ifndef _EDGYSEQUENCEGENERATOR_H_
#define _EDGYSEQUENCEGENERATOR_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef edgysequencegenerator_EXPORTS
/// Visual C++ specific code
#define edgysequencegenerator_DECLDIR __declspec(dllexport)
#else
#define edgysequencegenerator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define edgysequencegenerator_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon-utils/Roi.h>

template <typename T> class Interpolator;

/// Generate test sequences with motion edges
/** using a random pattern and some given mask representing a foreground
 *  and a background image.
 *  Velocities for foreground and background may be specified.
 *
 *  \ingroup charon-flow-testing
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class edgysequencegenerator_DECLDIR EdgySequenceGenerator :
		public TemplatedParameteredObject<T> {
public:
	/// Velocity of the foreground pattern
	ParameterList < double > foregroundVelocity;
	/// Velocity of the foreground pattern
	ParameterList < double > backgroundVelocity;
	/// Noise amplitude of the test pattern
	Parameter <float> noiseAmplitude;
	/// Numer of noise scales
	Parameter <int> noiseLevels;

	/// mask input
	InputSlot < cimg_library::CImgList<T> > mask;
	/// interpolator input
	InputSlot < Interpolator<T>* > interpolator;
	/// generated test sequence
	OutputSlot < cimg_library::CImgList<T> > sequence;
	/// generated test sequence
	OutputSlot < cimg_library::CImgList<T> > flow;
	/// sequence size
	OutputSlot<Roi<int>*> size;

	/// create a new EdgySequenceGenerator object
	/// \param name          Instance name
	EdgySequenceGenerator(const std::string& name);

	virtual ~EdgySequenceGenerator();

	/// Update object.
	virtual void execute();
};

#endif // _EDGYSEQUENCEGENERATOR_H_



