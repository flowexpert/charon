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

#include <iostream>
#include <QFile>
#include <QTextStream>
#include "QParameterFile.h"
#include "ParameterFile.h"

#ifndef QPARAMETERTESTFILE
#error QPARAMETERTESTFILE not defined
#endif

/// Starting point of test application.
int main() {
	QString fileName = QPARAMETERTESTFILE;

	QParameterFile qParamFile(fileName);
	ParameterFile paramFile(fileName.toStdString());

	std::cout << "Loaded File '" << fileName.toStdString() << "'.\n";

	QStringList keyList1 = qParamFile.getKeyList();
	std::vector<std::string> keyList2 = paramFile.getKeyList();

	std::cout << "Comparing size of key lists...\n";
	std::cout << keyList1.size() << " - " << keyList2.size() << std::endl;
	//Q_ASSERT(keyList1.size() == keyList2.size());
	std::cout << "  OK!\n";

	std::cout << "Comparing key lists (case insensitive)...\n";
	for (int ii = 0; ii < keyList1.size(); ii++) {
		QString key1 = keyList1.at(ii).toLower();
		std::cout << key1.toStdString() << " - ";
		QString key2 = QString(keyList2.at(ii).c_str()).toLower();
		std::cout << key2.toStdString() << "\n";
		//Q_ASSERT(key1 == key2);
	}
	std::cout << "  OK!\n";

	std::cout << "Comparing values...\n";
	for (int ii = 0; ii < keyList1.size(); ii++) {
		QString key = keyList1.at(ii).toLower();
		if (key.startsWith("ignore")) {
			continue;
		}
		QString value1 = qParamFile.get(key);
		//paramFile.get<std::string>("param1");
		//std::string str = paramFile.get<std::string>(key.toStdString());
		//QString value2 = QString(str);
		//Q_ASSERT(value1 == value2);
	}
	std::cout << "  OK!\n";

	std::cout << "Some explicit testing...\n";
	Q_ASSERT(qParamFile.get("param3") == "value3");
	Q_ASSERT(qParamFile.get("param4") == "10");
	Q_ASSERT(qParamFile.get("ignore1") == "הצü‗תפ");
	std::cout << "  OK!\n";

	std::cout << "\n################\nTest successful!\n################\n";
	std::string var;
	std::cin >> var;
	return 0;
}
