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
/** @file   ParameterFileModel.cpp
 *  @brief  Implementation of class ParameterFileModel.
 *  @date   12.08.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "ParameterFileModel.h"
#include "QParameterFile.h"
#include "PrefixValidator.h"
#include "VarTypeMap.h"
#include "MetaData.h"
#include <QFileDialog>
#include <QDir>
#include <QSet>
#include <QSettings>
#include <QMessageBox>
#include <QIcon>
#include <QMimeData>
#include <QUrl>

#include "ParameterFileModel.moc"

ParameterFileModel::ParameterFileModel(
			QString fName, QObject* myParent, QString metaFile) :
		QAbstractTableModel(myParent),
		_parameterFile(new QParameterFile()),
		_fileName(fName),
		_prefix(""),
		_metaInfos(0),
		_useMetaInfo(false),
		_onlyParams(false),
		_minPriority(0)
{
	// this ParameterFile stores the description of classes.
	if (!metaFile.isEmpty()) {
		loadMetaInfo(metaFile);
		setOnlyParams(true);
	}
	if (!_fileName.isEmpty())
		_load();

	connect(this, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
		SLOT(_updatePriority(QModelIndex,QModelIndex)));
}

ParameterFileModel::~ParameterFileModel() {
	if (_metaInfos) {
		delete _metaInfos;
		_metaInfos = 0;
	}
	delete _parameterFile;
}

int ParameterFileModel::rowCount(const QModelIndex& /*parent*/) const {
	return _keys.size();
}

int ParameterFileModel::columnCount(const QModelIndex& /*parent*/) const {
	return 3;
}

QVariant ParameterFileModel::data(const QModelIndex& ind, int role) const {
	// mapper to convert parameter.type into QVariant::Type
	const VarTypeMap& mapper = VarTypeMap::instance();

	if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
		int row = ind.row();

		if ((row >= 0) && (row < _keys.size())) {
			QString name;

			switch (ind.column()) {
			case 0:
				// parameter name (without prefix)
				name = _keys[row];
				if (!_prefix.isEmpty())
					name.remove(0, _prefix.length());

				// remove dot after valid prefix
				if (name[0] == '.')
					name.remove(0, 1);
				return name;

			case 1:
				{
					QVariant res = "";
					QString className = getClass(_keys[row]);
					if (_parameterFile->isSet(_keys[row]))
						res = _parameterFile->get(_keys[row]);
					else if (_onlyParams)
						res = metaInfo()->getDefault(_keys[row], className);

					if (_useMetaInfo && metaInfo()->isParameter(_keys[row],
							className)) {
						QString typestring = getType(_keys[row]);
						QVariant::Type type = mapper[typestring];
						Q_ASSERT(res.canConvert(type));
						res.convert(type);
					}
					// simply return the string value
					return res;
				}

			case 2:
				if (role == Qt::DisplayRole)
					return QVariant();
				if (_parameterFile->isSet(_keys[row] + ".editorpriority")) {
					return _parameterFile->get(_keys[row] + ".editorpriority").toUInt();
				}
				else {
					return 0u;
				}
			}
		}
	}
	if ((role == Qt::ToolTipRole) && _useMetaInfo) {
		QString ret = metaInfo()->getDocString(_keys[ind.row()], getClass(
				_keys[ind.row()]));
		if (!ret.isEmpty())
			return ret;
	}

	if (role == Qt::ForegroundRole && _onlyParams
		&& !_parameterFile->isSet(_keys[ind.row()]))
			return Qt::lightGray;

	if (role == Qt::BackgroundRole && _parameterFile->isSet(
		_keys[ind.row()] + ".editorpriority")) {

		switch (_parameterFile->get(_keys[ind.row()] + ".editorpriority").toInt()) {
		case 1:
			return QColor("#8f8");
		case 2:
			return QColor("#ff8");
		case 3:
			return QColor("#f80");
		default:
			break;
		}
	}
	if (ind.column() == 1 && role == Qt::CheckStateRole && _useMetaInfo
			&& metaInfo()->isParameter(_keys[ind.row()],
					getClass(_keys[ind.row()]))) {
		QString typestring = getType(_keys[ind.row()]);
		if (typestring == "bool") {
			if(_parameterFile->isSet(_keys[ind.row()])) {
				if (QVariant(_parameterFile->get(
						_keys[ind.row()])).toBool())
					return Qt::Checked;
				else
					return Qt::Unchecked;
			}
			else {
				QVariant defVal = metaInfo()->getDefault(
						_keys[ind.row()], getClass(_keys[ind.row()]));
				if (defVal.toBool())
					return Qt::Checked;
				else
					return Qt::Unchecked;
			}
		}
	}
	return QVariant();
}

