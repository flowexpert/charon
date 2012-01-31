/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** @file AbstractStencil.h
 *  Declaration of the parameter class AbstractStencil.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *
 *  @date 31.01.2012
 */

#ifndef _ABSTRACTSTENCIL_H_
#define _ABSTRACTSTENCIL_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef abstractstencil_EXPORTS
///Visual C++ specific code
#define abstractstencil_DECLDIR __declspec(dllexport)
#else
#define abstractstencil_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define abstractstencil_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// namespace for stencils
/**
 *  IMPORTANT (!!) :  "AbstractStencil" is tentative and has to be renamed to "Stencil"
 *            (!!)   ("Stencil" to e.g. "Stencil::Mask")
 */
namespace AbstractStencil
{

/// Base class for abstractstencils
/** An abstractstencil ...
 */
template <class T>
class abstractstencil_DECLDIR Base : public TemplatedParameteredObject<T>
{
public:
	/// Lambda coefficient of the stencil.
	Parameter<T> lambda;

	/// Output slot containing the this-pointer of the object
	OutputSlot<AbstractStencil::Base<T>*> out;

	/// default constructor
	Base(const std::string& classname /**[in] class name*/,
	     const std::string& name /**[in] instance name*/,
	     const std::string& doc /**[in] stencil documentation*/);

	// default destructor
	virtual ~Base() {}
};

/// Interface for stencils yielding energy.
/** This class provides the interface to return the energy of a stencil.
 */
template <class T>
class abstractstencil_DECLDIR Energy
{
public:
	/// function yielding stencil's energy wrt the parameter vector
	virtual T getEnergy( int n, int x, int y, int z, int c ) = 0;
};

/// Class for stencils yielding the energy gradient.
/** This class provides an interface to the energy gradient of a stencil.
 */
template <class T>
class abstractstencil_DECLDIR EnergyGradient
{
public:
        /// function yielding stencil's energy gradient wrt the parameter vector
        virtual std::vector<T> getEnergyGradient( int n, int x, int y, int z, int c ) = 0;

        /// function yielding count of gradient's components
        virtual int getEnergyGradientDimensions() = 0;
};

}; // namespace AbstractStencil

#endif

