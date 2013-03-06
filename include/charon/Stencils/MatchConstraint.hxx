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
/** \file MatchConstraint.hxx
 *  Implementation of the parameter class MatchConstraint.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 06.03.2013
 */

#ifndef _MATCHCONSTRAINT_HXX_
#define _MATCHCONSTRAINT_HXX_

#include <charon/Stencil.hxx>

#include "MatchConstraint.h"

/**
 *
 *  This stencil implements a matching constraint.
 *  Matching is being performed between the sought parameters
 *  and the given parameter guess at masked positions.
 *
 */

template <class T>
MatchConstraint<T>::MatchConstraint(const std::string& name) :
	Stencil::Base<T>("MatchConstraint", name,
			"<h2>Stencil for matching constraint."),
	mask(true,false)
{
	this->_addInputSlot(penaltyFunction,
	                    "penaltyFunction",
	                    "penalty function",
	                    "PenaltyFunction<T>*");

	this->_addInputSlot(parameterGuess,
	                    "parameterGuess",
	                    "current parameter guess",
	                    "CImgList<T>");

	this->_addInputSlot(roi, "roi", "region of interest", "Roi<int>*");

	this->_addInputSlot(mask, "mask", "mask indicating matching positions", "CImgList<T>");

	ParameteredObject::_addParameter(
			pUnknowns, "unknowns", "List of unknowns");

	ParameteredObject::_setTags("charon-flow;Stencils;CImg");
}

template <class T>
void MatchConstraint<T>::execute() {
	Stencil::Base<T>::execute();
	_lamb = this->lambda();
	_penaltyFunction = penaltyFunction();

	const Roi<int>& _roi = *(this->roi());

	_xBegin = _roi.xBegin();
	_xEnd = _roi.xEnd();
	_yBegin = _roi.yBegin();
	_yEnd = _roi.yEnd();

	// Copy the unknowns from the Parameter list into the set, which was
	// inherited from the Stencil class
	std::vector<std::string>::iterator puIt;
	for(puIt=pUnknowns().begin() ; puIt!=pUnknowns().end() ; puIt++) {
		this->_unknowns.insert(*puIt);
	}
}

template <class T>
void MatchConstraint<T>::updateStencil(
		const std::string& unknown,
		const Point4D<int>& p, const int&)
{
	// parameter value
	T paramValue;

	// mask
	bool maskValue = true;
	int maskConnected = mask.connected();
	if (maskConnected) {
		maskValue = (bool)mask()[0].atXY(p.x, p.y);
	}

	// fill stencil with masks
	for(unsigned int i=0; i< this->pUnknowns.size() ; i++) {
		SubStencil<T> entry;

		_dataMask.assign(1,1,1,1);
		_patternMask.assign(1,1,1,1);
		_center = Point4D<int>(0,0,0,0);
		_dataMask.fill( 0 );
		if (pUnknowns[i] == unknown) {
			_patternMask.fill( 1 );
		} else {
			_patternMask.fill( 0 );
		}
		this->_rhs = T(0.0);

		if (pUnknowns[i] == unknown)
		{
			if (maskValue) {
				_dataMask.fill( _lamb );
				paramValue = parameterGuess()[i].atXY( p.x, p.y );
				this->_rhs = _lamb * paramValue;
			}
		}

		entry.data = _dataMask;
		entry.pattern = _patternMask;
		entry.center = _center;
		this->_subStencils[pUnknowns[i]] = entry;
	}
}

template <class T>
cimg_library::CImg<T> MatchConstraint<T>::apply(
		const cimg_library::CImgList<T>& seq,
		const unsigned int frame) const
{
	return seq[frame].get_convolve(_dataMask);
}

#endif /* _MATCHCONSTRAINT_HXX_ */

