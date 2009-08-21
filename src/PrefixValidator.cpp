/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/**	@file	PrefixValidator.cpp
 *	@brief	Implementation of class PrefixValidator.
 *	@date	26.09.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "PrefixValidator.h"
#include "ParameterFile.hxx"

#include "PrefixValidator.moc"

PrefixValidator::PrefixValidator(const ParameterFile& data, QObject* myParent) :
		QValidator(myParent),
		_parameterFile(data) {
}

PrefixValidator::~PrefixValidator() {
}

QValidator::State PrefixValidator::validate(QString& input, int&) const {
	if(input.isEmpty())
		return QValidator::Acceptable;
    if (_parameterFile.getKeyList(std::string(input.toAscii()
            .constData())+".").size())
		return QValidator::Acceptable;
	std::vector<std::string> keyList = 
		_parameterFile.getKeyList(input.toAscii().constData());
	std::vector<std::string>::const_iterator iter;
	for(iter = keyList.begin(); iter != keyList.end(); iter++)
		if (iter->find(".") != std::string::npos)
			return QValidator::Intermediate;
	return QValidator::Invalid;
}

void PrefixValidator::fixup(QString& input) const {
	// do not change empty strings
	if(input.isEmpty())
		return;
	
	std::vector<std::string> res =
		_parameterFile.getKeyList(input.toAscii().constData());
	for(uint i=0; i<res.size(); i++) {
		std::string::size_type pos = res[i].find(".", input.length());
		if (pos != std::string::npos) {
			input = res[i].substr(0, pos).c_str();
			return;
		}
	}
}
