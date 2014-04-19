/*  This file is part of Charon.

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
/** \file Sampler.cpp
 *  Implementation of parameter class Sampler.
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 17.11.2010
 */
#include <charon-core/ParameteredObject.hxx>
#include <charon/Sampler.h>
#include <charon-utils/DataManagerCImgList.hxx>

Sampler::Sampler(const std::string& cn, const std::string& n,
	const std::string& doc) : ParameteredObject(cn, n, doc)
{
	_addInputSlot(
			pdf, "pdf", "sampling probability distribution", "Function*");
	_addOutputSlot(
			out, "out", "self-pointer", "Sampler*");
}

Sampler::~Sampler() {
}

void Sampler::execute() {
	out() = this;
}
