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
/** \file   GraphModel.cpp
 *  \brief  Implementation of class GraphModel
 *
 *  \date   10.09.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "GraphModel.h"
#include "ParameterFile.hxx"
#include "MetaData.h"
#include <QMessageBox>
#include <QtDebug>
#include <sstream>
#include <set>
#include <algorithm>
#include <QInputDialog>

#include "GraphModel.moc"

/// transform std::string into lowercase
/** \param[in] input  string to transform
 *  \returns          lowercase version
 */
inline std::string _toLower(std::string input) {
	std::transform(
		input.begin(), input.end(), input.begin(),
		(int(*)(int)) tolower);
	return input;
}

GraphModel::GraphModel(QString fName, QObject* myParent, QString metaFile) :
		ParameterFileModel(fName, myParent, metaFile) {

	// here we need some metaFile
	if (metaFile.isEmpty())
		qFatal("No metaFile in GraphModel given! Pleas specify one!");
}

GraphModel::~GraphModel() {
}

void GraphModel::clear(bool draw) {
	setPrefix("", false);
	ParameterFileModel::clear();
	if(draw)
		emit graphChanged();
}

void GraphModel::clear() {
	clear(true);
}

void GraphModel::reDraw() {
	emit graphChanged();
}

void GraphModel::_load() {
	ParameterFileModel::_load();
	QStringList n = nodes();
	if(n.size())
		setPrefix(n[0]);
}

bool GraphModel::nodeValid(const QString& name) const {
	try {
		return !getClass(name.toAscii().constData()).empty();
	}
	catch (const ParameterFile::Unset&) {
		return false;
	}
}

bool GraphModel::connected(QString source,
						   QString target) const {
	// assert lowercase names
	source = source.toLower();
	target = target.toLower();

	QStringList sourceSep = source.split(".");
	QStringList targetSep = target.split(".");

	// source and target have to be slots
	Q_ASSERT(source.indexOf(";") < 0);
	Q_ASSERT(target.indexOf(";") < 0);
	Q_ASSERT(sourceSep.size() == 2);
	Q_ASSERT(targetSep.size() == 2);

	// for convenience
	std::string sourceSlot = sourceSep[1].toAscii().constData();
	std::string targetSlot = targetSep[1].toAscii().constData();

	std::string sourceClass = getClass(source.toAscii().constData());
	std::vector<std::string> sourceOutputs =
			metaInfo()->getOutputs(sourceClass);
	std::transform(
			sourceOutputs.begin(), sourceOutputs.end(), sourceOutputs.begin(),
			_toLower);
	std::vector<std::string> sourceInputs  =
			metaInfo()->getInputs(sourceClass);
	std::transform(
			sourceInputs.begin(), sourceInputs.end(), sourceInputs.begin(),
			_toLower);
	std::vector<std::string>::const_iterator outIter;

	// source has to be an input/output
	bool sourceIsOutput = true;

	outIter = std::find(sourceOutputs.begin(), sourceOutputs.end(),
						sourceSlot);
	if(outIter == sourceOutputs.end())
		sourceIsOutput = false;

	if(!sourceIsOutput) {
		outIter = std::find(sourceInputs.begin(), sourceInputs.end(),
							sourceSlot);
		if(outIter == sourceInputs.end())
			throw std::runtime_error(
					std::string("source ") + source.toAscii().constData()
					+ " is neither input nor output slot!");
	}

	// check if target is of the corresponding slot type (input <-> output)
	std::string targetClass = getClass(target.toAscii().constData());
	std::vector<std::string> targetOutputs =
			metaInfo()->getOutputs(targetClass);
	std::transform(
			targetOutputs.begin(), targetOutputs.end(), targetOutputs.begin(),
			_toLower);
	std::vector<std::string> targetInputs  =
			metaInfo()->getInputs(targetClass);
	std::transform(
			targetInputs.begin(), targetInputs.end(), targetInputs.begin(),
			_toLower);
	if (sourceIsOutput) {
		outIter = std::find(targetInputs.begin(), targetInputs.end(),
							targetSlot);
		if (outIter == targetInputs.end())
			throw std::runtime_error(
					std::string(target.toAscii().constData())
					+ " has to be an input!");
	}
	else {
		outIter = std::find(targetOutputs.begin(), targetOutputs.end(),
							targetSlot);
		if (outIter == targetOutputs.end())
			throw std::runtime_error(
					std::string(target.toAscii().constData())
					+ " has to be an output!");
	}

	// check slot types
	std::string inSlotType  = getType(target.toAscii().constData());
	std::string outSlotType = getType(source.toAscii().constData());
	if (inSlotType != outSlotType)
		throw std::runtime_error(
				std::string("Type of \"") + target.toAscii().constData()
				+ "\" (" + inSlotType + ") does not match type of \""
				+ source.toAscii().constData() + "\" ("
				+ outSlotType + ")");

	bool established = true;


	if(!parameterFile().isSet(source.toAscii().constData()))
		established = false;
	else {
		std::string outList = parameterFile()
							  .get<std::string>(source.toAscii().constData());
		if(outList.find(target.toAscii().constData()) == std::string::npos)
			established = false;
	}

	// check if target node is in input/ouput list of source
	if (!parameterFile().isSet(target.toAscii().constData())) {
		if (established)
			throw std::runtime_error(std::string("Node ")
			+ source.toAscii().constData() + " missing in List "
					+ target.toAscii().constData() + "!");
	}
	else {
		std::string inList = parameterFile()
							 .get<std::string>(target.toAscii().constData());
		inList = _toLower(inList);
		if((inList.find(source.toAscii().constData()) == std::string::npos)
			&& established)
			throw std::runtime_error(std::string("Node ")
			+ source.toAscii().constData() + " missing in List "
					+ target.toAscii().constData() + "!");
	}

	return established;
}

