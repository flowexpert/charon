/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file   QParameterFile.cpp
 *  \brief  Implementation of class QParameterFile
 *  \date   15.03.2011
 *  \author <a href="mailto:jens-malte.gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 */

#include "QParameterFile.h"
#include <charon-core/ParameterFile.hxx>
#include <vector>

QParameterFile::QParameterFile(QString fileName) :
		_pFile(new ParameterFile())
{
	if(!fileName.isEmpty())
		load(fileName);
}

QParameterFile::~QParameterFile() {
	delete _pFile;
}

void QParameterFile::load(QString fileName) {
	_pFile->load(fileName.toStdString());
}

void QParameterFile::save(QString fileName) const {
	_pFile->save(fileName.toStdString());
}

bool QParameterFile::isSet(QString parameter) const {
	return _pFile->isSet(parameter.toStdString());
}

QString QParameterFile::get(QString parameter) const {
	return isSet(parameter) ?
			QString::fromStdString(
					_pFile->get<std::string>(parameter.toStdString())) :
			QString::null;
}

void QParameterFile::erase(QString parameter) {
	// also delete corresponding priority value if existent
	std::string str = parameter.toStdString();
	if (_pFile->isSet(str + ".editorpriority")) {
		_pFile->erase(str + ".editorpriority");
	}
	_pFile->erase(str);
}

void QParameterFile::set(QString parameter, QString value) {
	_pFile->set<std::string>(parameter.toStdString(),value.toStdString());
}

QStringList QParameterFile::getKeyList(QString beginsWith) const {
	std::vector<std::string> list =
			_pFile->getKeyList(beginsWith.toStdString());
	QStringList res;
	std::vector<std::string>::const_iterator ii;
	for(ii = list.begin(); ii != list.end(); ii++)
		res << QString::fromStdString(*ii);
	return res;
}

void QParameterFile::clear() {
	_pFile->clear();
}

QStringList QParameterFile::getList(QString parameter) const {
	return get(parameter).split(";",QString::SkipEmptyParts);
}

const ParameterFile& QParameterFile::toParameterFile() const {
	return *_pFile;
}
