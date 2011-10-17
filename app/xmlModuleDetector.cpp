/*  Copyright (C) 2011 Jens-Malte Gottfried

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
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
		QString linkId = cur.firstChildElement("link").attribute("refid");
		if (linkId == refid) {
			nodeid = cur.attribute("id");
			break;
		}
	}
	Q_ASSERT(!nodeid.isEmpty());

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

int main(int argc, char** argv) {
	QTextStream qout(stdout), qerr(stderr);
	if (argc < 2) {
		qout << "Usage: " << argv[0] << " <DocXmlDir> [options]\n";
		qout << "\twith options: --ids, --mod-index\n";
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

	QStringList modules, modIds, others;

	QDomNodeList nl = doc.elementsByTagName("compound");
	for (uint i=0; i < nl.length(); i++) {
		const QDomNode& n = nl.item(i);
		if (n.isElement() && n.toElement().attribute("kind") == "class") {
			if (checkModule(n.toElement().attribute("refid"))) {
				modules << n.firstChildElement("name").text();
				modIds << n.toElement().attribute("refid");
			}
			else {
				others << n.firstChildElement("name").text();
			}
		}
	}

	if(argc <= 2) {
		qout << "Charon-Modules:\n";
		for (int i=0; i < modules.size(); i++) {
			qout << "\t" << modules.at(i) << "\n";
		}
		qout << "Non-Module Classes:\n";
		for (int i=0; i < others.size(); i++) {
			qout << "\t" << others.at(i) << "\n";
		}
	}
	else if (std::string(argv[2]) == "--ids") {
		for (int i=0; i < modIds.size(); i++) {
			qout << modIds.at(i) << "\n";
		}
	}
	else if (std::string(argv[2]) == "--mod-index") {
		QDomElement base = doc.documentElement();
		QDomElement cur = base.firstChildElement("compound");
		while(!cur.isNull()) {
			if (cur.attribute("kind") != "class" ||
						modIds.indexOf(cur.attribute("refid")) < 0) {
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

	return 0;
}
