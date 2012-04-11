/*  Copyright (C) 2012 David Stronczek

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
/** \file   CentralMdiArea.cpp
 *  \brief  Implementation of class CentralMdiArea.
 *  \date   29.03.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

#include "CentralMdiArea.h"
#include "CentralMdiArea.moc"

#include <QMdiArea>
#include <QtGui>

CentralMdiArea::CentralMdiArea(QWidget* parent) : QMdiArea(parent) {
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
}

void CentralMdiArea::dragEnterEvent(QDragEnterEvent* e) {
	e->acceptProposedAction();
}

void CentralMdiArea::dragMoveEvent(QDragMoveEvent* e) {
	e->acceptProposedAction();
}

void CentralMdiArea::dropEvent(QDropEvent* e) {
	const QMimeData *mimeData = e->mimeData();

	QStringList files;

	if (mimeData->hasUrls()) {
		QList<QUrl> urlList = mimeData->urls();
		for (int i = 0; i < urlList.size(); i++) {
			QString url = urlList.at(i).path();
			files << url.mid(1);
		}
	}
	e->acceptProposedAction();

	if (!files.isEmpty()) {
		emit filesDropped(files);
	}
}
