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
/**	@file ModelToolBar.cpp
 *	@brief Implementation of class ModelToolBar
 *
 *	@date	28.11.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
 
#include <QAction>

#include "ModelToolBar.h"
#include "GraphModel.h"

ModelToolBar::ModelToolBar(const QString& title, QWidget* myParent) :
		QToolBar(title, myParent),
		_model(0) {
}

ModelToolBar::~ModelToolBar() {
}

void ModelToolBar::addModelActions() {
	QAction* act;
	act = addAction(
		QIcon::fromTheme("edit-delete",QIcon(":/icons/edit-delete.png")),
		tr("delete node"), this, SLOT(_delete()));
	act->setToolTip(tr("delete selected node"));
	
	act = addAction(
		QIcon::fromTheme("edit-rename",QIcon(":/icons/edit-rename.png")),
		tr("rename node"), this, SLOT(_rename()));
	act->setToolTip(tr("rename selected node"));
	
	act = addAction(
		QIcon::fromTheme("network-disconnect",QIcon(":/icons/network-disconnect.png")),
		tr("disconnect node"), this, SLOT(_disconnect()));
	act->setToolTip(tr("disconnect all slots of selected node"));
}

void ModelToolBar::setModel(ParameterFileModel* model) {
	_model = qobject_cast<GraphModel*>(model);
}

void ModelToolBar::_delete() {
	if (_model && !_model->prefix().isEmpty() && _model->prefixValid())
		_model->deleteNode(_model->prefix());
}
	
void ModelToolBar::_rename() {
	if (_model && !_model->prefix().isEmpty() && _model->prefixValid())
		_model->renameNode(_model->prefix());
}

void ModelToolBar::_disconnect() {
	if (_model && !_model->prefix().isEmpty() && _model->prefixValid())
		_model->disconnectAllSlots(_model->prefix());
}
