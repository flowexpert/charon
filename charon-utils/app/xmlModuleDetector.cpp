/*  Copyright (C) 2011 Jens-Malte Gottfried

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file xmlModuleDetector.hxx
 *  Helper application to detect charon modules in doxygen generated xml
 *  files.
 *  \author <a href="mailto:jmgottfried@web.de>Jens-Malte Gottfried</a>
 *  \date 17.10.2011
 */
#include <QtXml>
#include <stdexcept>
#include <QTextStream>

QString docDir = ".";

bool checkModule(QString refid) {
	QDomDocument doc(refid);
	QString fName = QString("%1/%2.xml").arg(docDir).arg(refid);
	QFile file(fName);
	if (!file.open(QIODevice::ReadOnly))
		throw std::runtime_error("failed to open file "+fName.toStdString());
	if (!doc.setContent(&file)) {
		file.close();
		throw std::runtime_error("xml parse error: "+fName.toStdString());
	}
	file.close();

	const QDomElement& def = doc.documentElement()
			.firstChildElement("compounddef");
	Q_ASSERT(def.attribute("id") == refid);
	const QDomElement& inh = def.firstChildElement("inheritancegraph");
	if (inh.isNull()) {
		return false; // no inheritance
	}

	// get inheritance graph id
	QString nodeid;
	for(QDomElement cur = inh.firstChildElement("node");
			!cur.isNull(); cur = cur.nextSiblingElement("node")) {
		const QDomElement& linkElem = cur.firstChildElement("link");
		if (linkElem.isNull()) {
			// no link attribute
			break;
		}
		QString linkId = linkElem.attribute("refid");
		if (linkId == refid) {
			nodeid = cur.attribute("id");
			break;
		}
	}
	if (nodeid.isEmpty()) {
		// node not found, possibly no link attributes present
		// like in STL classes or classes without included tag file
		return false;
	}

	// roll back inheritance to find ParameteredObject
	// using a stack to allow multiple inheritance
	QStack<QString> ancestors;
	ancestors.push(nodeid);
	while (!ancestors.isEmpty()) {
		QString curId = ancestors.pop();
		for(QDomElement cur = inh.firstChildElement("node");
				!cur.isNull(); cur = cur.nextSiblingElement("node")) {
			QString chId = cur.attribute("id");
			if (chId == curId) {
				// check if we reached ParameteredObject
				if (cur.firstChildElement("label").text() ==
						"ParameteredObject") {
					return true;
				}
				// proceed adding ancestors
				for(QDomElement chd = cur.firstChildElement("childnode");
						!chd.isNull();chd=chd.nextSiblingElement("childnode")) {
					if(chd.attribute("relation") == "public-inheritance") {
						ancestors.push(chd.attribute("refid"));
					}
				}
				break;
			}
		}
	}
	return false;
}

QMap<QString,QString> groupContent(QString groupId) {
	QDomDocument doc(groupId);
	QString fName = QString("%1/%2.xml").arg(docDir).arg(groupId);
	QFile file(fName);
	if (!file.open(QIODevice::ReadOnly))
		throw std::runtime_error("failed to open file "+fName.toStdString());
	if (!doc.setContent(&file)) {
		file.close();
		throw std::runtime_error("xml parse error: "+fName.toStdString());
	}
	file.close();

	const QDomElement& def = doc.documentElement()
			.firstChildElement("compounddef");
	Q_ASSERT(def.attribute("id") == groupId);

	QMap<QString,QString> result;
	for (QDomElement cur = def.firstChildElement("innerclass");
			!cur.isNull(); cur = cur.nextSiblingElement("innerclass")) {
		result.insert(cur.attribute("refid"),cur.text());
	}

	return result;
}

int main(int argc, char** argv) {
	QTextStream qout(stdout), qerr(stderr);
	if (argc < 2) {
		qout << "Usage: " << argv[0] << " <DocXmlDir> [options]\n";
		qout << "\twith options: --ids, --mod-index, --xcheck\n";
		return -2;
	}
	docDir = QDir(argv[1]).absolutePath();
	if (!QDir(docDir).exists("index.xml")) {
		qerr << "No File index.xml found!\n";
		return -1;
	}
	QDomDocument doc("doxygenindex");
	QFile file(QString("%1/index.xml").arg(docDir));
	if (!file.open(QIODevice::ReadOnly))
		return -1;
	if (!doc.setContent(&file)) {
		file.close();
		return -1;
	}
	file.close();

	QStringList others;
	QMap<QString,QString> modules, modGrp;

	QDomNodeList nl = doc.elementsByTagName("compound");

	// find charon-modules group
	QString grpId;
	for (uint i=0; i < nl.length(); i++) {
		const QDomNode& n = nl.item(i);
		if (n.isElement() && n.toElement().attribute("kind") == "group") {
			QString refId = n.toElement().attribute("refid");
			QString name = n.firstChildElement("name").text();
			if (name == "charon-modules") {
				grpId = refId;
			}
		}
	}
	if (grpId.isEmpty()) {
		qerr << "charon-modules group not found!\n";
	}
	else {
		modGrp = groupContent(grpId);
	}

	// analyze classes
	for (uint i=0; i < nl.length(); i++) {
		const QDomNode& n = nl.item(i);
		if (n.isElement() && n.toElement().attribute("kind") == "class") {
			QString refId = n.toElement().attribute("refid");
			QString name = n.firstChildElement("name").text();
			if (checkModule(refId)) {
				modules.insert(refId, name);
			}
			else {
				others << name;
			}
		}
	}

	if(argc <= 2) {
		if(modules.size() > 0) {
			QMapIterator<QString, QString> modIter(modules);
			qout << "Charon-Modules:\n";
			while (modIter.hasNext()) {
				qout << "\t" << modIter.next().value() << "\n";
			}
		}
		if (others.size() > 0) {
			qout << "Non-Module Classes:\n";
			for (int i=0; i < others.size(); i++) {
				qout << "\t" << others.at(i) << "\n";
			}
		}
	}
	else {
		QString opt2 = argv[2];
		if (opt2 == "--xcheck") {
			QStringList notInModules;
			QMapIterator<QString, QString> grpIter(modGrp);
			while (grpIter.hasNext()) {
				QString curId = grpIter.next().key();
				if(modules.contains(curId)) {
					modules.remove(curId);
				}
				else {
					notInModules << grpIter.value();
				}
			}
			if (notInModules.size() > 0) {
				qout << "In charon-modules group but no ParameteredObject:\n";
				QStringListIterator mIter(notInModules);
				while (mIter.hasNext()) {
					qout << "\t" << mIter.next() << "\n";
				}
			}
			if (modules.size() > 0) {
				qout << "ParameteredObjects but not in charon modules group:\n";
				QMapIterator<QString, QString> modIter(modules);
				while (modIter.hasNext()) {
					qout << "\t" << modIter.next().value() << "\n";
				}
			}
		}
		else if (opt2 == "--ids") {
			QMapIterator<QString, QString> modIter(modules);
			while (modIter.hasNext()) {
				modIter.next();
				qout << "\t" << modIter.key() << "\n";
			}
		}
		else if (opt2 == "--mod-index") {
			QDomElement base = doc.documentElement();
			QDomElement cur = base.firstChildElement("compound");
			while(!cur.isNull()) {
				if (cur.attribute("kind") != "class" ||
							!modules.contains(cur.attribute("refid"))) {
					QDomElement old = cur;
					cur=cur.nextSiblingElement("compound");
					base.removeChild(old);
				}
				else {
					cur=cur.nextSiblingElement("compound");
				}
			}
			doc.save(qout,2);
		}
	}

	return 0;
}
