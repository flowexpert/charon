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
/** @file   ParameterType.h
 *  @brief  Declaration of class ParameterType
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef PARAMETERTYPE_H_
#define PARAMETERTYPE_H_
#include <QString>
#include <QColor>

/// Type of a NodeParameter
class ParameterType {
public:
	/// default constructor
	/// @param typeName      name of the type
	/// @param color         color of the type
	ParameterType(QString typeName,QColor color=Qt::gray);

	/// @returns the type name (with template string)
	QString getTypeName();

	/// @returns the type name without template string
	QString getTypeNameUnTemplated();

	/// @returns the templatename
	QString getTempName();

	/// @returns the color
	QColor getColor();

	/// sets the color of the type
	/// @param col     color to set
	void setColor(QColor col);

	/// sets the types template name
	/// @param name     name of the template
	void setTempName(QString name);

	/// sets the templated flag
	/// @param tmp     flag value
	void setTemplated(bool tmp);

	/// @returns if type is templated
	bool isTemplated();

	/// sets the types name
	/// @param name    name to set
	void setTypeName(QString name);

private:
	/// name of the type
	QString _typeName;

	/// color of the type
	QColor _color;

	/// templated flag of the type
	bool _templated;

	/// templatename of the type
	QString _tempName;
};

#endif /* PARAMETERTYPE_H_ */
