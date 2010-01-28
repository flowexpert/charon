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

/** @file VigraCheck.cpp
 *  Try to complile some code using Vigra library
 *
 *  This checks the Vigra version and creates an executable file
 *  which will print this version to stdout.
 *
 *  @author <a href="jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 2010-01-28
 */

#include <vigra/config.hxx>

#ifndef VIGRA_VERSION
#warning "VIGRA_VERSION not defined!"
#endif

#include <iostream>

int main() {
	//std::cout << VIGRA_VERSION << std::endl;
	std::cout << VIGRA_VERSION_MAJOR << "." << VIGRA_VERSION_MINOR
		<< "." << VIGRA_VERSION_PATCH << std::flush;
	return 0;
}
