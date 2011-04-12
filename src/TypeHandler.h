/*	Copyright (C) 2011 Jonathan Wuest

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file   TypeHandler.h
 *  @brief  Declaration of class TypeHandler
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef TYPEHANDLER_H_
#define TYPEHANDLER_H_

#include "ParameterType.h"
#include <QMap>

/// Static class that handles type grouping and template detection
class TypeHandler {
public:
	/// adds a type to the _types map
	/// @param ptype   ParameterType to add
	static void addType(ParameterType *ptype);

	/// checks if type has already been added
	/// @param parameterTypeName   name of the type to check
	/// @returns true if type has already been added
	static bool hasType(QString parameterTypeName);

	/// @returns the ParameterType if set
	/// @param typeName   name of the type to return
	static ParameterType *getType(QString typeName);

	/// initalizes the Handler
	static void inizialize();
private:
	/// map containing all types in use
	static QMap<QString,ParameterType*> _types;
};

#endif /* TYPEHANDLER_H_ */
