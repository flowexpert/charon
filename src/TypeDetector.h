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

#ifdef DLLEX
#undef DLLEX
#endif // DLLEX
#ifdef WINDOWS
#ifdef CREATE_SHARED
#define DLLEX __declspec(dllexport)
#else  // CREATE_SHARED
#define DLLEX __declspec(dllimport)
#endif // CREATE_SHARED
#else  // WINDOWS
#define DLLEX
#endif // WINDOWS

#include <typeinfo>
#include <map>
#include <string>

/// String representation generation.
/// This class gives a string representation for simple types as
/// int, uint, double, float etc.
class DLLEX TypeDetector
{
private:
	/// Map that stores the type dictionary.
	std::map<std::string, std::string> _typemap;
	
	/// Forbid instanciation.
	TypeDetector();
	
	/// Instance pointer.
	static TypeDetector* _instance;

public:
	/// Get a TypeDetector instance.
	/// If no instance exists, this creates one.
	static const TypeDetector& instance();
	
	/// Get type representation.
	/// Known types will result the proper representation,
	/// unknown types will cause the original string to be returned.
	/// @param typeInfo		result of typeid(bla).name()
	std::string type(const std::string& typeInfo) const;
};

#endif /*TYPEDETECTOR_H_*/
