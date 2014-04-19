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
/**	@file	PrefixValidator.cpp
 *	@brief	Implementation of class PrefixValidator.
 *	@date	26.09.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "PrefixValidator.h"
#include "QParameterFile.h"

PrefixValidator::PrefixValidator(const QParameterFile& data, QObject* pp) :
		QValidator(pp),
		_parameterFile(data)
{
}

QValidator::State PrefixValidator::validate(QString& input, int&) const {
	if(input.isEmpty())
		return QValidator::Acceptable;
	if (_parameterFile.getKeyList(input+".").size() > 0)
		return QValidator::Acceptable;
	QStringList keyList =  _parameterFile.getKeyList(input);
	for (int ii=0; ii < keyList.size(); ii++)
		if (keyList[ii].contains(QRegExp(
				"^\\s*"+ input + ".*\\..*",Qt::CaseInsensitive)))
			return QValidator::Intermediate;
	return QValidator::Invalid;
}

void PrefixValidator::fixup(QString& input) const {
	// do not change empty strings
	if(input.isEmpty())
		return;
	
	QStringList keyList = _parameterFile.getKeyList(input);
	for(int ii=0; ii < keyList.size(); ii++) {
		if (keyList[ii].contains(QRegExp(
				"^\\s*"+ input + ".*\\..*",Qt::CaseInsensitive))) {
			input = keyList[ii];
			return;
		}
	}
}
