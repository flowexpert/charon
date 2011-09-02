/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/** @file   parameterfiletest.cpp
 *  @brief  Tests for references of parameters in charon-core.
 *  @date   12.08.2011
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <cstdlib>
#include <charon-core/ParameteredObject.hxx>

/// sample ParameteredObject class.
class Sample : public ParameteredObject {

public:
	/// sample string parameter
	Parameter<std::string>   par1;
	/// sample int parameter list
	ParameterList<int> par2;

	/// create a new sample object
	/// @param name             Object name
	Sample(const std::string& name = "") :
			ParameteredObject("sample", name, "some sample class")
	{
		// parameters
		_addParameter (par1, "par1", "sample integer parameter");
		_addParameter (par2, "par2", "sample float parameter");
	}
};

/// Test application for References in Parameters and ParameterFiles
int main(){
	try {
		ParameterFile paramtest;
		ParameterFile outfile;

		Sample sample1("sample1");
		Sample sample2("sample2");

		paramtest.set("sample1.type", "sample");
		paramtest.set("sample1.par1", "@sample2.par1");
		paramtest.set("sample1.par2", "1;5;3;153");
		paramtest.set("sample2.type", "sample");
		paramtest.set("sample2.par1", "test string");
		paramtest.set("sample2.par2", "@sample1.par2");

		sample1.loadParameters(paramtest);
		sample2.loadParameters(paramtest);
		/*
		//-- start of disposable test code --
		std::cout << "- ParameterFile before -" << std::endl;
		paramtest.toStream(std::cout);
		//--- end of disposable test code ---
		*/

		// tests reference to a simple Parameter of type std::string
		if(sample1.par1() != sample2.par1())
			throw std::string("Couldn't load first referenced value (string).");

		// tests reference to a ParameterList of type int
		if(sample2.par2.size() != sample1.par2.size())
			throw std::string("Referenced ParameterList sizes do not match.");
		else {
			for(size_t i = 0; i < sample2.par2.size(); i++) {
				if(sample2.par2[i] != sample1.par2[i])
					throw std::string("Referenced ParameterList value mismatch.");
			}
		}

		// test overwritability of references
		bool testSuccess = false;
		sample1.par1() = "overwriting test";
		try {
			sample1.save(paramtest);
		}
		catch(std::runtime_error) {
			testSuccess = true;
		}

		if(!testSuccess)
			throw std::string("Overwriting a Reference succeeded but shouldn't have.");

		/*
		//-- start of disposable test code --
		std::cout << std::endl << "- values inside charon-core -" << std::endl;
		sample1.save(outfile);
		sample2.save(outfile);
		outfile.toStream(std::cout);
		std::cout << std::endl << "- ParameterFile after -" << std::endl;
		paramtest.toStream(std::cout);
		std::cin.get();
		//--- end of disposable test code ---
		*/
	}
	catch(const std::string& message) {
		std::cerr << "\n\nFailure:" << std::endl;
		std::cerr << message << std::endl;
		return EXIT_FAILURE;
	}
	catch(...) {
		std::cerr << "Unhandled exception!" << std::endl;
		return EXIT_FAILURE;
	}

	// Tests passed
	return EXIT_SUCCESS;
}
