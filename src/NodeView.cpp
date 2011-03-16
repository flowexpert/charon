/*  Copyright (C) 2011 Jonathan Wuest

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
/** \file   NodeView.cpp
 *  \brief  Implementation of class NodeView
 *  \date   15.03.2011
 *  \author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "NodeView.h"
#include "GraphModel.h"
#include "FileManager.h"
#include <QMessageBox>

NodeView::NodeView(QWidget* p,QString classesFile) :
		QGraphicsView(p),_model(0) {
	setRenderHints( QPainter::Antialiasing );
	setMinimumSize(500,500);
	setAcceptDrops(true);
	show();
	nodehandler = new NodeHandler(this,classesFile);
	setScene(this->nodehandler);
}

GraphModel *NodeView::model(){
	return nodehandler->model();
}

bool NodeView::load(QString fname){
	QString errorMSG;
	bool ok = true;
	try {
		ok = nodehandler->load(fname);
	}
	catch (const std::string& msg) {
		errorMSG = msg.c_str();
	}
	catch (const std::exception& err) {
		errorMSG = err.what();
	}
	catch (...) {
		errorMSG = tr("Got unhandled and unexpected exception during load!");
	}
	if (!errorMSG.isEmpty()) {
		if (errorMSG.indexOf("neither input nor output slot") >= 0) {
			errorMSG += tr(
				"<br><br>"
				"This is usually a hint "
				"that there is a mismatch of your parameter file "
				"and the actual plugin information. "
				"This may be caused by changed slots or typos in the "
				"parameter file.<br><br>"
				"Please update the plugin information and check if all "
				"plugins used in the parameter file to load have been "
				"successfully recognized.<br>"
				"Make sure that the path where the plugin is loaded from "
				"is the path you expect. Perhaps an older version of the "
				"plugin has been found in the global plugin path or "
				"something similar happened."
			);
		}
		QRegExp regex("^Parameter\\s(\\w*)\\.parameters\\snot\\sset\\s*$");
		if (errorMSG.indexOf(regex) >= 0) {
			errorMSG += tr(
				"<br><br>"
				"This is usually a hint that you are trying to use a "
				"plugin unknown to tuchulcha (<b>%1</b>).<br><br>"
				"Please check your path configuration and "
				"update the plugin information.<br>"
				"Check the update log and make sure the <em>%1</em> plugin "
				"has been successfully found and loaded."
			).arg(regex.cap(1));
		}
		QMessageBox::warning(
			this,"Error loading parameter file",
			tr("Message: %1").arg(errorMSG));
		return false;
	}
	return ok;
}

void NodeView::setModel(GraphModel* md){
	nodehandler->setModel(md);
}

void NodeView::save(){
}

void NodeView::updateDisplay(){
	update();
}

NodeView::~NodeView() {
}

#include "NodeView.moc"
