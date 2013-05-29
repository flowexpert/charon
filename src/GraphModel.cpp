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
	// assert that source is an input slot
	if (!isInputSlot(source)) {
		qSwap(source,target);
	}
	if (!isInputSlot(source)) {
		throw std::runtime_error(
				tr("At least one of (%1,%2) has to be an input slot!")
					.arg(source).arg(target).toStdString());
	}

	// check if target is of the corresponding slot type (input <-> output)
	if (!isOutputSlot(target)) {
		throw std::runtime_error(
			tr("%1 has to be an output slot!").arg(target).toStdString());
	}

	// check slot types
	QString inSlotType  = getType(target);
	QString outSlotType = getType(source);
	if ((inSlotType.toLower() != outSlotType.toLower())&&!(inSlotType.toLower()=="virtual"||outSlotType.toLower()=="virtual"))
		throw std::runtime_error(
				tr("Type of \"%1\" (%2) does not match type of \"%3\" (%4)")
				.arg(target).arg(inSlotType).arg(source).arg(outSlotType)
				.toStdString());

	bool established =
			getValue(source).contains(QRegExp(target,Qt::CaseInsensitive));

	// check if target node is in input/ouput list of source
	if (established &&
			!(getValue(target).contains(QRegExp(source,Qt::CaseInsensitive)))) {
		throw std::runtime_error(
			tr("Node %1 missing in List %2!")
				.arg(source).arg(target).toStdString());
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
    if(!isInputSlot(source)) {
		// swap source and target
		qSwap(source,target);
	}

	// disconnect input slot, if assigned and not multi slot
    if (!isMultiSlot(source)) {
		QString val = getValue(source);
		if (!val.isEmpty())
			disconnectSlot(source, val, false);
	}

	// add target to source
	QString content = getValue(source);
	Q_ASSERT(content.indexOf(target,Qt::CaseInsensitive) < 0);
	QStringList targetList = content.split(";", QString::SkipEmptyParts);
	// add new target
	targetList << target;
	setValue(source, targetList.join(";"));

	// add source to target
	content = getValue(target);
	// this if instead of a strict Q_ASSERT
	// is a workaround to accept some old (buggy)
	// parameter files with left-over entries
	// within the output slot connections
	if (content.indexOf(source,Qt::CaseInsensitive) < 0) {
		QStringList sourceList = content.split(";", QString::SkipEmptyParts);
		// add new target
		sourceList << source;
		setValue(target, sourceList.join(";"));
	}

	if(draw)
		emit graphChanged();

	emit statusMessage(
			tr("connect slot %1 with %2").arg(source).arg(target));
}

void GraphModel::disconnectSlot(QString source, QString target, bool draw) {
	QString content = getValue(source).toLower();
	QStringList targets = content.split(";", QString::SkipEmptyParts);
	if (target.isEmpty()) {
		foreach (const QString& tar, targets) {
			setValue(source,QString());
			disconnectSlot(tar,source,false);
		}
	}
	else {
		int pos = targets.indexOf(target.toLower(),0);
		if (pos >= 0) {
			targets.removeAt(pos);
			setValue(source, targets.join(";"));
			disconnectSlot(target,source,false);
		}
	}

	if(draw)
		emit graphChanged();

	emit statusMessage(
			target.isEmpty() ?
				tr("disconnected slot %1 from all targets").arg(source) :
				tr("disconnected slot %1 from %2").arg(source).arg(target));
}

void GraphModel::disconnectAllSlots(QString node, bool draw) {
	node = node.section(".",0,0).toLower(); // get base name
	foreach (const QString& slot, getInputs(node)) {
		disconnectSlot(QString("%1.%2").arg(node).arg(slot),QString(),false);
	}
	foreach (const QString& slot, getOutputs(node)) {
		disconnectSlot(QString("%1.%2").arg(node).arg(slot),QString(),false);
	}

	if(draw)
		emit graphChanged();

	emit statusMessage(tr("disconnected all slots of node %1").arg(node));
}

void GraphModel::renameNode(QString nodename, bool draw) {
	nodename = nodename.section(".",0,0).toLower();
	bool ok;
	QString newName = QInputDialog::getText(
			0, tr("rename node"),
			tr("Enter new name for node \"%1\":").arg(nodename),
			QLineEdit::Normal, nodename, &ok);
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
		//renaming a parameter changes the list of keys and invalidates the indices
		//the easiest method is to restart iterating over the parameters every time
		//a match has been found
		int count = rowCount() ;
		for(int j = 0 ; j < count ; j++)
		{
			for(int i = 0 ; i < count ; i++) {
				// rename node
				QString curPar = data(index(i,0)).toString();
				if (curPar.startsWith(nodename+".",Qt::CaseInsensitive)) {
					QStringList parName = curPar.split(".");
					Q_ASSERT(parName.size() > 0);
					parName[0] = newName;
					setData(index(i, 0), parName.join("."));
				}
				// rename target slots of other nodes
				else if (isInputSlot(curPar) || isOutputSlot(curPar)) {
					QStringList parVals =
						getValue(curPar).split(";",QString::SkipEmptyParts);
					for (int k = 0; k < parVals.size(); k++) {
						QStringList target = parVals.at(k).split(".");
						Q_ASSERT(target.size() > 0);
						if (QString::compare(target[0],nodename,
								Qt::CaseInsensitive)==0) {
							target[0] = newName;
							parVals[k] = target.join(".");
						}
					}
					setValue(curPar, parVals.join(";"));
				}
			}
		}
		setOnlyParams(true);
		setPrefix(newName);

		if(draw)
			emit graphChanged();

		emit statusMessage(
				tr("renamed node %1 to %2").arg(nodename).arg(newName));
	}
}

