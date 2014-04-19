/*	Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.
    
    Tuchulcha is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tuchulcha is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/**	@file	VarTypeMap.cpp
 *	@brief	Implementation of class VarTypeMap
 *	@date	01.10.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "VarTypeMap.h"
#include <QMutexLocker>

// initialize static variables
VarTypeMap* VarTypeMap::_address(0);
QMutex VarTypeMap::_lock;

VarTypeMap::VarTypeMap() {
	// add types
	_typeMap["bool"]		= QVariant::Bool;
	_typeMap["int"]			= QVariant::Int;
	_typeMap["uint"]		= QVariant::UInt;
	_typeMap["char"]		= QVariant::Char;
	_typeMap["float"]		= QVariant::Double;
	_typeMap["double"]		= QVariant::Double;
	_typeMap["longlong"]	= QVariant::LongLong;
	_typeMap["ulonglong"]	= QVariant::ULongLong;
}

VarTypeMap::~VarTypeMap() {
}

VarTypeMap& VarTypeMap::instance() {
	QMutexLocker locker(&_lock);
	if(!_address)
		_address = new VarTypeMap;
	return *_address;
}

QVariant::Type VarTypeMap::operator[](QString type) const {
	QMap<QString, QVariant::Type>::const_iterator found;
	found = _typeMap.find(type);
	if (found == _typeMap.end())
		return QVariant::String;
	else
		return found.value();
}

