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
/**
 *  @file RobustnessTerm.hxx
 *  @brief implementation of class RobustnessTerm
 *  @author <a href="mailto:techfreaq@web.de">Nina Hernitschek</a>
 *  @date 13.01.2010
 */

#include <ParameteredObject.hxx>
#include <charon/RobustnessTerm.h>

RobustnessTerm::RobustnessTerm(
		const std::string& classname, const std::string& name,
		const std::string& doc) :
		ParameteredObject(classname,name,doc +
				"<br><br>This class is used for calculationg Robustness Term")
{
	this->_addOutputSlot(out,"this","Pointer to itself","RobustnessTerm*");
	this->_addParameter(
			epsilon,"epsilon",
			"parameter of the Robustness Term",0.001,"double");
	out = this;
}

double RobustnessTerm::DPsi(double s)  {
	return 0;
}
