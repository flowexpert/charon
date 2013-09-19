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
#include "FileManager.h"
#include "LogDialog.h"
#include "LogDecorators.h"
#include <QFileDialog>
#include <QDir>
#include <QSet>
#include <QSettings>
#include <QMessageBox>
#include <QIcon>
#include <QMimeData>
#include <QUrl>
#include <QTimer>
#include <QApplication>
#include <QMutex>

ParameterFileModel::ParameterFileModel(
			QString fName, QObject* myParent, QString metaFile) :
		QAbstractTableModel(myParent),
		_resetMutex(new QMutex()),
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
	delete _resetMutex;
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
	int row = ind.row();
	int col = ind.column();
	QString key = _keys[row];
	QString val;
	QVariant res;

	switch (role) {

	case Qt::EditRole:
	case Qt::DisplayRole:
		if ((row >= 0) && (row < _keys.size())) {
			switch (col) {
			case 0:
				// parameter name (without prefix)
				if (!_prefix.isEmpty())
					key.remove(0, _prefix.length());

				// remove dot after valid prefix
				if (key[0] == '.')
					key.remove(0, 1);
				return key;

			case 1:
				if (_parameterFile->isSet(_keys[row])) {
					val = _parameterFile->get(_keys[row]);
				}
				else if (_onlyParams) {
					val = getDefault(_keys[row]);
				}

				// handle parameter links
				if (val.startsWith('@')) {
					QString ref = val.mid(1);
					if(_parameterFile->isSet(ref)) {
						val = _parameterFile->get(ref);
					}
					else if(role == Qt::DisplayRole) {
						val = tr("[invalid reference to %1]").arg(ref);
					}
				}

				// handle QVariant type
				res = val;
				if (_useMetaInfo && isParameter(key)) {
					QString typestring = getType(key);
					QVariant::Type type = mapper[typestring];
					Q_ASSERT(res.canConvert(type));
					res.convert(type);
				}
				return res;

			case 2:
				if (role == Qt::DisplayRole) {
					return QVariant();
				}
				return getValue(key + ".editorpriority").toUInt();
			}
		}
		break;

	case Qt::ToolTipRole:
		if (_useMetaInfo) {
			QString ret = _metaInfos->getDocString(key, getClass(key));
			return ret.isEmpty() ? QVariant() : ret;
		}
		break;

	case Qt::ForegroundRole:
		if (_onlyParams && !isSet(key)) {
			return QColor(Qt::lightGray);
		}
		break;

	case Qt::BackgroundRole:
		switch (getValue(key+".editorpriority").toInt()) {
		case 1:
			return QColor("#8f8");
		case 2:
			return QColor("#ff8");
		case 3:
			return QColor("#f80");
		default:
			break;
		}
		break;

	case Qt::CheckStateRole:
		if (_useMetaInfo && col == 1 &&
				isParameter(key) && getType(key) == "bool") {
			const bool& checked = isSet(key) ?
					QVariant(getValue(key)).toBool() :
					QVariant(getDefault(key)).toBool();
			return checked ? Qt::Checked : Qt::Unchecked;
		}
		break;

	case Qt::StatusTipRole:
		if(col == 1) {
			if (isSet(key)) {
				QString ret = getValue(key);
				if(ret.startsWith('@')) {
					return tr("link to: %1").arg(ret.mid(1));
				}
			}
		}
		break;

	case Qt::DecorationRole:
		if(col == 1) {
			if (isSet(key)) {
				QString ret = getValue(key);
				if(ret.startsWith('@')) {
					return QIcon(":/icons/symlink.png");
				}
			}
		}
		break;

	} // role switch

	return QVariant();
}

