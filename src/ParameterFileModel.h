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
/** \file   ParameterFileModel.h
 *  \brief  Declaration of class ParameterFileModel.
 *  \date   12.08.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef PARAMETERFILEMODEL_H_
#define PARAMETERFILEMODEL_H_

#include <QAbstractTableModel>
#include <QStringList>

class QParameterFile;
class MetaData;
class PrefixValidator;

/// This model serves to provide a model frontend to access a ParameterFile
/// instance. It can be used in a TableView.
class ParameterFileModel: public QAbstractTableModel {
	Q_OBJECT
	Q_PROPERTY(QString _fileName    READ fileName       WRITE setFileName)
	Q_PROPERTY(QString _prefix      READ prefix         WRITE setPrefix)
	Q_PROPERTY(bool _useMetaInfo    READ useMetaInfo    WRITE setUseMetaInfo)
	Q_PROPERTY(bool _onlyParams     READ onlyParams     WRITE setOnlyParams)

public:
	/// Constructor initializing the model with the given file.
	/** \param  parent          parent QObject passed to inherited constructor.
	 *  \param  fileName        filename of the textfile to load.
	 *  \param  metaFile        ParameterFile describing classes
	 *                          and in-/outputs
	 */
	ParameterFileModel(
			QString fileName = "", QObject* parent = 0, QString metaFile = "");

	/// Default destructor.
	virtual ~ParameterFileModel();

	/// \name model/view interface
	//  \{

	/// Return number of table rows.
	/** \param  parent          index of parent item
	 *  \returns                number of rows
	 */
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

	/// Return number of table columns.
	/** \param  parent          index of parent item
	 *  \returns                number of columns (currently always 2)
	 */
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

	/// Access to item content.
	/** Returns data specific to the given role (display, icon, ...)
	 *  \param  index           item index
	 *  \param  role            display role
	 *  \returns                data encapsuled in a QVariant
	 */
	virtual QVariant data(
			const QModelIndex& index, int role = Qt::DisplayRole) const;

	/// Set item content.
	/** Overwrite the current value with the given one.
	 *  \param  index           item index
	 *  \param  value           new value to set
	 *  \param  role            display role
	 *  \retval true            data successfully set
	 *  \retval false           failure setting value
	 */
	virtual bool setData (
			const QModelIndex& index, const QVariant& value,
			int role = Qt::EditRole);

	/// Item flags (editable etc).
	/** \param  index           item index
	 *  \returns                flags of current item
	 */
	virtual Qt::ItemFlags flags (const QModelIndex& index) const;

	/// Data for the table headers.
	/** \param  section         row/column number
	 *  \param  orientation     horizontal or vertical header
	 *  \param  role            display role
	 *  \returns                data encapsuled in a QVariant
	 */
	virtual QVariant headerData (
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const;

	/// Add data to the model.
	/** \param  row             row to prepend the new rows to
	 *  \param  count           number of rows to append
	 *  \param  parent          parent item (here unused)
	 *  \return                 sucess of the operation
	 *  \remark                 Only appending is allowed!
	 */
	virtual bool insertRows (
			int row, int count,
			const QModelIndex& parent = QModelIndex());

	/// Remove data from the model.
	/** \param  row             first row to delete
	 *  \param  count           number of rows to append
	 *  \param  parent          parent item (here unused)
	 *  \return                 sucess of the operation
	 *  \remark                 Only removing the tail is allowed!
	 */
	virtual bool removeRows (
			int row, int count,
			const QModelIndex& parent = QModelIndex());

	// \} // model/view interface

	/// Get property _fileName
	QString fileName() const;

	/// Get property _prefix
	QString prefix() const;

	/// Get property _useMetadata;
	bool useMetaInfo() const;

	/// Get property _onlyparams.
	bool onlyParams() const;

	/// Get const pointer of ParameterFile
	const QParameterFile& parameterFile() const;

	/// Check prefix
	bool prefixValid() const;

	/// Metadata access
	const MetaData* metaInfo() const;

	/// get class of some given object
	/** \param  objName         name of the object
	 *  \returns                empty string if class is unknown
	 */
	QString getClass(QString objName) const;

	/// Get type of some parameter or slot.
	/** This also handles presence of some template parameters.
	 *  \param parName          name of the parameter
	 *  \returns                parameter/slot type
	 */
	QString getType(QString parName) const;

	/// update comment of node
	void setEditorComment(QString string);

signals:
	/// Property _fileName has changed
	/** \param  fileName        new value */
	void fileNameChanged(QString fileName);

	/// Property _prefix has changed
	/** \param  prefix          new value */
	void prefixChanged(const QString& prefix);

	/// New metadata have been loaded.
	/** \param  valid           true, if metadata are valid */
	void metaInfoChanged(bool valid);

	/// Property _useMetadata changed
	/** \param  value           new value */
	void useMetaInfoChanged(bool value);

	/// Property _onlyparams changed
	/** \param  value           new value */
	void onlyParamsChanged(bool value);

	/// send status message
	/** \param  msg             message */
	void statusMessage(const QString& msg);

	/// inform about modifications
	void modified(bool val /** new modification status*/);

	/// inform about editor comment change
	void commentChanged(QString comment);

public slots:
	/// Set new value for Property _fileName
	/** \param  fileName        new value for _fileName */
	virtual void setFileName(const QString& fileName);

	/// Change prefix
	/** \param  prefix          new prefix
	 *  \return                 previous prefix
	 */
	virtual QString setPrefix(const QString& prefix);

	/// load metaFile
	/** \param  fileName        filename to load from */
	virtual void loadMetaInfo(const QString& fileName);

	/// Set property _useMetadata
	/** \param  value           new value to set */
	void setUseMetaInfo(bool value);

	/// Set property _onlyparams
	/** \param  value           new value to set */
	void setOnlyParams(bool value);

	/// Load model content from parameterFile.
	/** Shows OpenFile Dialog and loads the selected file.
	 *  \param fName            file to load, shows OpenFile dialog if empty.
	 *  \retval false           error during load
	 */
	virtual bool load(const QString& fName = "");

	/// Clear ParameterFile content.
	virtual void clear();

	/// Save model content to a text file.
	/** When filename is empty, the old filename will be used (if avaiable).
	 *  When filename is given, the internal filename will be overwritten.
	 *  \param  fileName        name of the file to store
	 */
	virtual void save (const QString& fileName = "");

protected slots:
	/// Load data without showing OpenFile Dialog.
	/** \retval false           error during load */
	virtual bool _load();

	/// Update content (e.g. when prefix has changed)
	virtual void _update();

private:
	/// Pointer to the ParameterFile instance storing the model content
	QParameterFile* _parameterFile;

	/// Cache stored parameter names
	QStringList _keys;

	/// Store filename of the model data file
	QString _fileName;

	/// Parameter prefix
	QString _prefix;

	/// ParameterFile describing classes and in-/output slots
	const MetaData* _metaInfos;

	/// toggle usage of metadata
	bool _useMetaInfo;

	/// only show configurable parameters
	bool _onlyParams;

	/// Return set of objects contained in given parameter list.
	/** \param list             parameter list
	 *  \returns                set of objects (no duplicates)
	 */
	QStringList _collectObjects(QStringList list) const;

	/// filter keys starting with the given prefix
	/** \param list             list to filter
	 *  \returns                filtered list (only with given prefix)
	 */
	QStringList _prefixFilter(QStringList list) const;

	/// filter keys listed in the classes parameter section
	/** \param list             list to filter
	 *  \returns                filtered list (only parameters)
	 */
	QStringList _paramFilter(QStringList list) const;

	/// filter keys containing priority information
	/** \param list             list to filter
	 *  \returns                filtered list (no *.priority keys)
	 */
	QStringList _priorityFilter(QStringList list) const;
};

#endif /* PARAMETERFILEMODEL_H_ */
