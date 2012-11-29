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
/** \file   MetaData.h
 *  \brief  Implementation of class ParameterFileModel.
 *  \date   10.10.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef METADATA_H_
#define METADATA_H_

#include <QStringList>

class QParameterFile;

/// Handle metadata management for ParameterFileModel classes.
class MetaData {
public:
	/// Load metadata from given file
	MetaData(QString fileName);
	~MetaData();

	/// \name Data queries
	//  \{
	/// get classes contained in this metadata file
	QStringList getClasses() const;

	/// get input slots of some given object
	/** \param className        object class name
	 *                          (use ParameterFileModel::getClass to get this)
	 *  \returns                list of input slots
	 */
	QStringList getOutputs(QString className) const;

	/// get input slots of some given object
	/** \param className        object class name
	 *                          (use ParameterFileModel::getClass to get this)
	 *  \returns                list of input slots
	 */
	QStringList getOutputDisplayNames(QString className) const;

	/// get output slots of some given object
	/** \param className        object class name
	 *                          (use ParameterFileModel::getClass to get this)
	 *  \returns                list of output slots
	 */
	QStringList getInputs(QString className) const;

	/// get output slots of some given object
	/** \param className        object class name
	 *                          (use ParameterFileModel::getClass to get this)
	 *  \returns                list of output slots
	 */
	QStringList getInputDisplayNames(QString className) const;

	/// get parameters of some given object
	/** \param className        object class name
	 *                          (use ParameterFileModel::getClass to get this)
	 *  \returns                list of parameters
	 */
	QStringList getParameters(QString className) const;

	/// Get type of some parameter or slot.
	/** Use GraphModel::getType if there are template arguments and you need to
	 *  know the type in the specific instance.
	 *  \param parName          name of the parameter
	 *  \param className        object class name
	 *                          (use ParameterFileModel::getClass to get this)
	 *  \returns                slot type
	 */
	QString getType(QString parName, QString className) const;

	/// get default value for some editable parameter
	/** \param parName          name of the parameter
	 *  \param className        object class name
	 *  \returns                empty string, if no default given
	 */
	QString getDefault(QString parName, QString className) const;

	/// get docstring for some parameter or some class.
	/** \param parName          name of the parameter
	 *  \param className        object class name
	 *  \returns                empty string, if no docstring given
	 */
	QString getDocString(QString parName, QString className) const;

	/// get filename for some parameter or class documentation.
	/** \param parName          name of the parameter
	 *  \param className        object class name
	 *  \returns                empty string, if no docfile given
	 */
	QString getDocFile(QString parName, QString className) const;

	/// Check if some parameter/slot is an input slot.
	/** \param  name            name of the slot
	 *  \param  className       object class name
	 *  \retval true            given object is an input slot
	 */
	bool isInputSlot(QString name, QString className) const;

	/// Check if some slot is optional.
	/** \param  slotName        name of the slot
	 *  \param  className       object class name
	 *  \throws std::string     error message if slot is invalid
	 *                          (i.e. does not exists, etc.)
	 *  \retval true            slot is optional
	 */
	bool isOptionalSlot(QString slotName, QString className) const;

	/// Check if some slot is a multi slot.
	/** \param  slotName        name of the slot
	 *  \param  className       object class name
	 *  \throws std::string     error message if slot is invalid
	 *                          (i.e. does not exists, etc.)
	 *  \retval true            slot may have multiple targets
	 */
	bool isMultiSlot(QString slotName, QString className) const;

	/// Check if module is dynamic
	/**
	 *  \param  className       object class name
	 *  \retval true            module is dynamic
	 */
	bool isDynamic(QString className) const;
	//  \}  // queries

private:
	/// contained classes
	QStringList _classes;

	/// used ParameterFile (read only)
	const QParameterFile* _data;
};

#endif /*METADATA_H_*/
