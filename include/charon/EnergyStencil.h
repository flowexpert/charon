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
/** @file EnergyStencil.h
 *  Declaration of the parameter class EnergyStencil.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *
 *  @date 13.10.2011
 */

#ifndef _ENERGYSTENCIL_H_
#define _ENERGYSTENCIL_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef energystencil_EXPORTS
///Visual C++ specific code
#define energystencil_DECLDIR __declspec(dllexport)
#else
#define energystencil_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define energystencil_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Base class for EnergyStencils
/** An energy stencil yields the energy and its gradient according to the
    current parameter vector within a multidimensional energy space.
 */
template <class T>
class energystencil_DECLDIR EnergyStencil : public TemplatedParameteredObject<T> {
public:
	/// Lambda coefficient of the stencil.
	Parameter<T> lambda;

	/// Output slot containing the this-pointer of the object
	OutputSlot<EnergyStencil<T>*> out;

	/// default constructor
	EnergyStencil(const std::string& classname /**[in] class name*/,
	              const std::string& name /**[in] instance name*/,
	              const std::string& doc /**[in] stencil documentation*/);

	/// function yielding stencil's energy wrt the parameter vector
	virtual T getEnergy( int n, int x, int y, int z, int c ) = 0;

	/// function yielding stencil's energy gradient wrt the parameter vector
	virtual std::vector<T> getEnergyGradient( int n, int x, int y, int z, int c ) = 0;

	/// function yielding count of gradient's components
	virtual int getGradientComponentsCnt() = 0;

	// default destructor
	virtual ~EnergyStencil();
};

#endif