bool ParameterFileModel::setData(
		const QModelIndex& ind, const QVariant& value, int role) {

	if (!prefixValid())
		return false;

	if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
		if ((ind.row() >= 0) && (ind.row() < _keys.size())) {
			switch (ind.column()) {
			case 0:
				if (_onlyParams)
					return false;

				if (value.canConvert(QVariant::String)) {
					QString& oldName = _keys[ind.row()];
					QString newName;
					// do not forget the prefix at the beginning of the name
					if (!_prefix.isEmpty())
						newName = _prefix + ".";
					newName += value.toString();
					if (oldName == newName)
						return true; // nothing to do
					if (_parameterFile->isSet(newName))
						return false; // don't overwrite existing value

					// save value
					QString val = _parameterFile->get(oldName);
					_parameterFile->erase(oldName);
					_parameterFile->set(newName, val);
					oldName = newName; // save new name in name cache
					emit dataChanged(ind, ind);
					return true;
				}
				break;
			case 1:
				if (value.canConvert(QVariant::String)) {
					QString valueStr = value.toString();
					if (valueStr == _parameterFile->get(_keys[ind.row()]))
						return true; // nothing to do

					_parameterFile->set(_keys[ind.row()], valueStr);
					emit dataChanged(index(ind.row(), 0), ind);
					return true;
				}
				break;
			case 2:
				if (value.canConvert(QVariant::Int)) {
					// check if value is allowed
					int valueInt = value.toInt();
					QString valueStr = QVariant(valueInt).toString();
					if (valueInt < 0 || valueInt > 3) {
						return false;
					}

					if (valueInt == 0) {
						// 0 is default value -> no entry needed
						if (_parameterFile->isSet(
							_keys[ind.row()] + ".editorpriority")) {
							// check if value exists to prevent exceptions
							_parameterFile->erase(
								_keys[ind.row()] + ".editorpriority");
							emit dataChanged(index(ind.row(), 0), ind);
						}
						return true;
					}

					if (valueStr == _parameterFile->get(
						_keys[ind.row()] + ".editorpriority"))
							return true; // nothing to do

					_parameterFile->set(
						_keys[ind.row()] + ".editorpriority", valueStr);
					emit dataChanged(index(ind.row(), 0), ind);
					return true;
				}
				break;
			}
		}
	}
	if (ind.column() == 1 && role == Qt::CheckStateRole && _useMetaInfo
				&& metaInfo()->isParameter(_keys[ind.row()],
						getClass(_keys[ind.row()]))) {
		Q_ASSERT(getType(_keys[ind.row()]) == "bool");
		Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt()) ;
		if(state == Qt::Checked || state == Qt::PartiallyChecked)
			_parameterFile->set(_keys[ind.row()],"1");
		if(state == Qt::Unchecked)
			_parameterFile->set(_keys[ind.row()],"0");
		emit dataChanged(createIndex(ind.row(),0),ind);
	}
	return false;
}

Qt::ItemFlags ParameterFileModel::flags(const QModelIndex& ind) const {
	if (!prefixValid())
		return 0;
	if ((ind.row() >= 0) && (ind.row() <= _keys.size())) {
		switch (ind.column()) {
		case 0:
			if (_onlyParams)
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		case 1: {
			QString paramType = getType(_keys[ind.row()]);
			QVariant::Type dataType = data(ind).type();
			if(_useMetaInfo && metaInfo()->isParameter(_keys[ind.row()],
				getClass(_keys[ind.row()])) && paramType == "bool")

				return Qt::ItemIsSelectable | Qt::ItemIsEnabled
					| Qt::ItemIsUserCheckable;

			if (dataType == QVariant::String
				&& !paramType.contains(QRegExp("^\\{\\s*\\w.*\\}\\s*$"))) {

				return Qt::ItemIsSelectable | Qt::ItemIsEnabled
					| Qt::ItemIsEditable | Qt::ItemIsDropEnabled;

			}
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled
				| Qt::ItemIsEditable;
			}
		case 2:
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled
					| Qt::ItemIsEditable;
		default:
			return 0;
		}
	}
	return 0;
}

