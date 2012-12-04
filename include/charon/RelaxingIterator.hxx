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
/** \file RelaxingIterator.hxx
 *  Implementation of the parameter class RelaxingIterator.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 05.02.2010
 */

#ifndef _RELAXINGITERATOR_HXX_
#define _RELAXINGITERATOR_HXX_

#include "RelaxingIterator.h"
#ifndef NDEBUG
#include <limits>
#endif

template <typename T>
RelaxingIterator<T>::RelaxingIterator(const std::string& name) :
		SimpleIterator<T>("RelaxingIterator", name,
			"Iterator based on SimpleIterator class but with additional "
			"relaxing feature.<br><br>"
			"Stencils may be connected and iteration causes the Stencil "
			"weights to be multiplied by a factor moving from 0 to 1 "
			"(some with increasing, some with decreasing weight).<br>"
			"This helps to use convex optimization to generate a good "
			"initial guess and then switch to some non-convex optimization "
			"problem."),
		initial(true, true), // multiSlots
		final(true, true)
{
	ParameteredObject::_addInputSlot(initial, "initial",
		"Stencils which weight will be decreased, starting with full weight.",
		"Stencil<T>*");
	ParameteredObject::_addInputSlot(final, "final",
		"Stencils which weight will be increased, starting with zero weight.",
		"Stencil<T>*");
	ParameteredObject::_addParameter(initialWeight, "initialWeight",
		"mixture at first iteration", 0.);
	ParameteredObject::_addParameter(finalWeight, "finalWeight",
		"mixture at last iteration", 1.);
	_addFunction(RelaxingIterator<T>::getCur);

	ParameteredObject::_setTags("charon-flow;Iterators");
}

template <typename T>
void RelaxingIterator<T>::_preIterationHook() {
	// check preconditions: fixed number of iterations
	if (this->epsilon() > 0 || this->maxRuns() == 0) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "Relaxing does work with fixed number of itrations only.\n\t";
		msg << "Please set epsilon to zero and maxRuns to some ";
		msg << "positive value.";
		throw std::invalid_argument(msg.str().c_str());
	}
	// initialWeight() in [0;1]
	if(initialWeight() < 0. || initialWeight() > 1.) {
		if (this->epsilon() > 0 || this->maxRuns() == 0) {
			std::ostringstream msg;
			msg << __FILE__ << ":" << __LINE__ << "\n\t";
			msg << "Initial Weight has to be in range [0;1]!\n\t";
			msg << "Found: " << initialWeight();
			throw std::invalid_argument(msg.str().c_str());
		}
	}
	// finalWeight() in [0;1]
	if(finalWeight() < 0. || finalWeight() > 1.) {
		if (this->epsilon() > 0 || this->maxRuns() == 0) {
			std::ostringstream msg;
			msg << __FILE__ << ":" << __LINE__ << "\n\t";
			msg << "Final Weight has to be in range [0;1]!\n\t";
			msg << "Found: " << finalWeight();
			throw std::invalid_argument(msg.str().c_str());
		}
	}

	// prepare stencils
	ParameteredObject::runPreceeding(initial);
	ParameteredObject::runPreceeding(final);

	_initialLambdas.clear();
	for (unsigned int i=0; i<initial.size(); i++)
		_initialLambdas.push_back(initial[i]->lambda());
	assert(_initialLambdas.size() == initial.size());
	_finalLambdas.clear();
	for (unsigned int i=0; i<final.size(); i++)
		_finalLambdas.push_back(final[i]->lambda());
	assert(_finalLambdas.size() == final.size());
}

template <typename T>
double RelaxingIterator<T>::getCur() const {
	const double& lambda =
		(this->maxRuns() > 1) ?
			double(this->helper()->count()) / double(this->maxRuns()-1) : 0.;
	assert(lambda >= 0.);
	assert(lambda <= 1.);
	return finalWeight() * lambda + initialWeight() * (1.-lambda);
}

template <typename T>
void RelaxingIterator<T>::_beginIterationHook() {
	const double& cur = getCur();

	assert(_initialLambdas.size() == initial.size());
	for (unsigned int i=0; i<_initialLambdas.size(); i++) {
		const double l = (1.-cur) * _initialLambdas[i];
		initial[i]->lambda() = T(l);
		assert(double(initial[i]->lambda()) == l);
	}
	assert(_finalLambdas.size() == final.size());
	for (unsigned int i=0; i<_finalLambdas.size(); i++) {
		const double l = cur * _finalLambdas[i];
		final[i]->lambda() = T(l);
		assert(double(final[i]->lambda()) == l);
	}
}

template <typename T>
void RelaxingIterator<T>::_afterIterationHook() {
	assert(_initialLambdas.size() == initial.size());
	for (unsigned int i=0; i<_initialLambdas.size(); i++)
		initial[i]->lambda() = _initialLambdas[i];

	assert(_finalLambdas.size()   == final.size());
	for (unsigned int i=0; i<_finalLambdas.size(); i++)
		final[i]->lambda() = _finalLambdas[i];
}

#endif /* _RELAXINGITERATOR_HXX_ */
