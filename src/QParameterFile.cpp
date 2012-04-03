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
#include <QFile>
#include <QTextStream>

QParameterFile::QParameterFile(QString fileName)
{
	if(!fileName.isEmpty())
		load(fileName);
}

void QParameterFile::load(QString fileName) {
	_content.clear();
	_keys.clear();
	QFile inFile(fileName);
	if (inFile.open(QFile::ReadOnly|QIODevice::Text)) {
		QString fileContent = QString::fromLocal8Bit(inFile.readAll());
		QTextStream str(&fileContent,QIODevice::ReadOnly);
		// regexp instances for content analysis
		QRegExp lb("(.*)\\\\\\s*");      // handle line breaks
		QRegExp cm("(.*)#.*");           // strip comments
		QRegExp kv("(\\S+)\\s+(\\S.*)"); // extract key/value
		QRegExp ko("(\\S+)\\s*");        // empty keys
		// line-based analyis
		QString line;
		uint lc = 0;
		while (!str.atEnd()) {
			// trim to avoid whitespace at begin/end
			line = str.readLine().trimmed();
			lc++;
			// allow line breaks with backslash
			while (lb.exactMatch(line)) {
				line = lb.cap(1) + str.readLine().trimmed();
				lc++;
			}
			// strip comments
			if (cm.exactMatch(line)) {
				line = cm.cap(1);
			}
			// skip empty lines
			if (line.isEmpty()) {
				continue;
			}
			// match key/value pair
			if (kv.exactMatch(line)) {
				QString par(kv.cap(1)), val(kv.cap(2));
				Q_ASSERT(!par.isEmpty());
				set(par,val);
			}
			else if (ko.exactMatch(line)) {
				QString par(ko.cap(1));
				Q_ASSERT(!par.isEmpty());
				set(par);
			}
			else {
				qDebug("%s:%d: malformed line",fileName.toAscii().constData(),lc);
			}
		}
	}
}

void QParameterFile::save(QString fileName) const {
	QFile outFile(fileName);
	if (outFile.exists()) {
		QFile backupFile(fileName + "~");
		if (backupFile.exists()) {
			backupFile.remove();
		}
		outFile.copy(fileName + "~");
	}
	if (outFile.open(QFile::WriteOnly|QIODevice::Truncate|QIODevice::Text)) {
		QTextStream strm(&outFile);
		QStringListIterator kIter(_keys);
		while (kIter.hasNext()) {
			QString cur = kIter.next();
			strm << cur << "\t\t" << _content.value(cur.toLower()) << endl;
		}
	}
}

bool QParameterFile::isSet(QString parameter) const {
	return _content.contains(parameter.toLower());
}

QString QParameterFile::get(QString parameter) const {
	return isSet(parameter) ?
		_content.value(parameter.toLower()) : QString::null;
}

void QParameterFile::erase(QString parameter) {
	// also delete corresponding priority value if existent
	if (isSet(parameter + ".editorpriority")) {
		_content.erase(_content.find((parameter + ".editorpriority").toLower()));
		const int& pos = _keys.indexOf(QRegExp(parameter + ".editorpriority",
					Qt::CaseInsensitive, QRegExp::FixedString));
		Q_ASSERT(pos >= 0); Q_ASSERT(pos < _keys.size());
		_keys.removeAt(pos);
	}
	if (isSet(parameter)) {
		_content.erase(_content.find(parameter.toLower()));
		const int& pos = _keys.indexOf(
				QRegExp(parameter,Qt::CaseInsensitive,QRegExp::FixedString));
		Q_ASSERT(pos >= 0); Q_ASSERT(pos < _keys.size());
		_keys.removeAt(pos);
	}
}

void QParameterFile::set(QString parameter, QString value) {
	if (parameter.isEmpty()) {
		qDebug("Empty parameter given in set()");
	}
	else {
		if (!isSet(parameter)) {
			_keys.append(parameter);
		}
		if (value.isNull()) {
			value = "";
		}
		_content.insert(parameter.toLower(),value);
	}
}

QStringList QParameterFile::getKeyList(QString beginsWith) const {
	QStringList ret;
	if (beginsWith.isEmpty()) {
		ret = _keys;
	}
	else {
		QStringListIterator ki(_keys);
		while (ki.hasNext()) {
			QString cur = ki.next();
			if (cur.startsWith(beginsWith,Qt::CaseInsensitive)) {
				ret << cur;
			}
		}
	}
	return ret;
}

void QParameterFile::clear() {
	_content.clear();
}
