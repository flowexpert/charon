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
#include <QTextCodec>

QParameterFile::QParameterFile(QString fileName)
{
	if(!fileName.isEmpty())
		load(fileName);
}

void QParameterFile::load(QString fileName, QString encoding) {
	_content.clear();
	_keys.clear();
	QFile inFile(fileName);
	if (inFile.open(QFile::ReadOnly|QIODevice::Text)) {
		QString fileContent;
		if (encoding.isEmpty()) {
			fileContent = QString::fromLocal8Bit(inFile.readAll());
		}
		else {
			QTextCodec* decoder = QTextCodec::codecForName(encoding.toLatin1());
			if (!decoder) {
				decoder = QTextCodec::codecForLocale();
				QTextStream qerr(stderr,QIODevice::WriteOnly);
				qerr << "Selected encoding: " << decoder->name() << endl;
			}
			fileContent = decoder->toUnicode(inFile.readAll());
		}
		QTextStream str(&fileContent,QIODevice::ReadOnly);
		// regexp instances for content analysis
		QRegExp lb("(.*)\\\\\\s*");       // handle line breaks
		QRegExp cm("(.*\\S)?\\s*#.*");    // strip comments
		QRegExp kv("(\\S+)\\s*(\\S.*)?"); // extract key/value
		// line-based analyis
		QString line;
		uint lc = 0;
		while (!str.atEnd()) {
			// trim to avoid whitespace at begin/end
			line = str.readLine().trimmed();
			lc++;
			// strip comments
			if (cm.exactMatch(line)) {
				line = cm.cap(1);
			}
			// allow line breaks with backslash
			while (lb.exactMatch(line)) {
				QString next = str.readLine().trimmed();
				// strip comments
				if (cm.exactMatch(next)) {
					next = cm.cap(1);
				}
				line = lb.cap(1) + next;
				lc++;
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
			else {
				qDebug("%s:%d: malformed line",fileName.toLocal8Bit().constData(),lc);
			}
		}
	}
}

void QParameterFile::save(QString fileName) const {
	QFile outFile(fileName);
	if (outFile.exists()) {
		QString backupFileName(fileName + "~");
		QFile::remove(backupFileName);
		outFile.copy(backupFileName);
	}
	if (outFile.open(QFile::WriteOnly|QIODevice::Truncate|QIODevice::Text)) {
		QTextStream strm(&outFile);
		strm << toStringList().join("\n") << endl;
	}
}

QStringList QParameterFile::toStringList() const {
	QStringList res;
	for (QStringList::const_iterator kIter=_keys.constBegin();
			kIter!=_keys.constEnd(); kIter++) {
		res << QString("%1\t\t%2")
			.arg(*kIter).arg(_content.value(kIter->toLower()));
	}
	return res;
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

const QRegExp QParameterFile::prefixCheck("[a-z_][a-zA-Z0-9_\\-]*",Qt::CaseSensitive);
const QRegExp QParameterFile::postfixCheck("(?:\\.[a-zA-Z0-9_\\-]+)*",Qt::CaseSensitive);
const QRegExp QParameterFile::paramCheck("[a-z_][a-zA-Z0-9_\\-\\.]*",Qt::CaseSensitive);
const QRegExp QParameterFile::paramCheckSloppy("[\\w_\\-\\.@]*",Qt::CaseInsensitive);

bool QParameterFile::rename(QString oldPrefix, QString newPrefix) {
	// some sanity checks on the given parameters
	oldPrefix=oldPrefix.section(".",0,0);
	if (oldPrefix.isEmpty() || newPrefix.isEmpty() || oldPrefix == newPrefix) {
		return false;
	}
	QRegExp check(prefixCheck);
	if (!check.exactMatch(newPrefix)) {
		qDebug("QParameterFile::rename: new prefix is no valid prefix: %s",
			newPrefix.toLocal8Bit().constData());
		return false;
	}
	// collect keys to rename
	QStringList keysToRename;
	QRegExp hasPrefix(oldPrefix+"(\\.[\\w\\.\\-_]+)?",Qt::CaseInsensitive);
	for (QStringList::const_iterator kIter = _keys.constBegin();
					kIter!=_keys.constEnd(); kIter++) {
		if (hasPrefix.exactMatch(*kIter)) {
			keysToRename << *kIter;
			QString newKey = newPrefix+hasPrefix.cap(1);
			if (_keys.contains(newKey,Qt::CaseInsensitive)
				|| _content.contains(newKey.toLower())) {
				qDebug("new Key \"%s\" already exists! abort",
					newKey.toLocal8Bit().constData());
				return false;
			}
		}
	}
	if (keysToRename.isEmpty()) {
		// nothing to do
		qDebug("no keys found starting with prefix \"%s\"",
			oldPrefix.toLocal8Bit().constData());
		return false;
	}
	// rename the keys, this modifies _keys and _content,
	// so it is done after iterating over it
	for (QStringList::const_iterator rIter = keysToRename.constBegin();
					rIter!=keysToRename.constEnd(); rIter++) {
		QString cur = *rIter;
		hasPrefix.exactMatch(cur);
		Q_ASSERT(hasPrefix.matchedLength() >= 0);
		int pos = _keys.indexOf(QRegExp(cur,Qt::CaseInsensitive));
		Q_ASSERT(pos >= 0);
		Q_ASSERT(_content.contains(cur.toLower()));
		QString val = _content.value(cur.toLower());
		_keys.removeAt(pos); // remove old key
		Q_ASSERT(!_keys.contains(cur,Qt::CaseInsensitive));
		_content.remove(cur.toLower());
		Q_ASSERT(!_content.contains(cur.toLower()));
		QString newKey = newPrefix+hasPrefix.cap(1);
		_keys << newKey; // insert renamed one
		_content.insert(newKey.toLower(),val);
	}
	// rename the parameters containing the old prefix,
	// this keeps _keys and the keys of _content unchanged, so
	// renaming can be done while iterating over _content
	QRegExp paramMatch(
			QString("((?:%1;)*)(%3)(%2)?((?:;%1)*)")
				.arg(paramCheckSloppy.pattern())
				.arg(postfixCheck.pattern())
				.arg(oldPrefix),Qt::CaseInsensitive);
	for (QHash<QString,QString>::iterator cIter=_content.begin();
			cIter!=_content.end();cIter++) {
		while (paramMatch.exactMatch(cIter.value())) {
			QStringList mod = paramMatch.capturedTexts();
			mod.removeFirst(); // full capture
			Q_ASSERT(mod.length() == 4);
			Q_ASSERT(oldPrefix.compare(mod[1],Qt::CaseInsensitive)==0);
			Q_ASSERT(mod.join("") == cIter.value());
			mod.replace(1,newPrefix);
			cIter.value() = mod.join(QString());
		}
		if (cIter.value().contains(QRegExp(QString("^([^\\s]*;)?%1([\\.;][^\\s]*)?$")
					.arg(oldPrefix),Qt::CaseInsensitive))) {
			qWarning("line still contains old prefix (%s): %s",
					oldPrefix.toLocal8Bit().constData(),
					cIter.value().toLocal8Bit().constData());
			qWarning("it does not match the pattern: %s",
					paramMatch.pattern().toLocal8Bit().constData());
		}
	}
	return true;
}
