/*	Copyright (C) 2011 Jonathan Wuest

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
/** @file   NodeView.cpp
 *  @brief  Implementation of class NodeView
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */
#include <iostream>
#include "NodeView.h"
#include "GraphModel.h"
#include "FileManager.h"

#include "NodeView.moc"

using namespace std;

NodeView::NodeView(QWidget *parent,QString classesFile) : QGraphicsView(parent),_model(0) {
	this->setRenderHints( QPainter::Antialiasing );
	this->setAcceptDrops(true);
	this->show();
	this->_nodehandler = new NodeHandler(this,classesFile);
	this->setScene(this->_nodehandler);
}

GraphModel *NodeView::model(){
	return _nodehandler->model();
}

bool NodeView::load(const QString& fileName){
	bool ret = this->_nodehandler->load(fileName);
	fitInView(sceneRect(),Qt::KeepAspectRatio);
	return ret;
}

void NodeView::setModel(GraphModel *model){
	this->_nodehandler->setModel(model);
}

void NodeView::save(){
}
void NodeView::updateDisplay(){
	_nodehandler->update();
}

NodeView::~NodeView() {
}