QVariant ParameterFileModel::headerData(int section,
		Qt::Orientation orientation, int role) const {

	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			switch (section) {
			case 0:
				return "Parameter";
			case 1:
				return "Value";
			case 2:
				return "Priority";
			}
		} else {
			if ((section >= 0) && (section < _keys.size()))
				return section;
		}
	}
	return QVariant();
}

bool ParameterFileModel::insertRows(int row, int count,
		const QModelIndex& parentInd) {

	// only appending is allowed!
	if (!_onlyParams && prefixValid()
			&& (row >= 0) && (row == _keys.size()) && (count > 0)) {
		beginInsertRows(parentInd, row, row + count - 1);
		int offset = 0;
		int i = 1;

		// append new values
		while (i <= count) {
			// get temp name for new parameter
			QString name;
			if (!_prefix.isEmpty())
				name += _prefix + ".";
			name += QString("new%1").arg(i + offset);
			if (_parameterFile->isSet(name)) {
				offset++;
				continue;
			}
			_parameterFile->set(name, "");
			if (_minPriority > 0) {
				_parameterFile->set(name + ".editorpriority",
					QVariant(_minPriority).toString());
			}
			i++;
		}
		_keys = _parameterFile->getKeyList(_prefix);

		// apply modificators
		if (!_prefix.isEmpty())
			_keys = _prefixFilter(_keys);
		_keys = _priorityTagFilter(_keys);
		_keys = _priorityFilter(_keys);

		// check new number of elements
		Q_ASSERT(_keys.size() == (row + count));
		endInsertRows();
		return true;
	}
	else
		return false;
}

bool ParameterFileModel::removeRows(int row, int count,
		const QModelIndex& parentInd) {

	// forbid modifications on invalid prefix
	if (!prefixValid())
		return false;

	// check row and count dimensions (have to fit into table)
	if ((count > 0) && (row >= 0) && ((row + count) <= _keys.size())) {
		// when onlyparams is set, values are not really removed, but
		// reset to their default values
		if (!_onlyParams)
			beginRemoveRows(parentInd, row, row + count - 1);

		for (int i = row; i < row + count; i++)
			if (_parameterFile->isSet(_keys[i]))
				_parameterFile->erase(_keys[i]);

		// keys have not to be changed on onlyparam mode
		if (!_onlyParams) {
			_keys = _parameterFile->getKeyList(_prefix);
			endRemoveRows();
			return true;
		} else {
			emit dataChanged(index(row, 0), index(row + count, 1));
		}
	}
	return false;
}

void ParameterFileModel::clear() {
	setPrefix("");
	uint keys = rowCount(QModelIndex());
	if (keys > 0) {
		//beginRemoveRows(QModelIndex(), 0, keys - 1);
		beginResetModel();
		_parameterFile->clear();
		_keys.clear();
		setPrefix("");
		//endRemoveRows();
		endResetModel();
	}
}

bool ParameterFileModel::_load() {
	clear();
	_parameterFile->load(_fileName);
	_update();
	emit statusMessage(QString("File %1 loaded.").arg(_fileName));
	return true;
}

void ParameterFileModel::_update() {
	// remove keys
	//uint keys = _keys.size();
	beginResetModel();
	//if (keys > 0) {
		//beginRemoveRows(QModelIndex(), 0, keys - 1);
	_keys.clear();
		//endRemoveRows();
	//}

	// load all keys
	QStringList tempList = _parameterFile->getKeyList();
	if (tempList.size() == 0)
		return;

	// apply modificators
	if (!_prefix.isEmpty())
		tempList = _prefixFilter(tempList);
	if (_onlyParams)
		tempList = _paramFilter(tempList);
	tempList = _priorityTagFilter(tempList);
	tempList = _priorityFilter(tempList);

	// show selected parameters
	//if (tempList.size()) {
		//beginInsertRows(QModelIndex(), 0, tempList.size() - 1);
	_keys = tempList;
		//endInsertRows();
	//}
	endResetModel();
}

void ParameterFileModel::_updatePriority(const QModelIndex &topLeft,
	const QModelIndex &bottomRight) {
	// check if filter active
	if (_minPriority <= 0) {
		return;
	}

	// check if priority changed
	if (bottomRight.column() != 2) {
		return;
	}

	_update();
}