bool ParameterFileModel::setData(
		const QModelIndex& ind, const QVariant& value, int role) {

	if (!prefixValid())
		return false;

	switch (role) {
	case Qt::EditRole:
	case Qt::DisplayRole:
		if ((ind.row() >= 0) && (ind.row() < _keys.size())) {
			switch (ind.column()) {
			case 0:
				if (_onlyParams)
					return false;

				if (value.canConvert(QVariant::String)) {
					QString oldName = _keys[ind.row()];
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
					QString val = getValue(oldName);
					erase(oldName);
					setValue(newName, val);
					emit dataChanged(ind, ind);
					return true;
				}
				break;
			case 1:
				if (value.canConvert(QVariant::String)) {
					QString valueStr = value.toString();
					QString keyStr = _keys[ind.row()];
					if (valueStr == getValue(keyStr))
						return true; // nothing to do

					setValue(keyStr, valueStr);
					if (_onlyParams && _metaInfos->isDynamic(getClass(keyStr))) {
						save();
						_updateDynamics();
						QTimer::singleShot(0, this, SLOT(_update()));
						emit dynamicUpdate();
					}
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
						if (isSet(_keys[ind.row()] + ".editorpriority")) {
							// check if value exists to prevent exceptions
							erase(_keys[ind.row()] + ".editorpriority");
							emit dataChanged(index(ind.row(), 0), ind);
						}
						return true;
					}

					if (valueStr ==
							getValue(_keys[ind.row()] + ".editorpriority")) {
						return true; // nothing to do
					}

					setValue(_keys[ind.row()] + ".editorpriority", valueStr);
					emit dataChanged(index(ind.row(), 0), ind);
					return true;
				}
				break;
			}
		}
		break;

	case Qt::CheckStateRole:
		if (_useMetaInfo && ind.column()==1 && isParameter(_keys[ind.row()])) {
			Q_ASSERT(getType(_keys[ind.row()]) == "bool");
			bool checked (value.toBool());
			setValue (_keys[ind.row()], checked ? "true" : "false");
            emit dataChanged(index(ind.row(),0),ind);
            if (_keys[ind.row()].contains("active") && checked == true){
                reactivatePreviousPlugins();
            }
            // additional check whether the changed Parameter is the ActiveInactive
            if (_keys[ind.row()].contains("active") && checked == false){
                deactivate();
            }
        }
		break;
	}
	return false;
}

void ParameterFileModel::reactivatePreviousPlugins(){
    QString tmpPrefix = _prefix;
    QStringList inputsOfOneSlot;
    int parameterIndex = 0;
    for (int p = 0; p < _keys.size(); p++){
        if (_keys[p].contains("active")){
            parameterIndex = p;
            break;
        }
    }
    for (int i = 0; i < tmpPrefix.size(); i++){
        if (tmpPrefix.at(i) == '.'){
            tmpPrefix.truncate(i);
        }
    }
    for (int i = 0; i < getInputs(tmpPrefix).size(); i++){
        if (getValue(tmpPrefix + "."+ getInputs(tmpPrefix).at(i)).contains(";")){
            inputsOfOneSlot = getValue(tmpPrefix + "."+ getInputs(tmpPrefix).at(i)).split(";");
            for (int o = 0; o < inputsOfOneSlot.size(); o++){
                setPrefix(inputsOfOneSlot.at(o));
                for (int p = 0; p < _keys.size(); p++){
                    if (_keys[p].contains("active")){
                        parameterIndex = p;
                        break;
                    }
                }
                setValue(_keys[parameterIndex], "true");
                reactivatePreviousPlugins();
            }
        }
        else if (getValue(tmpPrefix + "."+ getInputs(tmpPrefix).at(i)) != ""){
            setPrefix(getValue(tmpPrefix + "."+ getInputs(tmpPrefix).at(i)));
            for (int p = 0; p < _keys.size(); p++){
                if (_keys[p].contains("active")){
                    parameterIndex = p;
                    break;
                }
            }
            setValue(_keys[parameterIndex], "true");
            reactivatePreviousPlugins();
        }
    }

}

