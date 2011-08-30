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
/** @file CGSolver.hxx
 *  Implementation of class CGSolver.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  @date 30.08.2011
 */

#ifndef _CGSOLVER_HXX_
#define _CGSOLVER_HXX_

#include "CGSolver.h"
#include "CGMinimize.h"

#include <charon-core/ParameteredObject.hxx>
#include <vector>

template <typename T>
CGSolver<T>::CGSolver(const std::string& name) : 
		TemplatedParameteredObject<T>("CGSolver", name),
                energyStencils(false,true)
{
	this->_addInputSlot(energyStencils,
	                    "energyStencils",
	                    "energy stencils",
	                    "EnergyStencil<T>*");
	this->_addInputSlot(initialParameters,
	                    "initialParameters",
	                    "initial parameters",
	                    "CImgList<T>");
        this->_addOutputSlot(optimizedParameters,
                             "optimizedParameters",
                             "optimized parameters",
                             "CImgList<T>");
        this->_addParameter(length,
	                    "length",
	                    "see minimize.m for details", 10);
}

template <typename T>
CGSolver<T>::~CGSolver() {
}

template <typename T>
void CGSolver<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	optimizedParameters() = cimg_library::CImgList<T>( initialParameters() );

	_pSize     = initialParameters().size();
	_pWidth    = initialParameters()[0].width();
	_pHeight   = initialParameters()[0].height();
	_pDepth    = initialParameters()[0].depth();
	_pSpectrum = initialParameters()[0].spectrum();

	int vectorSize = _pSize * _pWidth * _pHeight * _pDepth * _pSpectrum;

	std::vector<T> _initialParameterVector( vectorSize, T(0.0) );
	std::vector<T> _optimizedParameterVector( vectorSize, T(0.0) );

	int idx = 0;
	int n, x, y, z, c;
	for (n=0; n<_pSize; ++n)
	for (x=0; x<_pWidth; ++x)
	for (y=0; y<_pHeight; ++y)
	for (z=0; z<_pDepth; ++z)
	for (c=0; c<_pSpectrum; ++c) {
		_initialParameterVector[idx] = T(
		    initialParameters().atNXYZC( n, x, y, z, c ) );
		++idx;
	}

	typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator _energyStencilsBegin;
	typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator _energyStencilsEnd;
	_energyStencilsBegin = this->energyStencils.begin();
	_energyStencilsEnd   = this->energyStencils.end();

	minimize(
	    _optimizedParameterVector,
	    _initialParameterVector,
	    _energyStencilsBegin,
	    _energyStencilsEnd,
            this->length()
        );

	idx = 0;
	for (n=0; n<_pSize; ++n)
	for (x=0; x<_pWidth; ++x)
	for (y=0; y<_pHeight; ++y)
	for (z=0; z<_pDepth; ++z)
	for (c=0; c<_pSpectrum; ++c) {
		optimizedParameters().atNXYZC( n, x, y, z, c ) =
		    _optimizedParameterVector[idx];
		++idx;
	}

}

#endif // _CGSOLVER_HXX_

