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
	Q_PROPERTY(int _minPriority     READ minPriority    WRITE setMinPriority)

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

	virtual Qt::DropActions supportedDropActions() const;

	virtual QStringList mimeTypes() const;

	virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action,
		int row, int column, const QModelIndex &parent);

	// \} // model/view interface

	/// Get property _fileName
	QString fileName() const;

	/// Get property _prefix
	QString prefix() const;

	/// Get property _useMetadata;
	bool useMetaInfo() const;

	/// Get property _onlyparams.
	bool onlyParams() const;

	/// Get property _minPriority
	int minPriority() const;

	/// Get const pointer of ParameterFile
	const QParameterFile& parameterFile() const;

	/// Check prefix
	bool prefixValid() const;

	/// \name Metadata Queries with dynamic Information
	// \{
	/// check for available metaInfos
	bool metaInfoValid() const;

	/// get classes contained in metadata file
	QStringList getClasses() const;

	/// get class of some given object
	/** \param  objName         name of the object
	 *  \returns                empty string if class is unknown
	 */
	QString getClass(QString objName) const;

	/// Get type of some parameter or slot.
	/** This also handles presence of some template parameters.
	 *  \param parName          name of the parameter
	 *  \param applyTmplType    replace template parameter by selected type
	 *  \returns                parameter/slot type
	 */
	QString getType(QString parName, bool applyTmplType = true) const;

	/// pass to metaInfo, use dynamic metadata if needed
	QString getDefault(QString parName) const;

	/// Get input slots of object
	/** \param  objName         name of the object
	 *  \returns                name of input slots
	 */
	QStringList getInputs(QString objName) const;

	/// Get output slots of object
	/** \param  objName         name of the object
	 *  \returns                name of output slots
	 */
	QStringList getOutputs(QString objName) const;

	/// Get parameters of object
	/** \param  objName         name of the object
	 *  \returns                name of parameters
	 */
	QStringList getParameters(QString objName) const;

	/// Check if some parameter/slot is a parameter.
	/** \param  name            name of the parameter/slot
	 *  \retval true            given object is a parameter
	 */
	bool isParameter(QString name) const;

	/// Check if some parameter/slot is an input slot.
	/** \param  name            name of the slot
	 *  \retval true            given object is an input slot
	 */
	bool isInputSlot(QString name) const;

	/// Check if some parameter/slot is an output slot.
	/** \param  name            name of the slot
	 *  \retval true            given object is an output slot
	 */
	bool isOutputSlot(QString name) const;

	/// Check if some slot is a multi slot.
	/** \param  name            name of the slot
	 *  \retval true            slot may have multiple targets
	 */
	bool isMultiSlot(QString name) const;
	// \}

	/// \name QParameterFile wrapper functions
	// \{
	/// Get a parameter from the underlying parameter file
	/** \param parName          name of the parameter
	 *  \returns                QParameterFile parameter value
	 */
	QString getValue(QString parName) const;

	/// Set a parameter in the underlying parameter file
	/** \param parName          name of the parameter
	 *  \param value            new value of the parameter
	 */
	void setValue(QString parName, QString value);

	/// pass to QParameterFile::isSet
	bool isSet(QString parName) const;

	///	Delete a parameter from the the underlying parameter file
	/**	\param parName          name of the parameter
	 */
	void erase(QString parName);
	// \}

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

	/// Property _onlyParams changed
	/** \param  value           new value */
	void onlyParamsChanged(bool value);

	/// Property _minPriority changed
	/** \param value            new value */
	void minPriorityChanged(int value);

	/// send status message
	/** \param  msg             message */
	void statusMessage(const QString& msg);

	/// inform about modifications
	void modified(bool val /** new modification status*/);

	/// inform about editor comment change
	void commentChanged(QString comment);

	/// inform about dynamic plugin update
	void dynamicUpdate();

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

	/// Set property _minPriority
	/** \param  value           new value to set */
	void setMinPriority(int value);

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
	virtual void save(const QString& fileName = "");

protected slots:
	/// Load data without showing OpenFile Dialog.
	/** \retval false           error during load */
	virtual bool _load();

	/// Update content (e.g. when prefix has changed)
	virtual void _update();

	/// Update filter if priority changed
	virtual void _updatePriority(const QModelIndex &topLeft,
		const QModelIndex &bottomRight);

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

	/// minimum priority to be listed
	int _minPriority;

	/// update dynamic objects
	void _updateDynamics();

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
	 *  \returns                filtered list (no *.editorpriority keys)
	 */
	QStringList _priorityTagFilter(QStringList list) const;

	/// filter keys depending on their priority
	/** \param list             list to filter
	 *  \returns                filtered list
	 */
	QStringList _priorityFilter(QStringList list) const;

	/// creates path to dynamic meta info file for object
	QString _getDynamicMetaFile(QString objName) const;
};

#endif /* PARAMETERFILEMODEL_H_ */
