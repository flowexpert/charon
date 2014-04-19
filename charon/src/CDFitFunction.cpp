/*  Copyright (C) 2010 Jens-Malte Gottfried

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
/** \file CDFitFunction.cpp
 *  This file is needed for class CDFitFunction to work as a plugin.
 *  \author Jens-Malte Gottfried
 *  \date 11.01.2010
 */

#include <charon-core/ParameteredObject.hxx>
#include <charon/CDFitFunction.h>

CDFitFunction::CDFitFunction(
		const std::string& cName,
		const std::string& iName,
		const std::string& doc) :
			ParameteredObject(cName, iName, doc)
{
	_addOutputSlot(self, "self",
			"self-pointer", "CDFitFunction*");
	_addOutputSlot(probability, "probability",
			"probability function", "Function*");
	_addOutputSlot(energy, "energy",
			"energy function", "Function*");
}

void CDFitFunction::execute() {
	self() = this;
	probability() = 0;
	energy() = 0;
}

CDFitFunction::~CDFitFunction() {
}

void CDFitFunction::normalize() {
}

double CDFitFunction::rangeHint(std::vector<double>::size_type) const {
	return 1.;
}

void CDFitFunction::printParameters() {
	sout << "CDFitFunction \"" << getName() << "\" fit parameters:\n\t";
	std::vector<double>::size_type ii;
	for(ii = 0; ii < fitParameters.size(); ii++)
		sout << fitParameters[ii] << ";";
	sout << std::endl;
}