bool ParameterFileModel::load(const QString& fName) {
	// determine which file to load
	// (file dialog if fName is empty)
	QString fromDialog = fName;
	if (fromDialog.isEmpty()) {
		QString guess = _fileName;
		if (guess.isEmpty()) {
			QSettings settings;
			QStringList files =
					settings.value("recentFileList").toStringList();
			if (files.size() > 0)
				guess = files[0];
			else
				guess = QDir::homePath();
		}
		fromDialog = QFileDialog::getOpenFileName(0, tr("Open File"),
				guess, tr("ParameterFile (*.*)"));
	}
	if (fromDialog.isEmpty())
		return false;
	if (!QFileInfo(fromDialog).isFile()) {
		QMessageBox::warning(0, tr("Error loading file"),
				tr("File <em>%1</em> does not exist or is no file!")
						.arg(fromDialog));
		return false;
	}
	if (!QFileInfo(fromDialog).isReadable()) {
		QMessageBox::warning(0, tr("Error loading file"),
				tr("File <em>%1</em> is not readable!").arg(fromDialog));
		return false;
	}

	// fromDialog is a readable file now
	setFileName(fromDialog);
	return _load();
}

void ParameterFileModel::save(const QString& fName) {
	QString name;
	//default to old filename if defined
	if ((!_fileName.isEmpty()) && (_fileName != QDir::homePath()))
		name = _fileName;
	//overwrite with new filename if provided
	if (!fName.isEmpty())
		name = fName;
	if (!name.isEmpty()) {
		setFileName(name);
		_parameterFile->save(name);
		emit statusMessage(QString("File %1 saved.").arg(name));
		emit modified(false);
	} else
		emit statusMessage("File not saved! (no filename given)");
}
void ParameterFileModel::setFileName(const QString& fName) {
	if (fName.isEmpty() || fName == _fileName)
		return;
	_fileName = fName;
	emit fileNameChanged(_fileName);
}

QString ParameterFileModel::setPrefix(const QString& newPrefix) {
	if (newPrefix == _prefix)
		return _prefix;
	QString old = _prefix;
	_prefix = newPrefix;
	_update();
	emit prefixChanged(_prefix);
	return old;
}

const QParameterFile& ParameterFileModel::parameterFile() const {
	return *_parameterFile;
}

QString ParameterFileModel::fileName() const {
	return _fileName;
}

QString ParameterFileModel::prefix() const {
	return _prefix;
}

bool ParameterFileModel::prefixValid() const {
	return _prefix.isEmpty() ? true :
		(_parameterFile->getKeyList(_prefix+".").size() > 0);
}

void ParameterFileModel::loadMetaInfo(const QString& fName) {
	if (_metaInfos) {
		setUseMetaInfo(false);
		delete _metaInfos;
		_metaInfos = 0;
	}
	if (fName.isEmpty()) {
		_metaInfos = 0;
		emit metaInfoChanged(false);
	} else {
		_metaInfos = new MetaData(fName);
		emit metaInfoChanged(true);
	}
}

bool ParameterFileModel::useMetaInfo() const {
	return _useMetaInfo;
}

void ParameterFileModel::setUseMetaInfo(bool value) {
	if (value == _useMetaInfo)
		return;
	if ((!value) && _onlyParams)
		setOnlyParams(false);
	_useMetaInfo = value;
	reset();
	emit useMetaInfoChanged(value);
}

const MetaData* ParameterFileModel::metaInfo() const {
	return _metaInfos;
}

bool ParameterFileModel::onlyParams() const {
	return _onlyParams;
}

int ParameterFileModel::minPriority() const {
	return _minPriority;
}

void ParameterFileModel::setOnlyParams(bool on) {
	if (_onlyParams == on)
		return;
	if ((!_useMetaInfo) && on)
		setUseMetaInfo(on);
	_onlyParams = on;
	_update();
	emit onlyParamsChanged(_onlyParams);
}

QString ParameterFileModel::getClass(QString name) const {
	name = name.section(".",0,0);
	return _parameterFile->get(name + ".type");
}

QStringList ParameterFileModel::_prefixFilter(QStringList list) const {
	if(prefixValid()) {
		return list.filter(QRegExp("^\\s*"+_prefix+"\\."));
	}
	else {
		return list.filter(QRegExp("^\\s*"+_prefix));
	}
}

QStringList ParameterFileModel::_collectObjects(QStringList list) const {
	QSet<QString> result;
	for (int ii = 0; ii < list.size(); ii++) {
		QString cur = list[ii].section(".",0,0);
		result << cur;
	}
	return result.toList();
}