bool GraphModel::deleteNode(QString nodename, bool draw) {
	if(nodeValid(nodename) && QMessageBox::question(
			0, tr("confirm delete"),
			tr("Do you really want to delete node \"%1\"?").arg(nodename),
			QMessageBox::No | QMessageBox::Yes, QMessageBox::No)
			== QMessageBox::Yes) {
		disconnectAllSlots(nodename, false);

		// select next node, so that erase just touches the parameter file
		// which is the most efficient way of deleting values
		if (QString::compare(nodename,prefix(),Qt::CaseInsensitive) == 0) {
			selectNext();
		}
		if (QString::compare(nodename,prefix(),Qt::CaseInsensitive) == 0) {
			// still same prefix
			// this is the case, if only one node left, so use old style code
			setOnlyParams(false);
			removeRows(0, rowCount());
			setOnlyParams(true);
			setPrefix("");
		}
		else {
			// erase all keys of this node.
			// since the next code has been selected, all of the
			// parameters are currently not visible in the model, so
			// erase can just remove them without data change in the view
			QStringList toDelete = parameterFile().getKeyList(nodename+".");
			foreach (const QString& cur, toDelete) {
				erase(cur);
			}
		}

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
	result.sort();

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
	QString info, newName, baseName = className.toLower();
	QRegExp cNameCheck("([\\w]+)[\\s\\.]+.*");
	if (cNameCheck.exactMatch(baseName)) {
		baseName = cNameCheck.cap(1).toLower();
		Q_ASSERT(!baseName.isEmpty());
	}
	bool retry = false;

	// loop until a valid name is found or add node canceled
	do {
		retry = false;
		// generate new unique name based on the class name
		int nameNr = 0;
		do {
			newName = QString("%1%2").arg(baseName).arg(++nameNr);
		} while (nodeValid(newName));

		// ask user
		newName = QInputDialog::getText(
				0, tr("add new node"),
				info + tr("Enter a name for the new node:"),
				QLineEdit::Normal, newName).trimmed();

		if (newName.isEmpty()) {
			return QString();  // cancel adding node
		}

		// convert to lowercase to prevent problems later
		newName = newName.toLower() ;

		// check instance name rules
		if( nodeValid(newName) ) {
			retry = true;
			info = tr("This name is already in use.") + "\n"
					+ tr("Please use another name.") + "\n";
		}
		if( cNameCheck.exactMatch(newName) ) {
			retry = true;
			info = tr("Whitespace and dots in names are not allowed.") + "\n"
					+ tr("Please use a valid name.") + "\n";
			baseName = cNameCheck.cap(1).toLower();
		}
	} while (retry);

	setValue(newName+".type", className);

	emit statusMessage(
			tr("add node %1 of class %2").arg(newName).arg(className));
	if(draw)
		emit graphChanged();

	return newName;
}

bool GraphModel::setData(
		const QModelIndex& ind, const QVariant& value, int role) {
	QString param = data(index(ind.row(),0)).toString();
	if (!prefix().isEmpty()) {
		Q_ASSERT(prefixValid());
		param = prefix() + "." + param;
	}
	QRegExp ttype("(.*\\.)?templatetype",Qt::CaseInsensitive);
	if (
			(role == Qt::DisplayRole || role == Qt::EditRole) &&
			ind.column() == 1 &&
			value != data(ind) &&
			ttype.exactMatch(param)) {

		// disconnect slots on template type change, if neccessary
		QString node = param.section(".",0,0).toLower();
		QStringList allSlots;
		allSlots << getInputs(node) << getOutputs(node);
		foreach (const QString& slot, allSlots) {
			QString slotName = QString("%1.%2").arg(node).arg(slot);
			QString slotTypeR = getType(slotName, true);   // tmpltype applied
			QString slotTypeT = getType(slotName, false);  // plain type
			if (slotTypeR != slotTypeT) {
				// slot type depends on template type
				disconnectSlot(slotName, QString(), false);
			}
		}
		emit graphChanged();
	}
	return ParameterFileModel::setData(ind, value, role);
}

bool GraphModel::removeRows(int row, int count,
		const QModelIndex& parentInd) {
	QRegExp ttype("(.*\\.)?templatetype",Qt::CaseInsensitive);

	// Check if template type is about to be removed
	// and reset it to it's default value before deletion.
	// This handles proper disconnection on reset.
	for (int i = row; i < row+count; i++) {
		QString cur = data(index(row,0)).toString();
		if (!prefix().isEmpty()) {
			cur = prefix() + "." + cur;
		}
		if (ttype.exactMatch(cur)) {
			setData(index(row,1),getDefault(cur));
		}
	}

	return ParameterFileModel::removeRows(row, count, parentInd);
}