QString GraphModel::setPrefix(const QString& nodename, bool draw) {
	// avoid loops
	if(nodename == prefix())
		return prefix();

	// set parameterfileModel prefix
	QString ret = ParameterFileModel::setPrefix(nodename);

	if(!draw)
		return ret;

	// check if prefix is valid
	if (!prefixValid())
		return ret;

	emit graphChanged();
	return ret;
}

void GraphModel::loadMetaFile(const QString& fName) {
	if(fName.isEmpty())
		qFatal("Tried to set emtpy metaFile in GraphModel!");
	ParameterFileModel::loadMetaInfo(fName);
}

void GraphModel::connectSlot(const QString& source, const QString& target,
							 bool draw) {
	// check for valid connection and return, if this connection already
	// exists.
	if (connected(source, target))
		return;

	// identify input and output slot
	std::string sourceStr   = source.toAscii().constData();
	std::string sourceObj   = _toLower(ParameterFile::objName(sourceStr));
	std::string sourcePar   = _toLower(ParameterFile::parName(sourceStr));
	std::string sourceClass = _toLower(getClass(sourceStr));

	std::string targetStr   = target.toAscii().constData();
	std::string targetObj   = _toLower(ParameterFile::objName(targetStr));
	std::string targetPar   = _toLower(ParameterFile::parName(targetStr));
	std::string targetClass = _toLower(getClass(targetStr));

	bool sourceIsIn = metaInfo()->isInputSlot(sourceStr, sourceClass);
	if(!sourceIsIn) {
		// swap source and target
		sourceStr.swap(targetStr);
		sourceObj.swap(targetObj);
		sourcePar.swap(targetPar);
		sourceClass.swap(targetClass);
	}

	Q_ASSERT(metaInfo()->isInputSlot (sourceStr, sourceClass));
	Q_ASSERT(metaInfo()->isOutputSlot(targetStr, targetClass));

	// disconnect input slot, if assigned and not multi slot
	if (!metaInfo()->isMultiSlot(sourceStr, sourceClass)) {
		std::string val;
		if (parameterFile().isSet(sourceStr))
			val = parameterFile().get<std::string>(sourceStr);
		if (val.size() > 0)
			disconnectSlot(sourceStr.c_str(), val.c_str(), false);
	}

	// add target to source
	QString pref = setPrefix(sourceObj.c_str(), false);
	setOnlyParams(false);
	if (parameterFile().isSet(sourceStr)) {
		for(int i=0; i<rowCount(); i++) {
			if (data(index(i, 0)) == sourcePar.c_str()) {
				// check target is not in list
				QString content = data(index(i, 1)).toString().toLower();
				Q_ASSERT(content.indexOf(targetStr.c_str()) < 0);
				QStringList targetList = content.split(
						";", QString::SkipEmptyParts);
				// add new target
				targetList << targetStr.c_str();
				setData(index(i, 1), targetList.join(";"));
				break;
			}
		}
	}
	else {
		uint row = rowCount();
		insertRow(row);
		setData(index(row, 0), sourcePar.c_str());
		setData(index(row, 1), targetStr.c_str());
	}

	// add source to target
	setPrefix(targetObj.c_str(), false);
	if (parameterFile().isSet(targetStr)) {
		for(int i=0; i<rowCount(); i++) {
			if (data(index(i, 0)) == targetPar.c_str()) {
				// check source is not in list
				QString content = data(index(i, 1)).toString().toLower();
				if (content.indexOf(sourceStr.c_str()) < 0) {
					QStringList sourceList = content.split(
							";", QString::SkipEmptyParts);
					// add new target
					sourceList << sourceStr.c_str();
					setData(index(i, 1), sourceList.join(";"));
				}
				break;
			}
		}
	}
	else {
		uint row = rowCount();
		insertRow(row);
		setData(index(row, 0), targetPar.c_str());
		setData(index(row, 1), sourceStr.c_str());
	}

	// restore prefix and onlyparams
	setOnlyParams(true);
	setPrefix(pref, false);

	if(draw)
		emit graphChanged();

	emit statusMessage(tr("attempt to connect %1 with %2")
					   .arg(source).arg(target));
}

