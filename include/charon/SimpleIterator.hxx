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
/** \file SimpleIterator.hxx
 *  Implementation of the parameter class SimpleIterator.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 01.02.2010
 */

#ifndef _SIMPLEITERATOR_HXX_
#define _SIMPLEITERATOR_HXX_

#include "SimpleIterator.h"
#include <limits>

template <typename T>
SimpleIterator<T>::SimpleIterator(const std::string& name) :
		TemplatedParameteredObject<T>("SimpleIterator", name,
			"Iterate parts of the processing pipeline.<br><br>"
			"This iterator executes the processing toolchain until the flow "
			"changes become small eanough or some given iteration count "
			"is reached.<br><br>"
			"You have to activate at least one stop criterion by "
			"setting maxRuns or epsilon to a value greater than "
			"zero."),
		flowInit(true,false), // optional
		stop(true,false) // optional
{
	_init();
}

template <typename T>
SimpleIterator<T>::SimpleIterator(
			const std::string& className,
			const std::string& instanceName,
			const std::string& doc) :
		TemplatedParameteredObject<T>(className, instanceName,
				doc + "<br><br>This class inherits SimpleIterator."),
		flowInit(true,false), // optional
		stop(true,false) // optional
{
	_init();
}

template <typename T>
void SimpleIterator<T>::_init() {
#ifdef QT_GUI_LIB
	ParameteredObject::_addParameter (breakPoint, "breakPoint",
		"break on iterator", false);
#endif
	ParameteredObject::_addParameter (maxRuns, "maxRuns",
		"maximum number of iterations (0 to disable)", 0u);
	ParameteredObject::_addParameter (epsilon, "epsilon",
		"flow change threshold (0 to disable)", 0.);
	ParameteredObject::_addParameter<std::string>(norm, "norm",
		"norm postprocessing", "max", "{max;mean}");
	ParameteredObject::_addParameter (accumulate, "accumulate",
		"set to true if iteration calculates flow difference, "
		"i.e. operates with zero initial guess", true);
	ParameteredObject::_addParameter(updateRate, "updateRate",
		"update rate, i.e. ratio of change that is used", 1.);

	ParameteredObject::_addInputSlot(flow, "flow",
		"flow result calculaged during current iteration", "CImgList<T>");
	ParameteredObject::_addInputSlot(flowInit, "flowInit",
		"initial flow guess if different from helper", "CImgList<T>");
	ParameteredObject::_addInputSlot(helper, "helper",
		"iteration helper input", "IteratorHelper<T>*");
	ParameteredObject::_addOutputSlot(result, "result",
		"final flow result after all iterations", "CImgList<T>");
	ParameteredObject::_addInputSlot(
		stop, "stop", "external stop criterion");

	// avoid undefined references
	_addConstructor(SimpleIterator<T>(
			std::string(),std::string(),std::string()));
	_addFunction(SimpleIterator<T>::singleStep);
	_addFunction(SimpleIterator<T>::finalize);
}

template <typename T>
void SimpleIterator<T>::_preIterationHook() {
	// dummy function for usage in derived classes
}

template <typename T>
void SimpleIterator<T>::_beginIterationHook() {
	// dummy function for usage in derived classes
}

template <typename T>
void SimpleIterator<T>::_afterIterationHook() {
	// dummy function for usage in derived classes
}

template <typename T>
void SimpleIterator<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;

#ifdef QT_GUI_LIB
	// add remote control to iterator
	_remoteControl = new SimpleIteratorRemoteControl(this->getName());
#endif

	initialize();
	iterate();
	finalize();
	assert(ParameteredObject::executed());

#ifdef QT_GUI_LIB
	delete _remoteControl;
#endif
}

template <typename T>
void SimpleIterator<T>::initialize() {
	// initialize helper object
	helper()->execute();
	helper()->reset();

	if (updateRate() < 0.) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << ":\n\t";
		msg << "Update Rate has to be positive! (is " << updateRate() << ")";
		throw std::invalid_argument(msg.str().c_str());
	}

	// To insert code before the first iteration starts,
	// use this hook in derived classes.
	_preIterationHook();

	if (!maxRuns() && !(epsilon() > std::numeric_limits<double>::min())) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << ":\n\t";
		msg << "You have to set at least one of maxRuns and epsilon to a ";
		msg << "value greater than zero.";
		throw std::invalid_argument(msg.str().c_str());
	}
}

template <typename T>
void SimpleIterator<T>::iterate() {
#ifdef QT_GUI_LIB
	int ret;
#endif
	bool cont;
	do {
#ifdef QT_GUI_LIB
	        if (breakPoint()) {
        	        _remoteControl->setModal(true);
                	ret = _remoteControl->exec();
			sout << "ret = " << ret << std::endl;
			switch (ret) {
			case 1:    //    STEP
				break;
			case 2:    //    BREAK
				return;
				break;
			case 3:    //    CONTINUE
				breakPoint() = false;
				break;
			default:
				breakPoint() = false;
				break;
			}
       		 }
#endif
		cont = singleStep();
		if (stop.connected()) {
			cont = cont && !stop();
		}
	} while (cont);
}

