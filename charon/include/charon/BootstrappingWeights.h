/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file BootstrappingWeights.h
 *  Declaration of the parameter class BootstrappingWeights.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 20.03.2013
 */

#ifndef _BOOTSTRAPPINGWEIGHTS_H_
#define _BOOTSTRAPPINGWEIGHTS_H_

#ifdef _MSC_VER
#ifdef bootstrappingweights_EXPORTS
/// Visual C++ specific code
#define bootstrappingweights_DECLDIR __declspec(dllexport)
#else
#define bootstrappingweights_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define bootstrappingweights_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// This module computes so called bootstrapping weights
/** This module computes so called bootstrapping weights.
 *  
 *  Detailed documentation can be found within the paper
 *  "Bootstrap Optical Flow Confidence and Uncertainty Measure"
 *  by Jan Kybic and Claudia Nieuwenhuis (2011).
 *
 *  Algorithm 1 of that paper is being used.
 */
template <typename T>
class bootstrappingweights_DECLDIR BootstrappingWeights :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	BootstrappingWeights(const std::string& name = "");

	/// bootstrapping weights
	OutputSlot< cimg_library::CImgList<T> > weights;

	/// mask width
	Parameter< unsigned int > width;
	/// mask height
	Parameter< unsigned int > height;
	/// mask depth
	Parameter< unsigned int > depth;
	/// number of samples
	Parameter< unsigned int > samples;
	/// number of iterations
	Parameter< unsigned int > iterations;


protected:
	/// Update object.
	virtual void execute();
};

#endif // _BOOTSTRAPPINGWEIGHTS_H_

