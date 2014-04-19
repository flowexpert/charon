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
/// @file Outputgen.cpp
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
/// Needed for this class to work as plugin.
#include "Outputgen.h"

#if defined(MSVC) && defined (outputgen_EXPORTS)
#define DECLDIR __declspec(dllexport)
#else
///Not needed with GCC
#define DECLDIR
#endif

extern "C" DECLDIR ParameteredObject* create(std::string name, template_type /*t*/) {
	return new Outputgen(name);
}

extern "C" DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}
