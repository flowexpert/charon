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

#include <QStringList>
#include <QHash>

/// Qt implementation for ParameterFile class
/** Hides unneeded complexity and provides QString getter and setter.
 *  New implementation based on Qt file-i/o and QTextStream
 *  to handle files with arbitrary encodings.
 *  Also supports keys with mixed case values.
 *  Parameter search is still done case-insensitive.
 */
class QParameterFile {
public:
	/// default constructor
	QParameterFile(QString fileName = "" /** [in] file to load */);

	/// load parameter file
	/** The file is interpreted as local 8-bit encoded.
	 *  If specified, the given encoding is selected
	 *  \param fileName   path of file to read
	 *  \param encoding   enfoce given encoding
	 */
	void load(QString fileName, QString encoding=QString());
	/// save to given plain text file
	void save(QString fileName) const;
	/// get StringList representation, like file content after save()
	QStringList toStringList() const;

	/// check if a given parameter has been set
	bool isSet(QString parameter) const;

	/** \name setters and getters
	 *  If parameter is unset, the getters return a null string.
	 */
	// \{
	/// set parameter value
	/** \param parameter    parameter (key), will be added if unset
	 *  \param value        value to set
	 */
	void set(QString parameter, QString value = "");
	/// get parameter value
	/** \param parameter    parameter name (key)
	 *  \returns            null string if unset */
	QString get(QString parameter) const;

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

	/// rename prefix
	/** renames all keys starting with oldPrefix to ones with the given
	 *  new prefix and also renames occurrences of the oldPrefix in
	 *  values of other parameters (like slot connections)
	 *
	 *  The new prefix must match the rules for parameter file prefixes
	 *  (as specified in QParameterFile::prefixCheck ).
	 *  The old prefix is not checked, if there are dots, the part
	 *  before the dots is used. In the worst case, there are no occurrences
	 *  of an invalid keys which causes the function to return and skip
	 *  the parameter renaming.
	 *
	 *  \param oldPrefix    old prefix
	 *  \param newPrefix    new prefix (checks for valid prefix)
	 *  \retval true        renaming successful
	 *  \retval false       some error occured, nothing changed at all
	 */
	bool rename(QString oldPrefix, QString newPrefix);

	/// instance name or prefix check regex
	static const QRegExp prefixCheck;
	/// postfix check, everything that may follow a prefix (starting with ".")
	static const QRegExp postfixCheck;
	/// full parameter name check
	static const QRegExp paramCheck;
	/// full parameter name check (sloppy version, may handle buggy lines)
	static const QRegExp paramCheckSloppy;

private:
	/// key array, stored in mixed case
	QStringList _keys;
	/// value hash table, keys are stored in lowercase
	QHash<QString,QString> _content;
};

#endif // QPARAMETERFILE_H
