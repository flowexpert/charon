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
/// \file QParameterFileTest.cpp
/// Tests for correct behavior of QParameterFile

#include "QParameterFileTest.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtTest/QTest>
#include "QParameterFile.h"
#include <charon-core/ParameterFile.hxx>

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

void QParameterFileTest::initTestCase() {
	QString fileName = QPARAMETERTESTFILE;
	qDebug() << "File name:" << fileName;
	qFile = new QParameterFile;
	qFile->load(fileName, "latin1");
	pFile = new ParameterFile;
	pFile->load(fileName.toStdString());
}

void QParameterFileTest::cleanupTestCase() {
	delete pFile; pFile = 0;
	delete qFile; qFile = 0;
}

void QParameterFileTest::checkKeyLists() {
	QStringList keyList1 = qFile->getKeyList();
	QStringList keyList2 = fromStdStringList(pFile->getKeyList());

	QCOMPARE(keyList1.size(), keyList2.size());
	for (int ii = 0; ii < keyList1.size(); ii++) {
		QVERIFY2(
			QString::compare(keyList1[ii],keyList2[ii],Qt::CaseInsensitive)==0,
			QString("mismatch in key list entry %1")
				.arg(ii+1).toLocal8Bit().constData());
	}
}

void QParameterFileTest::checkValues() {
	QStringList keyList = qFile->getKeyList(); // key lists are equal
	QCOMPARE(keyList.size(),9);
	foreach (QString key, keyList) {
		QString value1 = qFile->get(key);
		QString value2 = QString::fromStdString(
					pFile->get<std::string>(key.toStdString()));
		if (!key.startsWith("ignore")) {
			QCOMPARE(value1,value2);
		}
	}
}

void QParameterFileTest::checkExplicit() {
	// Some explicit testing
	QCOMPARE(qFile->get("param3"),QString("value3"));
	QCOMPARE(qFile->get("param4"),QString("10"));
	QCOMPARE(qFile->get("paramEmpty"),QString());
}

void QParameterFileTest::checkUnicode() {
	// unicode test string
	const QChar unicode[] = {
		0xc3,0xa4,0xc3,0xb6,0xc3,0xbc,0xc3,0x9f,0xc3,
		0xba,0xc3,0xb4,0xc4,0xa7,0xc5,0x82,0xc4,0x85
	};
	QString ref = QString::fromRawData(unicode,18);
	QString value = qFile->get("ignore1");

	//*
	// unicode content raw output
	QString out;
	QTextStream qout(&out, QIODevice::WriteOnly);
	qout.setIntegerBase(16);
	for (int i=0; i<value.size(); i++) {
		qout << "0x" << (int)QChar(value[i]).unicode() << ",";
	}
	qDebug() << out;
	//*/

	QCOMPARE(ref,value);
}

QTEST_APPLESS_MAIN(QParameterFileTest)
