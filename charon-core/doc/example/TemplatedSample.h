/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file TemplatedSample.h
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
/// Sample templated plugin class
#ifndef TEMPLATEDSAMPLE_H_
#define TEMPLATEDSAMPLE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef templatedsample_EXPORTS
///Visual C++ specific code
#define templatedsample_DECLDIR __declspec(dllexport)
#else
#define templatedsample_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define templatedsample_DECLDIR
#endif

#include "ParameteredObject.hxx"

template<typename T>
class templatedsample_DECLDIR TemplatedSample: public TemplatedParameteredObject<T> {
public:
	Parameter<double> sampleParameter;
	OutputSlot<T> sampleOutput;
	TemplatedSample(const std::string& name = "") :
		TemplatedParameteredObject<T> ("sample", name, "some sample class"),
		sampleParameter(1.5) {

		//Add the output slot. Use the this-pointer to call the method.
		this->_addOutputSlot(sampleOutput, "sampleoutput",
				"a sample output slot", "T");
		//Add the parameter. And specify a default value (here: 1.5).
		//Notice that I do not specify the template type.
		this->_addParameter(sampleParameter, "sampleparameter",
				"a sample parameter", 1.5);
	}
};

#endif /* TEMPLATEDSAMPLE_H_ */
