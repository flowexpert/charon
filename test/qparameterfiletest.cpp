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

#ifndef QPARAMETERTESTFILE
#error QPARAMETERTESTFILE not defined
#endif

/// Starting point of test application.
int main() {
	QString fileName = QPARAMETERTESTFILE;
	QFile file(fileName);

	// open file with qt methods
	std::cout << "Opening File...\n";
	Q_ASSERT(file.open(QFile::ReadOnly));
	std::cout << "  OK!\n";

	QTextStream input(&file);

	// open file with QParameterFile
	QParameterFile paramFile(fileName);
	QStringList keyList = paramFile.getKeyList();

	std::cout << "Checking if KeyList contains values...\n";
	Q_ASSERT(!keyList.isEmpty());
	std::cout << "  OK!\n";

	QString line;
	while (!(line = input.readLine()).isNull()) {
		QString key = line.left(line.indexOf("\t\t"));
		QString value = line.mid(line.indexOf("\t\t") + 2);

		std::cout << "Looking for key '" << key.toStdString()
			<< "' in KeyList...\n";
		Q_ASSERT(keyList.contains(key));
		std::cout << "  OK!\n";

		std::cout << "Looking for key '" << key.toStdString()
			<< "' in QParameterFile...\n";
		Q_ASSERT(paramFile.isSet(key));
		std::cout << "  OK!\n";

		std::cout << "Checking value '" << value.toStdString() << "'...\n";
		Q_ASSERT(paramFile.get(key) == value);
		std::cout << "  OK!\n";
	}

	std::cout << "\n>> Reading was successful! <<\n\n";

	std::cout << "Saving a copy...\n";
	paramFile.save(fileName + ".copy");
	std::cout << "  OK!\n";

	std::cout << "Loading copy...\n";
	QParameterFile paramFile2(fileName + "~");
	std::cout << "  OK!\n";

	QStringList keyList2 = paramFile2.getKeyList();
	std::cout << "Checking if KeyList contains values...\n";
	Q_ASSERT(!keyList2.isEmpty());
	std::cout << "  OK!\n";

	for (int ii = 0; ii < keyList.size(); ii++) {
		QString key = keyList.at(ii);
		QString value = paramFile.get(key);

		std::cout << "Looking for key '" << key.toStdString()
			<< "' in KeyList...\n";
		Q_ASSERT(keyList2.contains(key));
		std::cout << "  OK!\n";

		std::cout << "Looking for key '" << key.toStdString()
			<< "' in QParameterFile...\n";
		Q_ASSERT(paramFile2.isSet(key));
		std::cout << "  OK!\n";

		std::cout << "Checking value '" << value.toStdString() << "'...\n";
		Q_ASSERT(paramFile2.get(key) == value);
		std::cout << "  OK!\n";
	}

	std::cout << "\n>> Writing was successful! <<\n\n";

	std::cout << "\n################\nTest successful!\n################\n";
	return 0;
}
