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

class ParameterType {
public:
	ParameterType(QString typeName,QColor color=Qt::gray);
	QString getTypeName();
        QString getTypeNameUnTemplated();
	QString getTempName();
	QColor getColor();
	void setColor(QColor col);
	void setTempName(QString name);
	void setTemplated(bool tmp);
	bool isTemplated();
	void setTypeName(QString name);
	virtual ~ParameterType();
private:
	QString typeName;
	QColor color;
	bool templated;
	QString tempName;
};

#endif /* PARAMETERTYPE_H_ */