// Deactivates following Plugins
void ParameterFileModel::deactivate(){
    QString tmpPrefix = _prefix;
    QStringList outputsOfOneSlot;
    int parameterIndex = 0;
    for (int p = 0; p < _keys.size(); p++){
        if (_keys[p].contains("active")){
            parameterIndex = p;
            break;
        }
    }
    for (int i = 0; i < tmpPrefix.size(); i++){
        if (tmpPrefix.at(i) == '.'){
            tmpPrefix.truncate(i);
        }
    }
    for (int i = 0; i < getOutputs(tmpPrefix).size(); i++){
        if (getValue(tmpPrefix + "."+ getOutputs(tmpPrefix).at(i)).contains(";")){
            outputsOfOneSlot = getValue(tmpPrefix + "."+ getOutputs(tmpPrefix).at(i)).split(";");
            for (int o = 0; o < outputsOfOneSlot.size(); o++){
                setPrefix(outputsOfOneSlot.at(o));
                for (int p = 0; p < _keys.size(); p++){
                    if (_keys[p].contains("active")){
                        parameterIndex = p;
                        break;
                    }
                }
                setValue(_keys[parameterIndex], "false");
                deactivate();
            }
        }
        else if (getValue(tmpPrefix + "."+ getOutputs(tmpPrefix).at(i)) != ""){
            setPrefix(getValue(tmpPrefix + "."+ getOutputs(tmpPrefix).at(i)));
            for (int p = 0; p < _keys.size(); p++){
                if (_keys[p].contains("active")){
                    parameterIndex = p;
                    break;
                }
            }
            setValue(_keys[parameterIndex], "false");
            deactivate();
        }
    }
}

// Reactivates all following Plugins
void ParameterFileModel::reactivate(){
    QString tmpPrefix = _prefix;
    QStringList outputsOfOneSlot;
    int parameterIndex = 0;
    for (int p = 0; p < _keys.size(); p++){
        if (_keys[p].contains("active")){
            parameterIndex = p;
            break;
        }
    }
    if (getValue(_keys[parameterIndex]) == "false"){
        setValue(_keys[parameterIndex], "true");
        for (int i = 0; i < tmpPrefix.size(); i++){
            if (tmpPrefix.at(i) == '.'){
                tmpPrefix.truncate(i);
            }
        }
        for (int i = 0; i < getOutputs(tmpPrefix).size(); i++){
            if (getValue(tmpPrefix + "."+ getOutputs(tmpPrefix).at(i)).contains(";")){
                outputsOfOneSlot = getValue(tmpPrefix + "."+ getOutputs(tmpPrefix).at(i)).split(";");
                for (int o = 0; o < outputsOfOneSlot.size(); o++){
                    setPrefix(outputsOfOneSlot.at(o));
                    reactivate();
                }
            }
            else if (getValue(tmpPrefix + "."+ getOutputs(tmpPrefix).at(i)) != ""){
                setPrefix(getValue(tmpPrefix + "."+ getOutputs(tmpPrefix).at(i)));
                reactivate();
            }
        }

    }
}

bool ParameterFileModel::active() const {
    int parameterIndex=0;
    for (int p = 0; p < _keys.size(); p++){
        if (_keys[p].contains("active")){
            parameterIndex = p;
            break;
        }
    }
    if (getValue(_keys[parameterIndex]) == "false"){
        return false;
    }
    else {
        return true;
    }
}



