/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/** \file   PrefixValidator.h
 *  \brief  Declaration of class PrefixValidator.
 *  \date   26.09.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef PREFIXVALIDATOR_H_
#define PREFIXVALIDATOR_H_

#include <QValidator>
class QParameterFile;

/// Validator for prefix edit widgets.
/**  It serves to check if an entered prefix is valid or not. */
class PrefixValidator : public QValidator {
	Q_OBJECT
public:
	/// default constructor
	/** \param data     input parameter file
	 *  \param parent   parent object
	 */
	PrefixValidator(const QParameterFile& data, QObject* parent = 0);

	/// Validate input string.
	/** This virtual function returns Invalid if input is invalid according to
	 *  this validator's rules, Intermediate if it is likely that a little
	 *  more editing will make the input acceptable (e.g. the user types
	 *  "inp" when "input" is a valid prefix), and Acceptable if the input
	 *  is valid.
	 *  \param input    input string
	 *  \param pos      current editing position
	 *  \returns        validation result
	 */
	virtual QValidator::State validate(QString& input, int& pos) const;

	/// Fix given input string.
	/** This function attempts to change input to be valid according to this
	 *  validator's rules. Here, the first compatible prefix will be returned,
	 *  if there is no such prefix, nothing happens.
	 *  \param input    input string
	 */
	virtual void fixup(QString& input) const;

private:
	const QParameterFile& _parameterFile;	///< parent ParameterFile
};

#endif /*PREFIXVALIDATOR_H_*/
