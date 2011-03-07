/*
 * ParameterType.cpp
 *
 *  Created on: 18.02.2011
 *      Author: jonathan
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