Qt::ItemFlags ParameterFileModel::flags(const QModelIndex& ind) const {
	if (!prefixValid() || ind.row() < 0 || ind.row() >= _keys.size()) {
		return Qt::NoItemFlags;
	}
	Q_ASSERT(ind.row() >= 0);
	Q_ASSERT(ind.row() < _keys.size());

	Qt::ItemFlags res = Qt::NoItemFlags;
	QString paramType;
	QVariant::Type dataType;

	switch (ind.column()) {
	case 0:
		// parameter names editable, if _onlyParams is not selected
		res = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		if (!_onlyParams) {
			res = res | Qt::ItemIsEditable;
		}
		break;

	case 1:
		res = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

		paramType = getType(_keys[ind.row()]);
		dataType = data(ind).type();
		if (_useMetaInfo && isParameter(_keys[ind.row()])
				&& paramType == "bool") {
			// bool parameters are user checkable
			res = res | Qt::ItemIsUserCheckable;
		}
		else {
			// all non-bool parameters allow editing (delegate)
			res = res | Qt::ItemIsEditable;

			if (dataType == QVariant::String &&
					!paramType.contains(QRegExp("^\\{\\s*\\w.*\\}\\s*$"))) {
				// string entries that do not represent a selection
				// may be entered by dropping some values
				res = res | Qt::ItemIsDropEnabled;
			}
		}
		break;

	case 2:
		res = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
		break;

	default:
		break;
	}

	return res;
}

QVariant ParameterFileModel::headerData(int section,
		Qt::Orientation orientation, int role) const {

	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return tr("Parameter");
		case 1:
			return tr("Value");
		case 2:
			return tr("Priority");
		}
	}
	return QAbstractTableModel::headerData(section,orientation,role);
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
			setValue(name, "");
			if (_minPriority > 0) {
				setValue(name + ".editorpriority",
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

		for (int i = row; i < row + count; i++) {
			if (isSet(_keys[i])) {
				_parameterFile->erase(_keys[i]);
			}
		}

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
	if (_parameterFile->getKeyList().size() > 0) {
		bool lock = _resetMutex->tryLock();
		if (lock) {
			beginResetModel();
		}
		_parameterFile->clear();
		_keys.clear();
		setPrefix("");
		if (lock) {
			endResetModel();
			_resetMutex->unlock();
		}
	}
}

void ParameterFileModel::_updateDynamics() {
	if (QApplication::instance()) {
		LogDialog dialog(
			new LogDecorators::UpdateDynamics(_fileName));
		bool finished = dialog.waitForFinished(500);
		if (!finished || dialog.hasErrorLines()) {
			dialog.show();
			dialog.exec();
		}
	}
	else {
		qDebug("(WW) no qapplication, skipping dynamic loading");
	}
}

bool ParameterFileModel::_load() {
	QMutexLocker locker(_resetMutex);
	beginResetModel();
	clear();
	_parameterFile->load(_fileName);
	_updateDynamics();
	_update();
	endResetModel();
	locker.unlock();
	emit statusMessage(tr("File %1 loaded.").arg(_fileName));
	return true;
}

void ParameterFileModel::_update() {
	// remove keys
	bool lock = _resetMutex->tryLock();
	if (lock) {
		beginResetModel();
	}
	_keys.clear();

	// load all keys
	QStringList tempList = _parameterFile->getKeyList();
	if (tempList.size() > 0) {
		// apply modificators
		tempList = _prefixFilter(tempList);
		tempList = _paramFilter(tempList);
		tempList = _priorityTagFilter(tempList);
		tempList = _priorityFilter(tempList);

		// show selected parameters
		_keys = tempList;
	}
	if (lock) {
		endResetModel();
		_resetMutex->unlock();
	}
}

void ParameterFileModel::_updatePriority(
		const QModelIndex&, const QModelIndex &bottomRight) {
	// check if filter active and priority changed
	if (_minPriority > 0 && bottomRight.column() >= 2) {
		_update();
	}
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
				guess, tr("ParameterFiles (*.wrp);;All Files (*.*)"));
	}
	if (fromDialog.isEmpty()) {
		emit statusMessage(tr("no file selected"));
	}
	else if (!QFileInfo(fromDialog).isFile()) {
		QMessageBox::warning(0, tr("Error loading file"),
				tr("File <em>%1</em> does not exist or is no file!")
						.arg(fromDialog));
	}
	else if (!QFileInfo(fromDialog).isReadable()) {
		QMessageBox::warning(0, tr("Error loading file"),
				tr("File <em>%1</em> is not readable!").arg(fromDialog));
	}
	else {
		// fromDialog is a readable file now
		setFileName(fromDialog);
		return _load();
	}
	return false;
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
		// update class case map
		_classCaseMap.clear();
		const QStringList& classes = _metaInfos->getClasses();
		foreach (const QString& cur, classes) {
			_classCaseMap.insert(cur.toLower(), cur);
		}

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

	// let view reread information about clomumn 1 (values)
	if (rowCount() > 0) {
		emit dataChanged(index(0,1),index(rowCount()-1,1));
	}

	emit useMetaInfoChanged(value);
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

