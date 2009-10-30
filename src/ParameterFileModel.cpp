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
#include "ParameterFile.hxx"
#include "PrefixValidator.h"
#include "VarTypeMap.h"
#include "MetaData.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include "PluginManager.h"
#include "FileManager.h"
#include <charon-utils/CImg.h>

#include "ParameterFileModel.moc"

ParameterFileModel::ParameterFileModel(QString fName, QObject* myParent,
		QString metaFile) :
	QAbstractTableModel(myParent), _fileName(fName), _prefix(""),
			_metaInfos(0), _useMetaInfo(false), _onlyParams(false) {
	_parameterFile = new ParameterFile();

	// this ParameterFile stores the description of classes.
	if (!metaFile.isEmpty()) {
		loadMetaInfo(metaFile);
		if (metaInfo()->valid())
			setOnlyParams(true);
	}
	if (!_fileName.isEmpty())
		_load();
	else
		_fileName = QDir::homePath();
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
	return 2;
}

QVariant ParameterFileModel::data(const QModelIndex& ind, int role) const {
	try {
		// mapper to convert parameter.type into QVariant::Type
		const VarTypeMap& mapper = VarTypeMap::instance();

		if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
			int row = ind.row();

			if ((row >= 0) && ((uint) row < _keys.size())) {
				QString name;

				switch (ind.column()) {
				case 0:
					// parameter name (without prefix)
					name = _keys[row].c_str();
					if (!_prefix.isEmpty())
						name.remove(0, _prefix.length());

					// remove dot after valid prefix
					if (name[0] == '.')
						name.remove(0, 1);
					return name;

				case 1:
					QVariant res = "";
					std::string className = getClass(_keys[row]);
					if (_parameterFile->isSet(_keys[row]))
						res = _parameterFile->get<std::string> (_keys[row]).c_str();
					else if (_onlyParams)
						res = metaInfo()->getDefault(_keys[row], className).c_str();

					if (_useMetaInfo && metaInfo()->isParameter(_keys[row],
							className)) {
						std::string typestring = getType(_keys[row]);
						QVariant::Type type = mapper[typestring];
						Q_ASSERT(res.canConvert(type));
						res.convert(type);
					}
					// simply return the string value
					return res;
				}
			}
		}
		if ((role == Qt::ToolTipRole) && _useMetaInfo) {
			QString ret = metaInfo()->getDocString(_keys[ind.row()], getClass(
					_keys[ind.row()])).c_str();
			if (!ret.isEmpty())
				return ret;
		}
		if (role == Qt::ForegroundRole && _onlyParams) {
			if (!_parameterFile->isSet(_keys[ind.row()]))
				return Qt::lightGray;
		}
		if (ind.column() == 1 && role == Qt::CheckStateRole && _useMetaInfo
				&& metaInfo()->isParameter(_keys[ind.row()],
						getClass(_keys[ind.row()]))) {
			std::string typestring = getType(_keys[ind.row()]);
			if (typestring == "bool") {
				if(_parameterFile->isSet(_keys[ind.row()])) {
					if (_parameterFile->get<bool>(_keys[ind.row()]))
						return Qt::Checked;
					else
						return Qt::Unchecked;
				}
				else {
					std::istringstream tmp(metaInfo()->getDefault(_keys[ind.row()],
							getClass(_keys[ind.row()])));
					bool val;
					tmp >> val;
					if (val)
						return Qt::Checked;
					else
						return Qt::Unchecked;
				}
			}
		}
	}
	catch (std::string msg) {
		qDebug() << QString("Error in ParameterModel::data()\nMessage: %1")
					.arg(msg.c_str());
	}
	return QVariant();
}