void GraphModel::disconnectSlot(const QString& source, const QString& target,
								bool draw) {
	QStringList sourceSep = source.toLower().split(".");
	QStringList targetSep = target.toLower().split(".");
	Q_ASSERT(sourceSep.size() == 2);
	Q_ASSERT(targetSep.size() == 2);
	QString prefixSave = setPrefix(sourceSep[0], false);
	setOnlyParams(false);
	Q_ASSERT(prefixValid());
	for(int i=0; i<rowCount(); i++) {
		if (data(createIndex(i, 0)) == sourceSep[1]) {
			// check target is in list
			QString content = data(createIndex(i, 1)).toString().toLower();
			Q_ASSERT(content.indexOf(target.toLower()) >= 0);
			QStringList targets = content.split(";", QString::SkipEmptyParts);
			int pos = targets.indexOf(target.toLower());
			Q_ASSERT(pos >= 0);
			targets.removeAt(pos);
			setData(createIndex(i, 1), targets.join(";"));
		}
	}
	setPrefix(targetSep[0], false);
	for(int i=0; i<rowCount(); i++) {
		if (data(createIndex(i, 0)) == targetSep[1]) {
			// check target is in list
			QString content = data(createIndex(i, 1)).toString().toLower();
			QStringList targets = content.split(";", QString::SkipEmptyParts);
			int pos = targets.indexOf(source.toLower());
			if (pos >= 0) {
				targets.removeAt(pos);
				setData(createIndex(i, 1), targets.join(";"));
			}
		}
	}

	setOnlyParams(true);
	setPrefix(prefixSave, false);

	if(draw)
		emit graphChanged();

	emit statusMessage(tr("disconnected node %1 from %2")
					   .arg(source).arg(target));
}

