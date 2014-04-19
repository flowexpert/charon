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
/** @file TypeDetector.cpp
 *  @brief Implementation of class TypeDetector
 *
 *  @date   17.12.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#include <charon-core/TypeDetector.h>

std::string TypeDetector::type(const std::string& info) {
	if (info == typeid(           double).name())  { return "double"; }
	if (info == typeid(            float).name())  { return "float";  }
	if (info == typeid(              int).name())  { return "int";    }
	if (info == typeid(      std::string).name())  { return "string"; }
	if (info == typeid(             bool).name())  { return "bool";   }
	if (info == typeid(     unsigned int).name())  { return "uint";   }
	if (info == typeid(             char).name())  { return "char";   }
	if (info == typeid(         long int).name())  { return "long";   }
	if (info == typeid(unsigned long int).name())  { return "ulong";  }
	// long long int not in ISO C++ 1998
	/*
	if (info == typeid(         long long int).name()) { return "longlong"; }
	if (info == typeid(unsigned long long int).name()) { return "ulonglong"; }
	*/
	// else
	return info;
}

void TypeDetector::destroy() {
}
