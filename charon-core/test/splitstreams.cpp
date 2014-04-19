/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/** @file splitstreams.cpp
 *  Unit tests for class SplitStream.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 11.08.2008
 */

#include <cassert>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
#include <charon-core/SplitStream.h>

/// Main application.
int main() {
	// start of the tests
	bool fail = false;
	std::cout << "Output to std::cout" << std::endl;

	std::vector<std::streambuf*> buffers;
	buffers.push_back(std::cout.rdbuf());
	std::streambuf* buf = new SplitStreamBuf(buffers);

	std::ostream* tout = new std::ostream(buf);
	*tout << "Output to std::ostream(buf);";
	*tout << " using numbers: " << (int) 4 << std::endl;

	delete tout;
	delete buf;
	tout = 0;
	buf = 0;

	sout.assign(std::cout);
	sout << "Output to SplitStream sout\n";
	sout << "Adress of sout (using manipulators): " << std::setw(20);
	sout << (void*) &sout << std::endl;

	std::ostringstream os;
	sout.assign(std::cout, os);
	std::string teststring = "Testing output to StringStream\n";
	sout << teststring;
	if(os.str() != teststring){
		std::cout << "String content des not match!" << std::endl;
		return EXIT_FAILURE;
	}

	// creating empy ostream* vector
	std::vector<std::ostream*> streamvec;
	fail = true;
	std::cout << "Testing expected exception..." << std::endl;
	try {
		// assigning emtpy vector has to rise exception
		sout.assign(streamvec);
	}
	catch(const std::invalid_argument& err) {
		std::cout << "Caught invalid_argument exception.\n";
		std::cout << "Exception message:\n" << err.what() << std::endl;
		fail = false;
	}
	if (fail) {
		std::cout << "No exception got!" << std::endl;
		return EXIT_FAILURE;
	}

	streamvec.push_back(&std::cout);
	sout.assign(streamvec);
	sout << "Test using new assigned sout" << std::endl;

	std::ofstream fileout("test.txt", std::ios::trunc);
	streamvec.push_back(&fileout);

	SplitStream sout2(std::cout, fileout);
	SplitStream sout3(streamvec);

	sout2 << "Line1 to file" << std::endl;
	sout3 << "Line2 to file" << std::endl;

	fileout.close();

	// read file content and check it
	std::ifstream reader("test.txt");
	fail = fail || reader.bad();
	if(!fail){
		std::string line;
		std::getline(reader, line);
		if (line != "Line1 to file"){
			std::cout << "Failure reading line 1" << std::endl;
			fail = true;
		}
		std::getline(reader, line);
		if (line != "Line2 to file"){
			std::cout << "Failure reading line 2" << std::endl;
			fail = true;
		}
		if (std::getline(reader, line)){
			std::cout << "End of file not reached!" << std::endl;
			fail = true;
		}
		if(!fail)
			std::cout << "File content sucessfully read." << std::endl;
	}
	reader.close();

	if(fail)
		return EXIT_FAILURE;

	sout.assign(std::cout);
	sout << "This goes only to std::cout." << std::endl;

	// tests passed sucessfully
	return EXIT_SUCCESS;
}
