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
/** @file   TypeHandler.cpp
 *  @brief  Implementation of class TypeHandler
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "TypeHandler.h"
#include <math.h>
#include <iostream>
#include <QRegExp>
using namespace std;
QMap<QString, ParameterType*> TypeHandler::_types;

void TypeHandler::addType(ParameterType *ptype) {
	QColor color = Qt::gray;
	QString type = ptype->getTypeName();
	QRegExp exp("*<?*>*");
	exp.setPatternSyntax(QRegExp::Wildcard);
	if (exp.exactMatch(ptype->getTypeName())) {
		QString tname = ptype->getTypeName();
		type = ptype->getTypeName().remove(QRegExp("<?*>*", Qt::CaseSensitive, QRegExp::Wildcard));
		QString tmpname = ptype->getTypeName().remove(QRegExp("*<", Qt::CaseSensitive, QRegExp::Wildcard));
		tmpname = tmpname.remove(QRegExp(">*", Qt::CaseSensitive, QRegExp::Wildcard));
		/*cout << "has template: " << tname.toStdString() << endl;
		cout << "w.o.:" << type.toStdString() << endl;
		cout << "type:" << tmpname.toStdString() << endl;*/
		ptype->setTempName(tmpname);
		ptype->setTemplated(true);
		ptype->setTypeName(type);
	}
	if(ptype->getTypeNameUnTemplated().contains("cimg",Qt::CaseInsensitive)) color = Qt::yellow;
	else if(ptype->getTypeNameUnTemplated().contains("vigra",Qt::CaseInsensitive)) color = Qt::darkYellow;
	else if(ptype->getTypeNameUnTemplated().contains("roi",Qt::CaseInsensitive)) color = Qt::green;
	else if(ptype->getTypeNameUnTemplated().contains("interpolator",Qt::CaseInsensitive)) color = Qt::blue;
	// add more color presets here
	else color = Qt::gray;
	ptype->setColor(color);
	_types.insert(type, ptype);
}

bool TypeHandler::hasType(QString parameterTypeName) {
	QRegExp exp("*<?*>*");
	exp.setPatternSyntax(QRegExp::Wildcard);
	if (exp.exactMatch(parameterTypeName)) {
		parameterTypeName = parameterTypeName.remove(QRegExp("<?*>*", Qt::CaseSensitive, QRegExp::Wildcard));
	}
	ParameterType *pt = _types.value(parameterTypeName);
	if (pt != 0) {
		return true;
	}
	return false;
}

ParameterType *TypeHandler::getType(QString typeName) {
	QRegExp exp("*<?*>*");
	exp.setPatternSyntax(QRegExp::Wildcard);
	if (exp.exactMatch(typeName)) {
		typeName = typeName.remove(QRegExp("<?*>*", Qt::CaseSensitive, QRegExp::Wildcard));
	}
	ParameterType *pt = _types.value(typeName);
	if (pt != 0) {
		return pt;
	} else {
		cout << "error: requested unknown type:" << typeName.toStdString() << endl;
		return 0;
	}
}

void TypeHandler::inizialize() {
	_types = QMap<QString, ParameterType*>();
}
