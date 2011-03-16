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

using namespace std;

ParameterType::ParameterType(QString typeName,QColor color) {
	this->typeName = typeName;
	this->color = color;
        this->templated = false;
}

QString ParameterType::getTypeName(){
    if(this->templated){
        return this->typeName + "<" + this->tempName + ">";
    }else{
        return this->typeName;
    }
	
}

QString ParameterType::getTempName(){
    return this->tempName;
}

QString ParameterType::getTypeNameUnTemplated(){
    return this->typeName;
}

QColor ParameterType::getColor(){
	return this->color;
}

void ParameterType::setColor(QColor color){
	this->color = color;
}

void ParameterType::setTypeName(QString name){
	this->typeName = name;
}

void ParameterType::setTemplated(bool tmp){
	this->templated = tmp;
}

bool ParameterType::isTemplated(){
	return this->templated;
}

void ParameterType::setTempName(QString tmpname){
	this->tempName = tmpname;
}

ParameterType::~ParameterType() {
	// TODO Auto-generated destructor stub
}
