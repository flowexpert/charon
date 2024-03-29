/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file   MetaData.cpp
 *  @brief  Implementation of class ParameterFileModel.
 *  @date   10.10.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#include "MetaData.h"
#include "QParameterFile.h"
#include <QSet>
#include <QVariant>

MetaData::MetaData(QString fileName) :
		_data(new QParameterFile(fileName)) {
	Q_ASSERT(!fileName.isEmpty());
	QStringList keyList = _data->getKeyList();
	 // contains all keys in _metadata
	QSet<QString> keys = QSet<QString>::fromList(keyList);
	QSet<QString> classes; // contains all classes in _metadata
	QSet<QString>::const_iterator keyIter;

	// each key needs a separator (classes cannot have a default value)
	for(keyIter = keys.begin(); keyIter != keys.end(); keyIter++) {
		int pos = keyIter->indexOf(".");
		if (pos < 0)
			qFatal("Invalid key (separator missing): %s",
					keyIter->toLocal8Bit().constData());
		classes.insert(keyIter->left(pos));
	}

	// create/update class vector
	_classes = classes.toList();
}

MetaData::~MetaData() {
	if(_data)
		delete _data;
}

QStringList MetaData::getClasses() const {
	return _classes;
}

QStringList MetaData::getOutputs (QString className) const {
	return _data->get(className + ".outputs")
			.split(";",QString::SkipEmptyParts);

}

QStringList MetaData::getInputs(QString className) const {

	return _data->get(className + ".inputs")
			.split(";",QString::SkipEmptyParts);

}

QStringList MetaData::getParameters(QString className) const {
	return _data->get(className + ".parameters")
			.split(";",QString::SkipEmptyParts);
}

QString MetaData::getType(QString parName, QString className) const {
	parName = parName.section(".",-1,-1).toLower();
	QString par = className + "." + parName + ".type";
	QString result = _data->get(par);
	if(result.isEmpty())
		result = "string";
	return result;
}

QString MetaData::getDefault(QString parName, QString className) const {
	parName =  parName.section(".",-1,-1).toLower();
	QString par = className + "." + parName;
	QString result = _data->get(par);
	return result;
}

QString MetaData::getDocString(QString parName, QString className) const {
	parName =  parName.section(".",-1,-1).toLower();
	QString par =  parName.isEmpty() ?
			className + ".doc" :
			className + "." + parName + ".doc";
	QString result = _data->get(par);
	return result;
}

QString MetaData::getDocFile(QString parName, QString className) const {
	parName =  parName.section(".",-1,-1).toLower();
	QString par = parName.isEmpty() ?
			className + ".docfile" :
			className + "." + parName + ".docfile";
	QString result = _data->get(par);
	return result;
}

bool MetaData::isInputSlot(QString name, QString className) const {
	name =  name.section(".",-1,-1).toLower();
	QStringList list = getInputs(className);
	return (list.indexOf(QRegExp(name,Qt::CaseInsensitive)) >= 0);
}

bool MetaData::isOptionalSlot(QString slotName, QString className) const {
	slotName = slotName.section(".",-1,-1).toLower();
	bool slotIsIn = isInputSlot(slotName, className);
	QString optName = className + "." + slotName + ".optional";
	return _data->isSet(optName) ?
			QVariant(_data->get(optName)).toBool() : !slotIsIn;
}

bool MetaData::isMultiSlot(QString slotName, QString className) const {
	slotName = slotName.section(".",-1,-1).toLower();
	bool slotIsIn = isInputSlot(slotName, className);
	QString optName = className + "." + slotName + ".multi";
	return _data->isSet(optName) ?
			QVariant(_data->get(optName)).toBool() : !slotIsIn;
}

bool MetaData::isDynamic(QString className) const {
	return QVariant(_data->get(className + ".isDynamicModule")).toBool();
}

QStringList MetaData::getTags(QString className) const {
	QString result = _data->get(className + ".tags") ;
	return result.split(";", QString::SkipEmptyParts);
}

QStringList MetaData::getInputDisplayNames(QString className) const
{
	QStringList inputs= _data->get(className + ".inputs")
			.split(";",QString::SkipEmptyParts);
	QStringList res;
	foreach(const QString& inp , inputs)
	{
		QString disp=_data->get(className +"."+inp+".displayname");

		if(disp!=QString::null)
			res.push_back(disp);
		else
			res.push_back(inp);
	}
	return res;
}

QStringList MetaData::getOutputDisplayNames(QString className) const
{
	QStringList outputs= _data->get(className + ".outputs")
			.split(";",QString::SkipEmptyParts);
	QStringList res;
	foreach(const QString& outp , outputs)
	{
		QString disp=_data->get(className +"."+outp+".displayname");

		if(disp!=QString::null)
			res.push_back(disp);
		else
			res.push_back(outp);
	}
	return res;
}
