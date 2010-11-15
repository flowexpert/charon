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
/** \file RelaxingIterator.cpp
 *  This file is needed for class RelaxingIterator to work as a plugin.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 05.02.2010
 */

/// Class name of the plugin
#define TYPE RelaxingIterator

#include <charon/RelaxingIterator.hxx>

/// Creates an instance of the plugin
extern "C" relaxingiterator_DECLDIR ParameteredObject*
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

///Deletes an instance of the plugin
extern "C" relaxingiterator_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}



