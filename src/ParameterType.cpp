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
/** @file   ParameterType.cpp
 *  @brief  Implementation of class ParameterType
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "ParameterType.h"

ParameterType::ParameterType (QString typeName,QColor color) :
	_typeName(typeName),
	_color(color),
	_templated(false)
{
}

QString ParameterType::getTypeName(){
	if (_templated) {
		return _typeName + "<" + _tempName + ">";
	} else {
		return _typeName;
	}
}

QString ParameterType::getTempName(){
	return _tempName;
}

QString ParameterType::getTypeNameUnTemplated(){
	return _typeName;
}

QColor ParameterType::getColor(){
	return _color;
}

void ParameterType::setColor(QColor color){
	_color = color;
}

void ParameterType::setTypeName(QString name){
	_typeName = name;
}

void ParameterType::setTemplated(bool tmp){
	_templated = tmp;
}

bool ParameterType::isTemplated(){
	return _templated;
}

void ParameterType::setTempName(QString tmpname){
	_tempName = tmpname;
}
