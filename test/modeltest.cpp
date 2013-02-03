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
/// @file modeltest.cpp
/// Tests for usage of (qt related) parameter file models.
/// Search for memory allocation bug.

#include <QTextStream>
#include "QParameterFile.h"
#include "ParameterFileModel.h"

#ifndef DEFAULTCLASSFILE
#error DEFAULTCLASSFILE not defined
#endif

/// Starting point of test application.
int main() {
	QString fileName = DEFAULTCLASSFILE;

	// Check if file can be loaded and display content
	QParameterFile file(fileName);
	QTextStream ostrm(stdout);
	ostrm << "Parameter list:\n";
	QStringList parameters = file.getKeyList();

	Q_ASSERT(!parameters.isEmpty());

	// Try to load the same file via a ParameterFileModel
	ParameterFileModel model;
	model.load(fileName);

	Q_ASSERT(model.rowCount() == parameters.size());
	for (int i=0; i < parameters.size(); i++) {
		QString val = model.data(model.index(i,0)).toString();
		ostrm << "\t" << val << "\n";
		Q_ASSERT(val == parameters[i]);
	}

	// check prefix handling
	model.setPrefix(parameters[0].section(".",0,0));
	ostrm << QString("\nWith prefix \"%1\":\n").arg(model.prefix());
	parameters = file.getKeyList(model.prefix()+".");

	Q_ASSERT(model.rowCount() == parameters.size());
	for (int i=0; i < parameters.size(); i++) {
		QString val = model.data(model.index(i,0)).toString();
		ostrm << "\t" << val << "\n";
		Q_ASSERT(val == parameters[i].section(".",1));
	}

	return 0;
}