QStringList GraphModel::_connections(QString node) const {
	node = node.split(".")[0].toLower(); // get base name
	std::string className = getClass(node.toAscii().constData());

	// collect input & output slots
	std::vector<std::string> inputs  =
			metaInfo()->getInputs (className);
	std::vector<std::string> outputs =
			metaInfo()->getOutputs(className);
	std::vector<std::string> curSlot;

	std::vector<std::string>::const_iterator slot;
	std::vector<std::string>::const_iterator target;

	QStringList result;

	// collect inputs
	for (slot = inputs.begin(); slot != inputs.end(); slot++) {
		QString slotName = QString("%1.%2").arg(node).arg(slot->c_str());
		if (!parameterFile().isSet(slotName.toAscii().constData()))
			continue;
		curSlot = parameterFile().getList<std::string>(
				slotName.toAscii().constData());
		// only multi slots can have more than one source!
		Q_ASSERT(metaInfo()->isMultiSlot(*slot, className)
				 || (curSlot.size() <= 1));
		std::vector<std::string>::const_iterator curSlotIter;
		for(curSlotIter = curSlot.begin(); curSlotIter != curSlot.end();
		curSlotIter++)
			result << QString("%1;%2").arg(slot->c_str())
			.arg(curSlotIter->c_str());
	}

	// collect outputs
	for (slot = outputs.begin(); slot != outputs.end(); slot++) {
		QString slotName = QString("%1.%2").arg(node).arg(slot->c_str());
		if (!parameterFile().isSet(slotName.toAscii().constData()))
			continue;
		curSlot = parameterFile().getList<std::string>(
				slotName.toAscii().constData());
		if (curSlot.size() > 0)
			// number of targets of an output slot is unlimited
			for(target = curSlot.begin(); target != curSlot.end(); target++)
				result << QString("%1;%2").arg(slot->c_str())
				.arg(target->c_str());
	}

	return result;
}

void GraphModel::disconnectAllSlots(QString node, bool draw) {
	node = node.split(".")[0]; // get base name
	QStringList connections = _connections(node);
	QStringList::const_iterator con;

	// disconnect each slot
	for(con = connections.begin(); con != connections.end(); con++){
		QStringList sep = con->split(";");
		Q_ASSERT(sep.size() == 2);
		disconnectSlot(QString("%1.%2").arg(node).arg(sep[0]), sep[1], false);
	}

	if(draw)
		emit graphChanged();

	emit statusMessage(tr("disconnected all slots of node %1").arg(node));
}

void GraphModel::renameNode(QString nodename, bool draw) {
	nodename = nodename.split(".")[0];
	bool ok;
	QString newName = QInputDialog::getText(
			0, tr("rename node"),
			tr("Enter new name for node \"%1\":").arg(nodename),
			QLineEdit::Normal, nodename, &ok);
	if (ok) {
		if(this->nodeValid(newName)) {
			QMessageBox::warning(
					0, tr("node exists"),
					tr("A node named <i>%1</i> does already exist.<br/>"
					   "Please choose another name.").arg(newName));
			return;
		}
		setPrefix("", false);
		setOnlyParams(false);
		// sweep through all parameters
		for(int i = 0; i < rowCount(); i++) {
			// rename node
			QStringList parName = data(createIndex(i,0)).toString().split(".");
			const std::string parName0 = parName[0].toAscii().constData();
			const std::string parName1 = parName[1].toAscii().constData();
			Q_ASSERT(parName.size() > 0);
			if (parName[0] == nodename) {
				parName[0] = newName;
				setData(createIndex(i, 0), parName.join("."));
			}
			// rename target slots of other nodes
			else if (metaInfo()->isInputSlot(parName1, getClass(parName0))
				|| metaInfo()->isOutputSlot(parName1,getClass(parName0))) {
				QStringList parVals =
						data(createIndex(i,1)).toString().split(";");
				for (int j=0; j<parVals.size(); j++) {
					QStringList target = parVals[j].split(".");
					Q_ASSERT(target.size() > 0);
					if (target[0].isEmpty())
						continue;
					Q_ASSERT(target.size() == 2);
					if (target[0] == nodename) {
						target[0] = newName;
						parVals[j] = target.join(".");
					}
				}
				setData(createIndex(i,1), parVals.join(";"));
			}
		}
		setOnlyParams(true);
		setPrefix(newName, false);

		if(draw)
			emit graphChanged();

		emit statusMessage(tr("renamed node %1 to %2").arg(nodename)
						   .arg(newName));
	}
}

void GraphModel::deleteNode(const QString& nodename, bool draw) {
	QMessageBox mbox(QMessageBox::Question, tr("confirm delete"),
					 tr("Do you really want to delete node \"%1\"?").arg(nodename));
	mbox.addButton(QMessageBox::Yes);
	mbox.addButton(QMessageBox::No);
	mbox.setDefaultButton(QMessageBox::No);
	mbox.setEscapeButton(QMessageBox::No);
	mbox.exec();
	if(mbox.result() == QMessageBox::Yes) {
		disconnectAllSlots(nodename, false);
		setOnlyParams(false);
		setPrefix(nodename, false);
		removeRows(0, rowCount());
		setOnlyParams(true);

		if(draw)
			emit graphChanged();

		emit statusMessage(tr("delete node %1").arg(nodename));
	}
}


