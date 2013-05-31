/*  Copyright (C) 2013 Jens-Malte Gottfried

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
/** \file QCopyListView.cpp
 *  \brief Implementation of class QCopyListView
 *
 *  \date 21.05.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#include "QCopyListView.h"

#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>

QCopyListView::QCopyListView(QWidget* pp) :
	QListView(pp)
{
}

QCopyListView::~QCopyListView() {
}

void QCopyListView::keyPressEvent(QKeyEvent* ev) {
	// based on the QT code from AbstractItemView::keyPressEvent
#if !defined(QT_NO_CLIPBOARD) && !defined(QT_NO_SHORTCUT)
	if (ev == QKeySequence::Copy) {
		QStringList lines;
		QModelIndexList selection = selectedIndexes();
		QListIterator<QModelIndex> selIter(selection);
		while (selIter.hasNext()) {
			QModelIndex curIdx = selIter.next();
			QVariant curData = model()->data(curIdx,Qt::DisplayRole);
			if (curData.type() == QVariant::String) {
				lines << curData.toString();
			}
		}
		QApplication::clipboard()->setText(lines.join("\n"));
		ev->accept();
	}
	else {
		QListView::keyPressEvent(ev);
	}
#else
	QListView::keyPressEvent(ev);
#endif
}
