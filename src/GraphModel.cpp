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
#include "QParameterFile.h"
#include "MetaData.h"
#include <QMessageBox>
#include <QtDebug>
#include <sstream>
#include <set>
#include <algorithm>
#include <QInputDialog>
#include <stdexcept>

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
	if (metaFile.isEmpty()) {
		qFatal("No metaFile in GraphModel given! Pleas specify one!");
	}
	if (!useMetaInfo()) {
		qFatal("MetaInfo not usable!");
	}

	connect(
			this, SIGNAL(dynamicUpdate()),
			this, SIGNAL(graphChanged()),
			Qt::QueuedConnection);
}

void GraphModel::clear(bool draw) {
	setPrefix("");
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

bool GraphModel::_load() {
	if (!ParameterFileModel::_load()) {
		return false;
	}

	Q_ASSERT(useMetaInfo());

	// check which classes are used in the workflow
	QSet<QString> wClasses;
	const QStringList& nds = nodes();
	foreach (const QString& cur, nds) {
		wClasses << getClass(cur);
	}

	// collect unknown classes
	QStringList uClasses;
	const QStringList& knownClasses = getClasses();
	foreach(const QString& s, wClasses) {
		if (!knownClasses.contains(s,Qt::CaseInsensitive)) {
			uClasses << s;
		}
	}

	if (!uClasses.isEmpty()) {
		QMessageBox::warning(
			0,tr("missing modules"),
			tr("The following classes are used in the workflow "
				"but are unknown to Tuchulcha:")
			+QString("<ul><li>%1</li></ul>").arg(uClasses.join("</li><li>"))
			+tr("Please check your plugin path settings, "
				"then update plugin informations "
				"and look if these plugins are found at all "
				"or error messages related with tese plugins occur."
			));
		return false;
	}

	emit graphChanged();
	selectNext();

	return true;
}

bool GraphModel::nodeValid(const QString& name) const {
	return !getClass(name).isEmpty();
}

bool GraphModel::connected(
			QString source, QString target) const {
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
	QString sourceSlot = sourceSep[1];
	QString targetSlot = targetSep[1];

	QStringList sourceOutputs = getOutputs(source);
	QStringList sourceInputs  = getInputs(source);

	// source has to be an input/output
	bool sourceIsOutput = true;

	if(sourceOutputs.indexOf(QRegExp(sourceSlot,Qt::CaseInsensitive))<0)
		sourceIsOutput = false;

	if(!sourceIsOutput) {
		if(sourceInputs.indexOf(QRegExp(sourceSlot, Qt::CaseInsensitive))<0)
			throw std::runtime_error(
					"source " + source.toStdString()
					+ " is neither input nor output slot!");
	}

	// check if target is of the corresponding slot type (input <-> output)
	QStringList targetOutputs = getOutputs(target);
	QStringList targetInputs  = getInputs(target);

	if (sourceIsOutput) {
		if(targetInputs.indexOf(QRegExp(targetSlot,Qt::CaseInsensitive))<0)
			throw std::runtime_error(
					target.toStdString() + " has to be an input!");
	}
	else {
		if(targetOutputs.indexOf(QRegExp(targetSlot,Qt::CaseInsensitive))<0)
			throw std::runtime_error(
					target.toStdString() + " has to be an output!");
	}

	// check slot types
	QString inSlotType  = getType(target);
	QString outSlotType = getType(source);
	if (inSlotType != outSlotType)
		throw std::runtime_error(
				"Type of \"" + target.toStdString()
				+ "\" (" + inSlotType.toStdString()
				+ ") does not match type of \""
				+ source.toStdString() + "\" ("
				+ outSlotType.toStdString() + ")");

	bool established = true;

	if(!parameterFile().isSet(source))
		established = false;
	else {
		QString outList = parameterFile().get(source);
		if (!outList.contains(QRegExp(target,Qt::CaseInsensitive)))
			established = false;
	}

	// check if target node is in input/ouput list of source
	if (!parameterFile().isSet(target)) {
		if (established)
			throw std::runtime_error(
					"Node " + source.toStdString() + " missing in List "
					+ target.toStdString() + "!");
	}
	else {
		QString inList = parameterFile().get(target);
		if (!inList.contains(QRegExp(source,Qt::CaseInsensitive))
				&& established)
			throw std::runtime_error(
					"Node " + source.toStdString() + " missing in List "
					+ target.toStdString() + "!");
	}

	return established;
}

void GraphModel::loadMetaFile(QString fName) {
	if(fName.isEmpty())
		qFatal("Tried to set emtpy metaFile in GraphModel!");
	ParameterFileModel::loadMetaInfo(fName);
}

void GraphModel::connectSlot(QString source, QString target, bool draw) {
	// check for valid connection and return, if this connection already
	// exists.
	if (connected(source, target))
		return;

	// identify input and output slot
	bool sourceIsIn = isInputSlot(source);
	if(!sourceIsIn) {
		// swap source and target
		qSwap(source,target);
	}

	// check slot types
	QString inSlotType  = getType(source);
	QString outSlotType = getType(target);
	if (inSlotType != outSlotType)
		throw std::runtime_error(
				"Type of \"" + source.toStdString()
				+ "\" (" + inSlotType.toStdString()
				+ ") does not match type of \""
				+ target.toStdString() + "\" ("
				+ outSlotType.toStdString() + ")");


	// disconnect input slot, if assigned and not multi slot
	if (!isMultiSlot(source)) {
		QString val;
		if (isSet(source))
			val = getValue(source);
		if (!val.isEmpty())
			disconnectSlot(source, val, false);
	}

	// add target to source
	if (isSet(source)) {
		QString content = getValue(source);
		Q_ASSERT(content.indexOf(target) < 0);
		QStringList targetList = content.split(
				";", QString::SkipEmptyParts);
		// add new target
		targetList << target;
		setValue(source, targetList.join(";"));
	}
	else {
		setValue(source, target);
	}

	// add source to target
	if (isSet(target)) {
		QString content = getValue(target);
		if (content.indexOf(source) < 0) {
			QStringList sourceList = content.split(
					";", QString::SkipEmptyParts);
			// add new target
			sourceList << source;
			setValue(target, sourceList.join(";"));
		}
	}
	else {
		setValue(target, source);
	}

	if(draw)
		emit graphChanged();

	emit statusMessage(
			tr("connect slot %1 with %2").arg(source).arg(target));
}

void GraphModel::disconnectSlot(QString source, QString target, bool draw) {
	if (isSet(source)) {
		// check target is in list
		QString content = getValue(source).toLower();
		QStringList targets = content.split(
				";", QString::SkipEmptyParts);
		int pos = targets.indexOf(target.toLower());
		if (pos >= 0) {
			targets.removeAt(pos);
			setValue(source, targets.join(";"));
		}
	}
	if (isSet(target)) {
		// check target is in list
		QString content = getValue(target).toLower();
		QStringList targets = content.split(";", QString::SkipEmptyParts);
		int pos = targets.indexOf(source.toLower());
		if (pos >= 0) {
			targets.removeAt(pos);
			setValue(target, targets.join(";"));
		}
	}

	if(draw)
		emit graphChanged();

	emit statusMessage(
			tr("disconnected node %1 from %2").arg(source).arg(target));
}

QStringList GraphModel::_connections(QString node) const {
	node = node.split(".")[0].toLower(); // get base name
	QString className = getClass(node);

	// collect input & output slots
	QStringList inputs  = getInputs (node);
	QStringList outputs = getOutputs(node);
	QStringList curSlot;

	QStringList::const_iterator slot;
	QStringList::const_iterator target;

	QStringList result;

	// collect inputs
	for (slot = inputs.begin(); slot != inputs.end(); slot++) {
		QString slotName = QString("%1.%2").arg(node).arg(*slot);
		if (!isSet(slotName))
			continue;
		curSlot = parameterFile().getList(slotName);
		// only multi slots can have more than one source!
		Q_ASSERT(isMultiSlot(slotName) || (curSlot.size() <= 1));
		QStringList::const_iterator curSlotIter;
		for(curSlotIter = curSlot.begin(); curSlotIter != curSlot.end();
		curSlotIter++)
			result << QString("%1;%2").arg(*slot).arg(*curSlotIter);
	}

	// collect outputs
	for (slot = outputs.begin(); slot != outputs.end(); slot++) {
		QString slotName = QString("%1.%2").arg(node).arg(*slot);
		if (!parameterFile().isSet(slotName))
			continue;
		curSlot = parameterFile().getList(slotName);
		if (curSlot.size() > 0)
			// number of targets of an output slot is unlimited
			for(target = curSlot.begin(); target != curSlot.end(); target++)
				result << QString("%1;%2").arg(*slot).arg(*target);
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
	//parameter file does only support lowercase and handling is inconsistent
	//convert to lowercase to prevent problems later
	newName = newName.toLower() ;
	if (ok) {
		if(nodeValid(newName)) {
			QMessageBox::warning(
					0, tr("node exists"),
					tr("A node named <i>%1</i> does already exist.<br/>"
					   "Please choose another name.").arg(newName));
			return;
		}
		setPrefix("");
		setOnlyParams(false);
		// sweep through all parameters
		for(int i = 0; i < rowCount(); i++) {
			// rename node
			QStringList parName = data(createIndex(i,0)).toString().split(".");
			Q_ASSERT(parName.size() > 0);
			if (parName[0] == nodename) {
				parName[0] = newName;
				setData(createIndex(i, 0), parName.join("."));
			}
			// rename target slots of other nodes
			else if (isInputSlot(parName.join("."))
					|| isOutputSlot(parName.join("."))) {
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
		setPrefix(newName);

		if(draw)
			emit graphChanged();

		emit statusMessage(tr("renamed node %1 to %2").arg(nodename)
						   .arg(newName));
	}
}

bool GraphModel::deleteNode(QString nodename, bool draw) {
	if(QMessageBox::question(
			0, tr("confirm delete"),
			tr("Do you really want to delete node \"%1\"?").arg(nodename),
			QMessageBox::No | QMessageBox::Yes, QMessageBox::No)
			== QMessageBox::Yes) {
		disconnectAllSlots(nodename, false);
		setOnlyParams(false);
		setPrefix(nodename);
		removeRows(0, rowCount());
		setOnlyParams(true);

		if(draw)
			emit graphChanged();

		emit statusMessage(tr("delete node %1").arg(nodename));
		return true;
	}
	return false;
}


QStringList GraphModel::nodes() const {
	QSet<QString> nodeSet;

	// detect objects
	QStringList keys = parameterFile().getKeyList();
	foreach (const QString& ckey, keys) {
		nodeSet << ckey.section(".",0,0);
	}

	QStringList result;
	foreach(const QString& node, nodeSet) {
		if(nodeValid(node))
			result << node;
	}

	return result;
}

void GraphModel::selectNext(bool back) {
	const QStringList& curNodes = nodes();

	// skip, if no nodes to select avaiable
	if (!curNodes.size())
		return;

	int pos = 0;

	if(!prefix().isEmpty() && prefixValid()) {
		pos = curNodes.indexOf(prefix());
		Q_ASSERT(pos >= 0);
		if (back) {
			pos = (pos - 1 + curNodes.size()) % curNodes.size();
		}
		else {
			pos = (pos + 1) % curNodes.size();
		}
	}

	setPrefix(curNodes[pos]);
}

QString GraphModel::addNode(QString className, bool draw) {
	// new name input and check if valid
	static int nameNr = 0 ;
	QString newName = QString("newnode%1").arg(nameNr++) ;
	QString	info;
	bool ok ;
	do {
		ok = false;
		newName = QInputDialog::getText(
				0, tr("add new node"),
				info + tr("Enter a name for the new node:"),
				QLineEdit::Normal, newName, &ok);
		if(!ok)
			return "";
		//parameter file does only support lowercase and handling is inconsistent
		//convert to lowercase to prevent problems later
		newName = newName.toLower() ;
		if( (ok = nodeValid(newName)) ) {
			info = tr("This name is already in use.") + "\n"
					+ tr("Please use another name.") + "\n";
		}
		if(newName.contains(QRegExp("[\\s\\.]")))
		{	
			ok = true ;
			info = tr("Whitespace and dots in names are not allowed.") + "\n"
					+ tr("Please use a valid name.") + "\n";
		}
	} while (newName.isEmpty() || ok) ;

	setPrefix("");
	setOnlyParams(false);


	insertRow(rowCount());
	setData(index(rowCount()-1, 0), newName + ".type");
	setData(index(rowCount()-1, 1), className);
	setPrefix(newName);
	setOnlyParams(true);

	emit statusMessage(
			tr("add node %1 of class %2").arg(newName).arg(className));
	if(draw)
		emit graphChanged();
	return newName;
}

bool GraphModel::setData(const QModelIndex& ind, const QVariant& value,
						 int role) {
	if (ind.column() == 1) {
		if (value == data(ind))
			return ParameterFileModel::setData(ind, value, role);
		if (data(index(ind.row(), 0)) == "templatetype") {
			if ((role != Qt::DisplayRole) && (role != Qt::EditRole))
				return false;

			bool res = ParameterFileModel::setData(ind, value, role);

			QStringList l = _connections(prefix());
			for (int i = 0; i < l.size(); i++) {
				QStringList connection = l[i].split(";");
				QString slotNameC = prefix()+"."+connection[0];
				QString slotTypeR = getType(slotNameC, true);
				QString slotTypeT = getType(slotNameC, false);
				if (slotTypeR != slotTypeT) {
					disconnectSlot(
						prefix()+"."+connection[0],connection[1], false);
				}
			}
			reDraw();
			return res;
		}
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

