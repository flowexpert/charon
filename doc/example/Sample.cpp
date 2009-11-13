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
/// @file Sample.cpp
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
/// Needed for this class to work as plugin.
#include "Sample.h"

#if defined(MSVC) && defined (sample_EXPORTS)
#define DECLDIR __declspec(dllexport)
#else
///Not needed with GCC
#define DECLDIR
#endif

extern "C" DECLDIR ParameteredObject* create(const std::string& name, template_type /*t*/) {
	return new Sample(name);
}

extern "C" DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}
