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
/** \file SimpleIterator.cpp
 *  This file is needed for class SimpleIterator to work as a plugin.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 01.02.2010
 */

/// Class name of the plugin
#define TYPE SimpleIterator

#include <charon/SimpleIterator.hxx>

#ifndef NO_BASE_CTOR_HACK
// These two classes are pure dummy classes.
// They cause the base constructor of
// SimpleIterator compiled and added into the
// generated library.
/// dummy class 1
template<typename T>
class Vktpmfxr : public SimpleIterator<T> {
public:
	Vktpmfxr() : SimpleIterator<T>("","","") {
	}
};

/// dummy class 2
class Mhvkcwgm {
public:
	Mhvkcwgm();
};

Mhvkcwgm::Mhvkcwgm() {
	_addConstructor(Vktpmfxr<double>());
	_addConstructor(Vktpmfxr<float>());
	_addConstructor(Vktpmfxr<int>());
}
#endif

/// Creates an instance of the plugin
extern "C" simpleiterator_DECLDIR ParameteredObject*
		create(const std::string & name, template_type t) {
	switch(t) {
	case ParameteredObject::TYPE_DOUBLE:
		return new TYPE<double>(name);
		break;
	case ParameteredObject::TYPE_FLOAT:
		return new TYPE<float>(name);
		break;
	case ParameteredObject::TYPE_INT:
		return new TYPE<int>(name);
		break;
	default:
		return new TYPE<int>(name);
		break;
	}
}

/// Deletes an instance of the plugin
extern "C" simpleiterator_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" simpleiterator_DECLDIR
ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD ;
#else
	return ParameteredObject::RELEASE_BUILD ;
#endif
}
