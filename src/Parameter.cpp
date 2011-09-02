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
/** @file Parameter.cpp
 *  Implements an operator for the class AbstractParameter.
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  @date 24.08.2009
 */
#include <charon-core/Parameter.h>

std::ostream & operator<<(
		std::ostream& os, const AbstractParameter& parameter) {
	parameter.intoStream(os);
	return os;
}

std::string AbstractParameter::_followLink(
		const ParameterFile& pf, const std::string paramName) const {
	if (pf.isSet(paramName)) {
		std::string refName = pf.get<std::string>(paramName);
		if(refName.size() > 0 && refName.substr(0,1) == "@") {
			refName = refName.erase(0,1);
			return refName;
		}
	}
	return paramName;
}