QString ParameterFileModel::getClass(QString name, bool fixCase) const {
	name = name.section(".",0,0);
	QString cName = getValue(name + ".type").toLower();
	if (fixCase) {
		cName = _classCaseMap.value(cName,cName);
	}
	return cName;
}

QStringList ParameterFileModel::_prefixFilter(QStringList list) const {
	if (_prefix.isEmpty()) {
		return list;
	}
	else if (prefixValid()) {
		return list.filter(QRegExp("^\\s*"+_prefix+"\\.",Qt::CaseInsensitive));
	}
	else {
		return list.filter(QRegExp("^\\s*"+_prefix,Qt::CaseInsensitive));
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
	if (_onlyParams) {
		QStringList result;
		QStringList objects = _collectObjects(list);

		// append all possible parameters for all objects
		for (int ii = 0; ii < objects.size(); ii++) {
			QStringList tmp;
			tmp << getParameters(objects[ii]);
			tmp.replaceInStrings(QRegExp("(^.*$)"), objects[ii]+".\\1");
			result << tmp;
		}
		return result;
	}
	else {
		return list;
	}
}

QStringList ParameterFileModel::_priorityTagFilter(QStringList list) const {
	QStringList result;
	foreach (const QString& cur, list) {
		if (!cur.endsWith(".editorpriority", Qt::CaseInsensitive)) {
			result << cur;
		}
	}
	return result;
}

QStringList ParameterFileModel::_priorityFilter(QStringList list) const {
	if (_minPriority <= 0) {
		return list;
	}
	QStringList result;
	foreach (const QString& cur, list) {
		if (getValue(cur+".editorpriority").toInt() >= _minPriority) {
			result << cur;
		}
	}
	return result;
}

QString ParameterFileModel::getType(QString parName, bool tmplType) const {
	if(!_useMetaInfo)
		return QString();
	QString cName = getClass(parName);
	QString res;

	if (_metaInfos->isDynamic(cName)) {
		QFileInfo fileInfo(_getDynamicMetaFile(parName.section(".",0,0)));
		if (fileInfo.exists()) {
			QParameterFile pFile(fileInfo.absoluteFilePath());
			res = pFile.get(cName + "." + parName.section(".",1) + ".type");
		}
	}
	if (res.isEmpty()) {
		res = _metaInfos->getType(parName, cName);
	}
	if (tmplType && res.contains(
			QRegExp("^\\s*(.*<\\s*T\\s*>.*|T)\\s*$",Qt::CaseInsensitive))) {
		parName = parName.section(".",0,0);
		QString tType = getValue(parName + ".templatetype");
		if(tType.isEmpty()) {
			tType = getDefault(parName + ".templatetype");
		}
		res.replace(
				QRegExp("<\\s*T\\s*>",Qt::CaseInsensitive),
				QString("<%1>").arg(tType));
		res.replace(
				QRegExp("^\\s*T\\s*$",Qt::CaseInsensitive),tType);
	}
	return res;
}

QString ParameterFileModel::getValue(QString parName) const {
	return _parameterFile -> get(parName);
}

bool ParameterFileModel::isSet(QString parName) const {
	return _parameterFile->isSet(parName);
}

void ParameterFileModel::setValue(QString parName, QString value) {
	if (isSet(parName) && getValue(parName) == value) {
		return;
	}

	// write value to parameter file
	bool existed = isSet(parName);
	_parameterFile -> set(parName, value);

	if (parName.endsWith(".editorcomment", Qt::CaseInsensitive)) {
		emit commentChanged(value);
	}

	if (existed) {
		// parameter existed before, so inform about change,
		// if parameter is visible
		const QStringList& keys = _parameterFile -> getKeyList();
		int i = keys.indexOf(parName);
		if ( i >= 0) {
			emit dataChanged(index(i, 1), index(i, 1));
		}
	}
	else {
		// parameter did not exist before,
		// so perhaps it would appear and an update run is neccessary
		_update();
	}
	emit modified(true);
}

void ParameterFileModel::erase(QString parName) {
	if (_parameterFile->isSet(parName)) {
		int pos = _keys.indexOf(parName);
		if (pos >= 0) {
			beginRemoveRows(QModelIndex(),pos,pos);
		}
		_parameterFile -> erase(parName);
		if (pos >= 0) {
			endRemoveRows();
		}
		emit modified(true);
	}
}

bool ParameterFileModel::rename(QString oldPrefix, QString newPrefix) {
	bool lock = _resetMutex->tryLock();
	if (lock) {
		beginResetModel();
	}
	setPrefix("");
	bool res = _parameterFile->rename(oldPrefix,newPrefix);
	if (lock) {
		endResetModel();
		_resetMutex->unlock();
	}
	if (res) {
		setPrefix(newPrefix);
		emit modified(true);
	}
	return res;
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

bool ParameterFileModel::dropMimeData(const QMimeData* mData,
	Qt::DropAction action, int row, int column, const QModelIndex& pInd) {

	// only accept text and files
	QString content;
	if (mData->hasUrls()) {
		QList<QUrl> urlList = mData->urls();
		if (urlList.size() >= 1) {
			content = urlList.at(0).path();
		}
		for (int ii = 1; ii < urlList.size(); ii += 1) {
			content += ";" + urlList.at(ii).path();
		}
	} else if (mData->hasText()) {
		content = mData->text();
	} else {
		return QAbstractTableModel::dropMimeData(mData, action,
			row, column, pInd);
	}
	
	setData(pInd, content);

	return true;
}

QString ParameterFileModel::_getDynamicMetaFile(QString objName) const {
	const FileManager& fm = FileManager::instance();
	return fm.configDir().absoluteFilePath("dynamics/"
		+ QFileInfo(_fileName).baseName() + "_" + objName + ".wrp");
}

QStringList ParameterFileModel::getInputs(QString objName) const {
	objName = objName.section(".",0,0);
	QString className = getClass(objName);
	if (_metaInfos->isDynamic(className)) {
		QFileInfo fileInfo(_getDynamicMetaFile(objName));
		if (fileInfo.exists()) {
			MetaData tmp(fileInfo.absoluteFilePath());
			return tmp.getInputs(className);
		}
	}
	return _metaInfos->getInputs(className);
}

QStringList ParameterFileModel::getInputDisplayNames(QString objName) const {
	objName = objName.section(".",0,0);
	QString className = getClass(objName);
	if (_metaInfos->isDynamic(className)) {
		QFileInfo fileInfo(_getDynamicMetaFile(objName));
		if (fileInfo.exists()) {
			MetaData tmp(fileInfo.absoluteFilePath());
			return tmp.getInputDisplayNames(className);
		}
	}
	return _metaInfos->getInputDisplayNames(className);
}

QStringList ParameterFileModel::getOutputs(QString objName) const {
	objName = objName.section(".",0,0);
	QString className = getClass(objName);
	if (_metaInfos->isDynamic(className)) {
		QFileInfo fileInfo(_getDynamicMetaFile(objName));
		if (fileInfo.exists()) {
			MetaData tmp(fileInfo.absoluteFilePath());
			return tmp.getOutputs(className);
		}
	}
	return _metaInfos->getOutputs(className);
}

QStringList ParameterFileModel::getOutputDisplayNames(QString objName) const {
	objName = objName.section(".",0,0);
	QString className = getClass(objName);
	if (_metaInfos->isDynamic(className)) {
		QFileInfo fileInfo(_getDynamicMetaFile(objName));
		if (fileInfo.exists()) {
			MetaData tmp(fileInfo.absoluteFilePath());
			return tmp.getOutputDisplayNames(className);
		}
	}
	return _metaInfos->getOutputDisplayNames(className);
}


QStringList ParameterFileModel::getParameters(QString objName) const {
	objName = objName.section(".",0,0);
	QString className = getClass(objName);
	if (_metaInfos->isDynamic(className)) {
		QFileInfo fileInfo(_getDynamicMetaFile(objName));
		if (fileInfo.exists()) {
			MetaData tmp(fileInfo.absoluteFilePath());
			return tmp.getParameters(className);
		}
	}
	return _metaInfos->getParameters(className);
}

bool ParameterFileModel::isParameter(QString name) const {
	QString obj = name.section(".",0,0);
	name =  name.section(".",-1);
	QStringList list = getParameters(obj);
	return (list.indexOf(QRegExp(name,Qt::CaseInsensitive)) >= 0);
}

bool ParameterFileModel::isInputSlot(QString name) const {
	QString obj = name.section(".",0,0);
	name =  name.section(".",-1);
	QStringList list = getInputs(obj);
	return (list.indexOf(QRegExp(name,Qt::CaseInsensitive)) >= 0);
}

bool ParameterFileModel::isOutputSlot(QString name) const {
	QString obj = name.section(".",0,0);
	name =  name.section(".",-1);
	QStringList list = getOutputs(obj);
	return (list.indexOf(QRegExp(name,Qt::CaseInsensitive)) >= 0);
}

bool ParameterFileModel::isMultiSlot(QString slotName) const {
	QString className = getClass(slotName);
	QString objName = slotName.section(".",0,0);
	if (_metaInfos->isDynamic(className)) {
		QFileInfo fileInfo(_getDynamicMetaFile(objName));
		if (fileInfo.exists()) {
			MetaData tmp(fileInfo.absoluteFilePath());
			return tmp.isMultiSlot(slotName,className);
		}
	}
	return _metaInfos->isMultiSlot(slotName,className);
}

QString ParameterFileModel::getDoc(QString parName) const {
	QString className = getClass(parName);
	QString objName = parName.section(".",0,0);
	if (_metaInfos->isDynamic(className)) {
		QFileInfo fileInfo(_getDynamicMetaFile(objName));
		if (fileInfo.exists()) {
			MetaData tmp(fileInfo.absoluteFilePath());
			return tmp.getDocString(parName,className);
		}
	}
	return _metaInfos->getDocString(parName,className);
}

QStringList ParameterFileModel::getClasses() const {
	return _metaInfos->getClasses();
}

QString ParameterFileModel::getDefault(QString parName) const {
	QString className = getClass(parName);
	QString objName = parName.section(".",0,0);

	// handle default template type
	QRegExp ttype("(.*\\.)?templatetype",Qt::CaseInsensitive);
	QString defaultTmplType = getValue("global.templatetype");
	if (ttype.exactMatch(parName) && !defaultTmplType.isEmpty()) {
		return defaultTmplType;
	}

	if (_metaInfos->isDynamic(className)) {
		QFileInfo fileInfo(_getDynamicMetaFile(objName));
		if (fileInfo.exists()) {
			MetaData tmp(fileInfo.absoluteFilePath());
			return tmp.getDefault(parName,className);
		}
	}
	return _metaInfos->getDefault(parName,className);
}

bool ParameterFileModel::metaInfoValid() const {
	return _metaInfos;
}
