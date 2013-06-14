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
/**	@file TypeDetector.h
 *	@brief Declaration of class TypeDetector
 *
 *	@date	17.12.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef TYPEDETECTOR_H_
#define TYPEDETECTOR_H_

#include <typeinfo>
#include <map>
#include <string>
#include "DllEx.h"

/// String representation generation.
/// This class gives a string representation for simple types as
/// int, uint, double, float etc.
namespace TypeDetector {
	/// Get type representation.
	/** Known types will result the proper representation,
	 *  unknown types will cause the original string to be returned.
	 *  \param typeInfo     result of typeid(bla).name()
	 *  \returns            string representation
	 */
	charon_core_DLL_PUBLIC
	std::string type(const std::string& typeInfo);

	/// deprecated, for compatibility, does nothing.
	/** Has been used in tests sometimes.
	 *  Please remove calls to this function. */
	charon_core_DLL_PUBLIC charon_DEPRECATED
	void destroy();
}

#endif /*TYPEDETECTOR_H_*/
