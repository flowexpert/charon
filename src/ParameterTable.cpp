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
/** \file   ParameterTable.cpp
 *  \brief  Implementation of class ParameterTable.
 *  \date   30.03.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

#include "ParameterTable.h"
#include "ParameterTable.moc"

#include <QTableView>
#include <QtGUI>

ParameterTable::ParameterTable(QWidget* parent) : QTableView(parent) {
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
}

void ParameterTable::dragEnterEvent(QDragEnterEvent* e) {
	e->setDropAction(Qt::CopyAction);
	e->accept();
}

void ParameterTable::dragMoveEvent(QDragMoveEvent* e) {
	e->setDropAction(Qt::CopyAction);
	e->accept();
}

void ParameterTable::dropEvent(QDropEvent* e) {
	const QMimeData *mimeData = e->mimeData();

	QString result;
	if (mimeData->hasText()) {
		result = mimeData->text();
	} else if (mimeData->hasHtml()) {
		result = mimeData->html();
	} else if (mimeData->hasUrls()) {
		QList<QUrl> urlList = mimeData->urls();
		if (urlList.size() >= 1) {
			result = urlList.at(0).path().mid(1);
		}
	}
	e->setDropAction(Qt::CopyAction);
	e->accept();

	if (!result.isNull()) {
		emit contentDropped(indexAt(e->pos()), result);
	}
}
