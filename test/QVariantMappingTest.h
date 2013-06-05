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
/** \file   QVariantMappingTest.h
 *  \brief  Declaration of test class QVariantMappingTest
 *  \date   06.05.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QObject>

/// test class for QVariant Type mapping
/** this Qt built in method could be an alternative for the
 *  self-made VarTypeMap class.
 *  This test shows some issues, e.g. mapping of "float" does not work
 *  as expected.
 */
class QVariantMappingTest: public QObject
{
	Q_OBJECT
private slots:
	/// print variant types (via qDebug)
	void variantTypes();
	/// check mapping as it is done by VarTypeMap
	void variantMapCheck();
};
