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

#include <QObject>
#include <QTextStream>

class ParameterFile;
class QParameterFile;

/// test class for QParameterFile
/** During the tests, the behaviour of the QParameterFile class
 *  is compared to the original ParameterFile implementaion.
 */
class QParameterFileTest: public QObject
{
	Q_OBJECT
private:
	ParameterFile*  pFile; ///< parameter file (charon-core)
	QParameterFile* qFile; ///< parameter file (QParameterFile)

private slots:
	/// setup
	void initTestCase();
	/// check key lists
	void checkKeyLists();
	/// check content
	void checkValues();
	/// check some values explicitly
	void checkExplicit();
	/// check special characters
	void checkUnicode();
	/// finalize
	void cleanupTestCase();
};
