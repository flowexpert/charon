/*  Copyright (C) 2013 Jens-Malte Gottfried

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
/** \file   QVariantMappingTest.cpp
 *  \brief  Implementation of test class QVariantMappingTest
 *  \date   06.05.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "QVariantMappingTest.h"

#include <QtTest/QtTest>
#include <QVariant>
#include <QDebug>

void QVariantMappingTest::variantTypes() {
	qDebug() << QVariant::typeToName(QVariant::Bool);
	qDebug() << QVariant::typeToName(QVariant::Int);
	qDebug() << QVariant::typeToName(QVariant::UInt);
	qDebug() << QVariant::typeToName(QVariant::Char);
	qDebug() << QVariant::typeToName(QVariant::Double);
	qDebug() << QVariant::typeToName(QVariant::LongLong);
	qDebug() << QVariant::typeToName(QVariant::ULongLong);
	qDebug() << QVariant::typeToName(QVariant::String);
}

void QVariantMappingTest::variantMapCheck() {
	// commented out values do not map to the mentioned QVariant types
	QCOMPARE(QVariant::nameToType("bool"),QVariant::Bool);
	QCOMPARE(QVariant::nameToType("int"),QVariant::Int);
	QCOMPARE(QVariant::nameToType("uint"),QVariant::UInt);
	//QCOMPARE(QVariant::nameToType("char"),QVariant::Char);
	//QCOMPARE(QVariant::nameToType("float"),QVariant::Double); // show stopper
	QCOMPARE(QVariant::nameToType("double"),QVariant::Double);
	//QCOMPARE(QVariant::nameToType("longlong"),QVariant::LongLong);
	//QCOMPARE(QVariant::nameToType("ulonglong"),QVariant::ULongLong);
	//QCOMPARE(QVariant::nameToType("string"),QVariant::String);
}

QTEST_MAIN(QVariantMappingTest)