bool ParameterFileModel::setData(const QModelIndex& ind, const QVariant& value,
		int role) {

	if (!prefixValid())
		return false;

	if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
		if ((ind.row() >= 0) && ((uint) ind.row() < _keys.size())) {
			switch (ind.column()) {
			case 0:
				if (_onlyParams)
					return false;

				if (value.canConvert(QVariant::String)) {
					std::string& oldName = _keys[ind.row()];
					std::string newName;
					// do not forget the prefix at the beginning of the name
					if (!_prefix.isEmpty())
						newName = std::string(_prefix.toAscii().constData())
								+ ".";
					newName += value.toString().toAscii().constData();
					if (oldName == newName)
						return true; // nothing to do
					if (_parameterFile->isSet(newName))
						return false; // don't overwrite existing value

					// save value
					std::string val =
							_parameterFile->get<std::string> (oldName);
					_parameterFile->erase(oldName);
					_parameterFile->set(newName, val);
					oldName = newName; // save new name in name cache
					emit dataChanged(ind, ind);
					return true;
				}
				break;
			case 1:
				if (value.canConvert(QVariant::String)) {
					std::string valueStr =
							value.toString().toAscii().constData();
					if (_parameterFile->isSet(_keys[ind.row()]) && (valueStr
							== _parameterFile->get<std::string> (
									_keys[ind.row()])))
						return true; // nothing to do

					_parameterFile->set<std::string> (_keys[ind.row()],
							valueStr);
					emit dataChanged(index(ind.row(), 0), ind);
					return true;
				}
				break;
			}
			std::cerr << "Got invalid Data in setData" << std::endl;
			std::cerr << "Row: " << ind.row() << "\t";
			std::cerr << "Column: " << ind.column() << std::endl;
		}
	}
	if (ind.column() == 1 && role == Qt::CheckStateRole && _useMetaInfo
				&& metaInfo()->isParameter(_keys[ind.row()],
						getClass(_keys[ind.row()]))) {
		Q_ASSERT(getType(_keys[ind.row()]) == "bool");
		_parameterFile->set<bool>(_keys[ind.row()],value.toBool());
		emit(dataChanged(ind,ind));
		qDebug() << value;
	}
	return false;
}

Qt::ItemFlags ParameterFileModel::flags(const QModelIndex& ind) const {
	if (!prefixValid())
		return 0;
	if ((ind.row() >= 0) && ((uint) ind.row() <= _keys.size())) {
		switch (ind.column()) {
		case 0:
			if (_onlyParams)
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		case 1:
			if(_useMetaInfo && metaInfo()->isParameter(_keys[ind.row()],
					getClass(_keys[ind.row()]))
				&& getType(_keys[ind.row()]) == "bool")
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled
					| Qt::ItemIsUserCheckable;
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
			}
		} else {
			if ((section >= 0) && ((uint) section < _keys.size()))
				return section;
		}
	}
	return QVariant();
}

bool ParameterFileModel::insertRows(int row, int count,
		const QModelIndex& parentInd) {

	if (!prefixValid())
		return false;
	if (_onlyParams)
		return false;

	// only appending is allowed!
	if ((row >= 0) && ((uint) row == _keys.size()) && (count > 0)) {
		beginInsertRows(parentInd, row, row + count - 1);
		int offset = 0;
		int i = 1;

		// append new values
		while (i <= count) {
			// get temp name for new parameter
			std::ostringstream name;
			if (!_prefix.isEmpty())
				name << _prefix.toAscii().constData() << ".";
			name << "new" << i + offset;
			if (_parameterFile->isSet(name.str())) {
				offset++;
				continue;
			}
			_parameterFile->set(name.str(), "");
			i++;
		}
		std::string prefixStr = _prefix.toAscii().constData();
		_keys = _parameterFile->getKeyList(prefixStr);

		// apply modificators
		if (!_prefix.isEmpty())
			_keys = _prefixFilter(_keys);

		// check new number of elements
		Q_ASSERT(_keys.size() == (uint) (row + count));
		endInsertRows();
		return true;
	}
	return false;
}

