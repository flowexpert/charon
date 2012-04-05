/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file qparameterfiletest.cpp
/// Tests for correct behavior of QParameterFile

#include <QFile>
#include <QTextStream>
#include "QParameterFile.h"
#include "ParameterFile.hxx"

#ifndef QPARAMETERTESTFILE
#error QPARAMETERTESTFILE not defined
#endif

QStringList fromStdStringList(const std::vector<std::string>& src) {
	QStringList dst;
	for (std::vector<std::string>::const_iterator iter = src.begin();
			iter != src.end(); iter++) {
		dst << QString::fromStdString(*iter);
	}
	return dst;
}

template <typename T>
void checkEqual(bool& ok, T value1, T value2, QString desc) {
	if (value1 != value2) {
		QTextStream qerr(stderr,QIODevice::WriteOnly);
		qerr << "Error: '" << value1 << "' != '" <<
				value2 << "' (" << desc << ")" << endl;
		ok = false;
	}
}

/// unicode test string
static const QChar unicode[14] = {
	0xc3, 0xa4, 0xc3, 0xb6, 0xc3, 0xbc, 0xc3, 0x9f,
	0xc3, 0xba, 0xc3, 0xb4, 0xc4, 0xa7};

/// Starting point of test application.
int main() {
	QString fileName = QPARAMETERTESTFILE;
	QTextStream qout(stdout,QIODevice::WriteOnly);
	bool success = true;

	QParameterFile qParamFile;
	qParamFile.load(fileName, "latin1");
	ParameterFile paramFile(fileName.toStdString());

	qout << "Loaded File '" << fileName << "'" << endl;

	QStringList keyList1 = qParamFile.getKeyList();
	QStringList keyList2 = fromStdStringList(paramFile.getKeyList());

	qout << "Comparing size of key lists: ";
	qout << keyList1.size() << " - " << keyList2.size() << endl;
	checkEqual(success, keyList1.size(), keyList2.size(), "key list size");

	qout << "\nKey lists:" << endl;
	for (int ii = 0; ii < keyList1.size(); ii++) {
		QString key1 = keyList1[ii];
		QString key2 = keyList2[ii];
		qout << "\t" << ii+1 << ": " << key1 << " - " << key2 << endl;
		checkEqual(
					success,QString::compare(key1, key2, Qt::CaseInsensitive),
					0,QString("key list entry %1").arg(ii+1));
	}

	qout << "\nComparing values:" << endl;
	for (int ii = 0; ii < keyList1.size(); ii++) {
		QString key = keyList1[ii];
		QString value1 = qParamFile.get(key);
		QString value2 = QString::fromStdString(
					paramFile.get<std::string>(key.toStdString()));
		qout << "\t" << key << ": '"
			 << value1 << "' -- '" << value2 << "'" << endl;
		if (!key.startsWith("ignore")) {
			checkEqual(success,value1,value2,QString("value of %1").arg(key));
		}
	}

	// Some explicit testing
	QString param, ref, value;
	param = "param3"; ref = "value3"; value = qParamFile.get(param);
	checkEqual(success,ref,value,param);
	param = "param4"; ref = "10"; value = qParamFile.get(param);
	checkEqual(success,ref,value,param);
	param = "paramEmpty"; ref = ""; value = qParamFile.get(param);
	checkEqual(success,ref,value,param);
	param = "ignore1"; ref = QString::fromRawData(unicode,14);
	value = qParamFile.get(param);
	/*
	// unicode content raw output
	qout.setIntegerBase(16);
	for (int i=0; i<value.size(); i++) {
		qout << "0x" << (int)QChar(value[i]).unicode() << ", ";
	qout << endl;
	}
	*/
	checkEqual(success,ref,value,param);

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