QStringList ParameterFileModel::_paramFilter(QStringList list) const {
	Q_ASSERT(_onlyParams);
	QStringList result;
	QStringList objects = _collectObjects(list);

	// append all possible parameters for all objects
	for (int ii = 0; ii < objects.size(); ii++) {
		QStringList tmp;
		tmp << metaInfo()->getParameters(getClass(objects[ii]));
		tmp.replaceInStrings(QRegExp("(^.*$)"), objects[ii]+".\\1");
		result << tmp;
	}
	return result;
}

QStringList ParameterFileModel::_priorityTagFilter(QStringList list) const {
	QStringList result;
	while (!list.empty()) {
		QString str = list.first();
		if (!str.endsWith(".editorpriority")) {
			result << str;
		}
		list.pop_front();
	}
	return result;
}

QStringList ParameterFileModel::_priorityFilter(QStringList list) const {
	if (_minPriority <= 0) {
		return list;
	}
	QStringList result;
	while (!list.empty()) {
		QString str = list.first();
		if (_parameterFile->get(str + ".editorpriority").toInt() >= _minPriority) {
			result << str;
		}
		list.pop_front();
	}
	return result;
}

QString ParameterFileModel::getType(QString parName) const {
	if(!_useMetaInfo)
		return "";
	QString res = metaInfo()->getType(parName, getClass(parName));
	if (res.contains(
			QRegExp("^\\s*(.*<\\s*T\\s*>.*|T)\\s*$",Qt::CaseInsensitive))) {
		parName = parName.section(".",0,0);
		QString tType = parameterFile().get(parName + ".templatetype");
		if(tType.isEmpty()) {
			tType = metaInfo()->getDefault("templatetype", getClass(parName));
		}
		res.replace(
				QRegExp("<\\s*T\\s*>",Qt::CaseInsensitive),
				QString("<%1>").arg(tType));
		res.replace(
				QRegExp("^\\s*T\\s*$",Qt::CaseInsensitive),tType);
	}
	return res;
}

void ParameterFileModel::setEditorComment(QString comment) {
	if (_prefix.isEmpty()) {
		return;
	}

	bool valSet = false;
	bool paramsBefore = _onlyParams;
	setOnlyParams(false);

	// check if "editorcomment" entry already exists
	for (int i = 0; i < rowCount(); i++) {
		QString str = data(index(i,0)).toString();
		if (data(index(i,0)).toString() == "editorcomment") {
			QString str = comment;
			// delete if empty
			if (str.isEmpty()) {
				removeRow(i);
			} else {
				QString oldData = data(index(i, 1)).toString();
				if (oldData == comment) {
					setOnlyParams(paramsBefore);
					return;
				}
				setData(index(i,1), comment);
			}
			valSet = true;
			break;
		}
	}

	// if not, insert row
	if (!valSet && !comment.isEmpty()) {
		insertRow(rowCount());
		setData(index(rowCount()-1,0), "editorcomment");
		setData(index(rowCount()-1,1), comment);
	}

	setOnlyParams(paramsBefore);

	// update selected node
	emit commentChanged(comment);
}

void ParameterFileModel::setMinPriority(int value) {
	if (value < 0 || value > 3) {
		return;
	}

	_minPriority = value;
	_update();

	emit minPriorityChanged(value);
}

Qt::DropActions ParameterFileModel::supportedDropActions() const {
	return Qt::CopyAction | Qt::MoveAction;
}

QStringList ParameterFileModel::mimeTypes() const {
	QStringList list;
	list << "text/plain";
	list << "text/uri-list";
	return list;
}

bool ParameterFileModel::dropMimeData(const QMimeData *mimeData,
	Qt::DropAction action, int row, int column, const QModelIndex &parent) {

	// only accept text and files
	QString content;
	if (mimeData->hasUrls()) {
		QList<QUrl> urlList = mimeData->urls();
		if (urlList.size() >= 1) {
			content = urlList.at(0).path().mid(1);
		}
		for (int ii = 1; ii < urlList.size(); ii += 1) {
			content += ";" + urlList.at(ii).path().mid(1);
		}
	} else if (mimeData->hasText()) {
		content = mimeData->text();
	} else {
		return QAbstractTableModel::dropMimeData(mimeData, action,
			row, column, parent);
	}
	
	setData(parent, content);

	return true;
}