bool ParameterFileModel::removeRows(int row, int count,
		const QModelIndex& parentInd) {

	// forbid modifications on invalid prefix
	if (!prefixValid())
		return false;

	// check row and count dimensions (have to fit into table)
	if ((count > 0) && (row >= 0) && ((uint) (row + count) <= _keys.size())) {
		// when onlyparams is set, values are not really removed, but
		// reset to their default values
		if (!_onlyParams)
			beginRemoveRows(parentInd, row, row + count - 1);

		for (int i = row; i < row + count; i++)
			if (_parameterFile->isSet(_keys[i]))
				_parameterFile->erase(_keys[i]);

		// keys have not to be changed on onlyparam mode
		if (!_onlyParams) {
			_keys = _parameterFile->getKeyList(_prefix.toAscii().constData());
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
		beginRemoveRows(QModelIndex(), 0, keys - 1);
		_parameterFile->clear();
		_keys.clear();
		setPrefix("");
		endRemoveRows();
	}
}

void ParameterFileModel::_load() {
	clear();
	// get Number of Parameters in new file
	_parameterFile->load(_fileName.toAscii().constData());
	_update();
	emit statusMessage(QString("File %1 loaded.").arg(_fileName));
}

void ParameterFileModel::_update() {
	// remove keys
	uint keys = _keys.size();
	if (keys > 0) {
		beginRemoveRows(QModelIndex(), 0, keys - 1);
		_keys.clear();
		endRemoveRows();
	}

	// load all keys
	std::vector<std::string> tempList = _parameterFile->getKeyList();
	if (tempList.size() == 0)
		return;

	// apply modificators
	if (!_prefix.isEmpty())
		tempList = _prefixFilter(tempList);
	if (_onlyParams)
		tempList = _paramFilter(tempList);

	// show selected parameters
	if (tempList.size()) {
		beginInsertRows(QModelIndex(), 0, tempList.size() - 1);
		_keys = tempList;
		endInsertRows();
	}
}

void ParameterFileModel::load(const QString& fName) {
	QString fromDialog = fName;

	QString guess = _fileName;
	if (guess.isEmpty() || guess == QDir::homePath()) {
		QSettings settings("Heidelberg Collaboratory for Image Processing",
			"Tuchulcha");
		QStringList files = settings.value("recentFileList").toStringList();
		if (files.size() > 0)
			guess = files[0];
		else
			guess = QDir::homePath();
	}
	if (fromDialog.isEmpty())
		fromDialog = QFileDialog::getOpenFileName(0, tr("Open File"),
				guess, tr("ParameterFile (*.*)"));
	setFileName(fromDialog);
	if (fromDialog.isEmpty())
		throw std::string("Empty filename");

	_load();
}

void ParameterFileModel::save(const QString& fName) {
	QString name;
	if ((!_fileName.isEmpty()) && (_fileName != QDir::homePath()))
		name = _fileName;
	if (!fName.isEmpty())
		name = fName;
	if (!name.isEmpty()) {
		setFileName(name);
		_parameterFile->save(name.toAscii().constData());
		emit statusMessage(QString("File %1 saved.").arg(name));
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

const ParameterFile& ParameterFileModel::parameterFile() const {
	return *_parameterFile;
}

QString ParameterFileModel::fileName() const {
	return _fileName;
}

QString ParameterFileModel::prefix() const {
	return _prefix;
}

bool ParameterFileModel::prefixValid() const {
	if (_prefix.isEmpty())
		return true;
	return (_parameterFile->getKeyList(std::string(
			_prefix.toAscii().constData()) + ".").size() > 0);
}

void ParameterFileModel::loadMetaInfo(const QString& fName) {
	if (_metaInfos) {
		delete _metaInfos;
		_metaInfos = 0;
	}
	if (fName.isEmpty()) {
		_metaInfos = 0;
		emit metaInfoChanged(false);
	} else {
		std::string fNameStr = fName.toAscii().constData();
		_metaInfos = new MetaData(fNameStr);
		setUseMetaInfo(metaInfo()->valid());
		emit metaInfoChanged(metaInfo()->valid());
	}
}

bool ParameterFileModel::useMetaInfo() const {
	return _useMetaInfo;
}

void ParameterFileModel::setUseMetaInfo(bool value) {
	if (value == _useMetaInfo)
		return;
	if ((!metaInfo()->valid()) && value)
		qDebug() << "no valid metadata given, cannot activate _useMetadata";
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

void ParameterFileModel::setOnlyParams(bool on) {
	if (_onlyParams == on)
		return;
	if ((!metaInfo()->valid()) && on) {
		qDebug() << "no valid metadata given, onlyparams is not avaiable";
		_onlyParams = false;
	} else {
		if ((!_useMetaInfo) && on)
			setUseMetaInfo(on);
		_onlyParams = on;
	}
	_update();
	emit onlyParamsChanged(_onlyParams);
}

std::string ParameterFileModel::getClass(std::string name) const {
	std::string::size_type pos = name.find(".");
	if (pos != std::string::npos)
		name = name.substr(0, pos);
	std::string className;
	if (_parameterFile->isSet(name + ".type"))
		className = _parameterFile->get<std::string> (name + ".type");
	else
		throw "Class name of " + name + " not set!";
	return className;
}

std::vector<std::string> ParameterFileModel::_prefixFilter(const std::vector<
		std::string>& list) const {
	std::string pref = _prefix.toAscii().constData();
	std::string::size_type length = pref.length();
	Q_ASSERT(length > 0);
	std::vector<std::string> result;
	std::vector<std::string>::const_iterator listIter;
	for (listIter = list.begin(); listIter != list.end(); listIter++) {
		if (prefixValid()) {
			if (listIter->substr(0, length + 1) == (pref + "."))
				result.push_back(*listIter);
		} else if (listIter->substr(0, length) == pref)
			result.push_back(*listIter);
	}
	return result;
}

std::set<std::string> ParameterFileModel::_collectObjects(const std::vector<
		std::string>& list) const {
	std::set<std::string> result;
	std::string::size_type pos;
	std::string object;
	std::vector<std::string>::const_iterator listIter;
	std::set<std::string>::const_iterator objIter;
	for (listIter = list.begin(); listIter != list.end(); listIter++) {
		pos = listIter->find(".");
		if ((pos == 0) || (pos == std::string::npos))
			throw "invalid file format! Line \"" + *listIter + "\" invalid.";
		result.insert(listIter->substr(0, pos));
	}

	if (_useMetaInfo) {
		Q_ASSERT(metaInfo()->valid());
		for (objIter = result.begin(); objIter != result.end(); objIter++) {
			getClass(*objIter);
		}
	}
	return result;
}

std::vector<std::string> ParameterFileModel::_paramFilter(const std::vector<
		std::string>& list) const {
	Q_ASSERT(_onlyParams);
	std::list<std::string> temp;
	const std::set<std::string>& objects = _collectObjects(list);
	std::set<std::string>::const_iterator obj;

	// append all possible parameters for all objects
	for (obj = objects.begin(); obj != objects.end(); obj++) {
		const std::vector<std::string>& pars = metaInfo()->getParameters(
				getClass(*obj));
		for (uint i = 0; i < pars.size(); i++)
			temp.push_back(*obj + "." + pars[i]);
	}

	std::vector<std::string> result;
	result.resize(temp.size());
	result.assign(temp.begin(), temp.end());
	return result;
}

void ParameterFileModel::executeWorkflow() {
	std::string globalPluginPath = FileManager::instance().getGlobalPluginPath();
	std::string privatePluginPath = FileManager::instance().getPrivatePluginPath();
	PluginManager man(globalPluginPath,privatePluginPath);
	std::string logFile = FileManager::instance().configDir()
		.path().toAscii().constData();
	logFile += "/executeLog.txt";
	std::ofstream log(logFile.c_str(), std::ios::trunc);
	Q_ASSERT(log.good());
	sout.assign(log, std::cout);
	try {
		man.loadParameterFile(*_parameterFile);
		man.executeWorkflow();
	}
	catch (const std::string& msg) {
		QMessageBox::warning(0, tr("error during execution"),
			tr("Caught exception of type string.\n\nMessage:\n%1")
				.arg(msg.c_str()));
	}
	catch (const std::exception& excpt) {
		QMessageBox::warning(0, tr("error during execution"),
			tr("Caught exception of type %1.\n\nMessage:\n%2")
				.arg(typeid(excpt).name())
				.arg(excpt.what()));
	}
	catch (const cimg_library::CImgException& excpt) {
		QMessageBox::warning(0, tr("error during execution"),
			tr("Caught CImg exception of type %1.\n\nMessage:\n%2")
				.arg(typeid(excpt).name())
				.arg(excpt.message));
	}
	catch (...) {
		QMessageBox::warning(0, tr("error during execution"),
			tr("Caught exception of unknown type"));
	}
	sout.assign();
	log.close();
}

std::string ParameterFileModel::getType(std::string parName) const {
    if(!_useMetaInfo)
        return "";
    std::string res = metaInfo()->getType(parName, getClass(parName));
    std::string::size_type pos=StringTool::toLowerCase(res).find("<t>");
    if((pos != std::string::npos) || StringTool::toLowerCase(res)=="t") {
        std::string instanceName = parName.substr(0, parName.find('.'));
        if(parameterFile().isSet(instanceName + ".templatetype")) {
            if(pos != std::string::npos)
                res.replace(pos, 3, "<" + parameterFile().
                    get<std::string>(instanceName + ".templatetype") + ">");
            else if(StringTool::toLowerCase(res) == "t")
                res = parameterFile().get<std::string>(instanceName + ".templatetype");
        } else {
            try{
                std::string type = metaInfo()->getDefault("templatetype", getClass(parName));
                if (pos != std::string::npos)
                    res.replace(pos, 3, std::string("<") + type  + ">");
                else if(StringTool::toLowerCase(res) == "t")
                    res = type;
            } catch (...) {
                qDebug() << "Templated Slot in non-templated Object found.";
            }
        }
    }
    return res;
}
