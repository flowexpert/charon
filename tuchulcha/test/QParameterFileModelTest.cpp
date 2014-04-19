/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file ParameterFileModeTest.cpp
/// Tests for usage of (qt related) parameter file models.
/// Search for memory allocation bug.

#include "QParameterFileModelTest.h"
#include <QTextStream>
#include "QParameterFile.h"
#include "ParameterFileModel.h"

#include <QtTest/QTest>
#include <QtCore/QDebug>

#ifndef TEST_DIR
#error TEST_DIR not defined
#define TEST_DIR
#endif

void QParameterFileModelTest::initTestCase() {
	QString fileName = TEST_DIR "/sampleclasses.wrp";
	qDebug() << "Filename:" << fileName;

	// content as QParameterFile
	qFile = new QParameterFile(fileName);
	//qDebug() << qFile->getKeyList();

	// same content as ParameterFileModel
	model = new ParameterFileModel;
	model->setHandleDynamics(false);
	model->load(fileName);
}

void QParameterFileModelTest::cleanupTestCase() {
	delete model; model=0;
	delete qFile; qFile=0;
}

void QParameterFileModelTest::checkContent() {
	QStringList parameters = qFile->getKeyList();
	QVERIFY(!parameters.isEmpty());
	QCOMPARE(model->rowCount(), parameters.size());
	for (int i=0; i < parameters.size(); i++) {
		QString val = model->data(model->index(i,0)).toString();
		QCOMPARE(val, parameters[i]);
	}
}

void QParameterFileModelTest::checkPrefix() {
	model->setPrefix("FileReader");
	QStringList parameters = qFile->getKeyList(model->prefix()+".");
	QCOMPARE(model->rowCount(), parameters.size());
	for (int i=0; i < parameters.size(); i++) {
		QString val = model->data(model->index(i,0)).toString();
		QCOMPARE(val, parameters[i].section(".",1));
	}
}

QTEST_MAIN(QParameterFileModelTest)