QStringList GraphModel::nodes() const {
	std::set<std::string> nodeSet;

	// detect objects
	std::vector<std::string> keys = parameterFile().getKeyList();
	for (uint i = 0; i < keys.size(); i++) {
		ulong pos = keys[i].find(".");
		if ((pos == std::string::npos) || (pos == 0))
			continue;
		nodeSet.insert(keys[i].substr(0, pos));
	}

	QStringList result;
	std::set<std::string>::const_iterator node;

	for(node = nodeSet.begin(); node != nodeSet.end(); node++)
		if(nodeValid(node->c_str()))
			result << node->c_str();

	return result;
}

void GraphModel::selectNext(bool back, bool draw) {
	QStringList curNodes = nodes();

	// skip, if no nodes to select avaiable
	if (!curNodes.size())
		return;

	int pos = 0;

	if(prefix().isEmpty())
		pos = 1;
	else {
		pos = curNodes.indexOf(prefix());
	}

	if (back)
		pos = (pos - 1 + curNodes.size()) % curNodes.size();
	else
		pos = (pos + 1) % curNodes.size();

	setPrefix(curNodes[pos], draw);
}

QString GraphModel::addNode(const QString& className, bool draw) {
	// new name input and check if valid
	QString newName, info;
	do {
		bool ok = false;
		newName = QInputDialog::getText(0, tr("add new node"),
										info + tr("Enter a name for the new node:"),
										QLineEdit::Normal, tr("newnode"), &ok);
		if(!ok)
			return "";
		if(nodeValid(newName))
			info = tr("This name is already in use.\n"
					  "Please use another name.\n");
		if(newName.contains(QRegExp("[\\s.]")))
			info = tr("Whitespace and dots in names are not allowed.\n"
					  "Please use a valid name.\n");
	} while (newName.isEmpty()
		|| nodeValid(newName)
		|| newName.contains(QRegExp("\\s"))
		);

	setPrefix("", false);
	setOnlyParams(false);

	std::vector<std::string> inputs =
			metaInfo()->getInputs(className.toAscii().constData());
	std::vector<std::string> outputs =
			metaInfo()->getOutputs(className.toAscii().constData());

	insertRow(rowCount());
	setData(index(rowCount()-1, 0), newName + ".type");
	setData(index(rowCount()-1, 1), className);
	setPrefix(newName, false);
	setOnlyParams(true);

	emit statusMessage(tr("add node %1 of class %2").arg(newName)
					   .arg(className));

	if(draw)
		emit graphChanged();
	return newName;
}

bool GraphModel::setData(const QModelIndex& ind, const QVariant& value,
						 int role) {
	if ((ind.column() == 1) && (data(index(ind.row(), 0)) == "templatetype")) {
		if ((role != Qt::DisplayRole) && (role != Qt::EditRole))
			return false;

		if (value == data(ind))
			return ParameterFileModel::setData(ind, value, role);
		bool res = ParameterFileModel::setData(ind, value, role);

		QStringList l = _connections(prefix());
		for (int i = 0; i < l.size(); i++) {
			QStringList connection = l[i].split(";");
			std::string slotType = metaInfo()->getType(connection[0].toAscii().constData(),
													   getClass(prefix().toAscii().constData()));
			if (StringTool::toLowerCase(slotType).find("<t>")) {
				disconnectSlot(prefix() + "." + connection[0], connection[1], false);
			}
		}
		reDraw();
		return res;
	}
	return ParameterFileModel::setData(ind, value, role);
}

bool GraphModel::removeRows(int row, int count,
		const QModelIndex& parentInd) {
	bool res = ParameterFileModel::removeRows(row, count, parentInd);
	if(rowCount() > 0 && data(index(row,0)) == "templatetype")
		reDraw();
	return res;
}
