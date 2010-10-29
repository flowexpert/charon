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
/** @file PsiStencil.cpp
 *  Implementation of class PsiStencil.
 *  This is the PsiStencil which is
 *	derived from class Stencil.
 *  It is used to apply a robustness term on a stencil
 *  @see Stencil.cpp
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *
 *  @date 03.03.2010
 */

#define TYPE PsiStencil

#include <charon/Stencils/PsiStencil.hxx>

extern "C" psistencil_DECLDIR
ParameteredObject* create(const std::string &name, template_type t) {
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

extern "C" psistencil_DECLDIR
void destroy(ParameteredObject * b) {
	delete b;
}
