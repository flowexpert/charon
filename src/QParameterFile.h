/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file   QParameterFile.h
 *  \brief  Declaration of class QParameterFile
 *  \date   15.03.2011
 *  \author <a href="mailto:jens-malte.gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 */

#ifndef QPARAMETERFILE_H
#define QPARAMETERFILE_H

class ParameterFile;
#include <QStringList>

/// Qt wrapper for ParameterFile class
/** Hides unneeded complexity and provides getter and setter with
 *  Qt compatible data types like QString or QStringList
 */
class QParameterFile {
public:
	/// default constructor
	QParameterFile(QString fileName = "" /** [in] file to load */);
	~QParameterFile();

	/// load parameter file
	void load(QString fileName);
	/// save to given plain text file
	void save(QString fileName) const;

	/// check if a given parameter has been set
	bool isSet(QString parameter) const;

	/** \name setters and getters
	 *  If parameter is unset, the getters return a null string.
	 */
	// \{
	QString get(QString parameter) const;
	void set(QString parameter, QString value);
	QStringList getList(QString parameter) const;
	void set(QString parameter, QStringList value);

	/// Look for parameters beginning with a given string.
	/** \param  beginsWith  Beginning of parameter name
	 *  \returns            matching keys
	 */
	QStringList getKeyList(QString beginsWith = "") const;
	// \}

	/// remove parameter from file
	void erase(QString parameter);

	/// clear content
	void clear();

	/// get wrapped parameter file
	const ParameterFile& toParameterFile() const;

private:
	/// wrapped parameter file instance
	ParameterFile* _pFile;
};

#endif // QPARAMETERFILE_H