template <typename T>
bool SimpleIterator<T>::singleStep() {
	prepareStep();
	performStep();
	return finishStep();
}

template <typename T>
void SimpleIterator<T>::prepareStep() {
	// To insert code at the beginning of each iteration,
	// use this hook in derived classes.
	_beginIterationHook();
	sout << "\tstarting iteration " << helper()->count()+1 << " of ";
	sout << this->getClassName() << " \"";
	sout << this->getName() << "\"" << std::endl;
	helper()->resetExecuted(true);
}


template <typename T>
void SimpleIterator<T>::performStep() {
	// iteration by calling execute() on all parent objects,
	// since the iteration helper object is only executed once,
	// only the "inner objects" are iterated.
	ParameteredObject::execute();
}


template <typename T>
bool SimpleIterator<T>::finishStep() {
	IteratorHelper<T>* help = helper();
	sout << "\titeration " << help->count()+1 << " of ";
	sout << this->getClassName() << " \"";
	sout << this->getName() << "\" finished\n";

	assert(updateRate() >= 0.);
	assert(updateRate() <= 1.);
	cimg_library::CImgList<T>& helpFlow = help->flow();
	const cimg_library::CImgList<T>& oldFlow =
			(flowInit.connected() ? flowInit() : help->flow());
	static cimg_library::CImgList<T> newFlow;
	static cimg_library::CImgList<T> diffFlow;
	const cimg_library::CImgList<T>& curFlow = flow();
	double curChange;

	sout << "\t\told flow size: "
			<< oldFlow[0].width() << "x"
			<< oldFlow[0].height() << "\n\t\tq";

	sout << "old flow mean: (";
	cimglist_for(oldFlow,kk)
		sout << oldFlow[kk].mean() << "; ";

	sout << ")\n\t\titeration flow size: "
			<< curFlow[0].width() << "x"
			<< curFlow[0].height() << "\n\t\t";

	sout << "mean flow result from iteration: (";
	cimglist_for(curFlow,kk)
			sout << curFlow[kk].mean() << "; ";
	sout << ")" << std::endl;

	// calculate new flow
	curChange = 0.;
	newFlow.assign(curFlow);
	if (accumulate()) {
		cimglist_for(newFlow, kk)
			newFlow[kk] += oldFlow[kk];
		sout << "\t\tadding results to previous flow (accumulate)\n";
	}

	// maximum runs criterion (count() is updated later -> -1)
	bool cont = !maxRuns() || help->count() < (maxRuns()-1);
	if (cont && epsilon() > 0) {
		// calculate flow changes
		diffFlow.assign(newFlow);
		sout << "\t\tdifference flow mean: (";
		cimglist_for(diffFlow, kk) {
			diffFlow[kk] -= oldFlow[kk];
			sout << diffFlow[kk].mean() << "; ";
		}
		sout << ")" << std::endl;

		cimg_library::CImg<T> norms(curFlow[0]);

		// calculate 2-norm of flow difference (for each pixel)
		norms.fill(T(0));
		cimglist_for(diffFlow,kk)
			norms += diffFlow[kk].get_sqr();
		norms.sqrt();

		// seach maximum or mean flow difference
		if (norm() == "max")
			curChange = norms.max();
		else if (norm() == "mean")
			curChange = norms.mean();
		else {
			std::ostringstream msg;
			msg << __FILE__ << ":" << __LINE__ << "\n\t";
			msg << "norm has to be \"max\" or \"mean\"!";
			throw std::invalid_argument(msg.str().c_str());
		}

		// flow change criterion
		sout << "\tcurrent " << norm() << " flow change: ";
		sout << curChange << std::endl;
		cont = epsilon() <=0 || curChange >= epsilon();
	}
	if(updateRate() != 1.) {
		// implement possibility to change step size
		cimg_library::CImgList<T> temp(oldFlow);
		cimglist_for(temp, kk) {
			temp[kk] += updateRate() * (newFlow[kk]-temp[kk]);
		}
		sout << "\t\tupdate rate: " << updateRate() << "\n";
		helpFlow.assign(temp);
	}
	else {
		helpFlow.assign(newFlow);
	}

	sout << "\t\tnew flow size: "
			<< helpFlow[0].width() << "x"
			<< helpFlow[0].height() << "\n";
	sout << "\t\tnew flow mean: (";
	cimglist_for(helpFlow, kk)
		sout << helpFlow[kk].mean() << "; ";
	sout << ")" << std::endl;
	help->update();
	return cont;
}

template <typename T>
void SimpleIterator<T>::finalize() {
	result() = helper()->flow();

	// To insert code after the last iteration,
	// use this hook in derived classes.
	_afterIterationHook();
}

#endif /* _SIMPLEITERATOR_HXX_ */

