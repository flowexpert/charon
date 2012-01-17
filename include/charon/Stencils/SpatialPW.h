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
/** @file SpatialPW.h
 *  Declaration of class SpatialPW.
 *  @author <a href="jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 2010-01-18
 */

#ifndef _SPATIAL_PW_H_
#define _SPATIAL_PW_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef spatialpw_EXPORTS
/// Visual C++ specific code
#define spatialpw_DECLDIR __declspec(dllexport)
#else
#define spatialpw_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define spatialpw_DECLDIR
#endif

class DiffFunction;
class Diff2Function;
class Diff2LinFunction;
#include <charon/Stencil.h>

class Function;

/// This is the SpatialPW stencil
/** This stencil implements the spacial pairwise term of the
 *  lerning optical flow paper by Sun et. al.
 *
 *  \ingroup charon-stencils
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class spatialpw_DECLDIR SpatialPW : public Stencil<T> {
private:
	cimg_library::CImg<T> _patternMask; ///< common pattern mask
	Point4D<int> _center;               ///< common center

	/// calculate data pattern
	/** Calculates the cross-shaped 3x3 data pattern
	 *  at the given coordinates.
	 *  \param entry            preallocated image to use
	 *  \param[in] p            coordinates
	 *  \param[in] unknown      current unknown
	 *  \returns                rhs for given coordinates
	 */
	T _calculateData(
			cimg_library::CImg<T>& entry,
			const std::string& unknown,
			const Point4D<int>& p=Point4D<int>()) const;

	/// \name casted versions of pu and pv
	//\{
	const DiffFunction *_pu, *_pv;
	const Diff2Function *_pu2, *_pv2;
	const Diff2LinFunction *_puL, *_pvL;
	//\}

public:
	/// default constructor
	/// \param name instance name
	SpatialPW(const std::string& name = "");

	/// initial guess of flow
	InputSlot<cimg_library::CImgList<T> > flowGuess;
	/// fitted probabilities for u
	InputSlot<Function*> pu;
	/// fitted probabilities for v
	InputSlot<Function*> pv;
	/// drop initial guess terms from RHS
	Parameter<bool> calcDiff;
	/// use linearization of second derivative
	Parameter<bool> useDiff2Lin;

	/// Update stencil values
	/** \copydoc Stencil::updateStencil
	 *  \param[in] unknown    query substencils for this unknown,
	 *  \param[in] p,v        coordinates
	 */
	virtual void updateStencil(
			const std::string& unknown,
			const Point4D<int>& p=Point4D<int>(), const int& v=0);

	/// Calculate stencil expressions
	/** This will calculate the terms
	 *  \f[\left(\frac{\phi'^2}{\phi^2}-\frac{\phi''}{\phi}\right)\f]
	 *  using the four images \f$u_x\f$, \f$u_y\f$, \f$v_x\f$, \f$v_y\f$
	 *  as input. This corresponds to the lambda values of the l2norm stencil
	 *  applied along these directions.
	 *
	 *  \param[in] flow      flow field to perform calculations for
	 *  \param[in] frame     ignored (only flow fields with one time step
	 *                       are accepted)
	 *  \return              resulting images appended in x direction
	 */
	virtual cimg_library::CImg<T> apply(
			const cimg_library::CImgList<T>& flow,
			const unsigned int frame) const;

	/// calculate energy \f$E_S\f$ of the current flow guess
	virtual double getEnergy() const;

protected:
	/// Initialize Stencil
	/** \copydoc ParameteredObject::execute
	 */
	virtual void execute();
};

#endif // _SPATIAL_PW_H_
