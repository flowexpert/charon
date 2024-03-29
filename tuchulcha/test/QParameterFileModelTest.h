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
/// @file ParameterFileModeTest.h
/// Tests for usage of (qt related) parameter file models.
/// Search for memory allocation bug.

#include <QtCore/QObject>
class QParameterFile;
class ParameterFileModel;

/// test class for ParameterFileModel
/** Check model content and compare it to the raw parameter file content. */
class QParameterFileModelTest: public QObject
{
	Q_OBJECT
private:
	QParameterFile*     qFile; ///< parameter file (QParameterFile)
	ParameterFileModel* model; ///< parameter file model

private slots:
	/// setup
	void initTestCase();
	/// check content
	void checkContent();
	/// check prefix handling
	void checkPrefix();
	/// finalize
	void cleanupTestCase();
};
