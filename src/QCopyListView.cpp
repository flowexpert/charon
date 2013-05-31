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
		QStringList linesT,linesH;
		QModelIndexList selection = selectedIndexes();
		QListIterator<QModelIndex> selIter(selection);
		while (selIter.hasNext()) {
			QModelIndex curIdx = selIter.next();
			QVariant curData; QString colStyle;
			curData = model()->data(curIdx,Qt::ForegroundRole);
			if (curData.type() == QVariant::Brush) {
				QBrush brush = qvariant_cast<QBrush>(curData);
				QString color = brush.color().name();
				if (!color.isEmpty()) {
					colStyle += QString("color:%1;").arg(color);
				}
			}
			curData = model()->data(curIdx,Qt::BackgroundRole);
			if (curData.type() == QVariant::Brush) {
				QBrush brush = qvariant_cast<QBrush>(curData);
				QString color = brush.color().name();
				if (!color.isEmpty()) {
					colStyle += QString("background-color:%1;").arg(color);
				}
			}
			curData = model()->data(curIdx,Qt::DisplayRole);
			if (curData.type() == QVariant::String) {
				QString curLine = curData.toString();
				linesT << curLine;
				if (colStyle.isEmpty()) {
					linesH << curLine;
				}
				else {
					linesH << QString("<span style=\"%1\">%2</span>")
						.arg(colStyle).arg(curLine);
				}
			}
		}
		/*
		// analyze old conent
		const QMimeData* oldContent = QApplication::clipboard()->mimeData();
		qDebug("Formats: %s\n",
			oldContent->formats().join(",").toLocal8Bit().constData());
		qDebug("Html content: %s\n",
			oldContent->html().toLocal8Bit().constData());
		QVariant tmp = oldContent->data("text/html");
		qDebug("moz_html: %s\n", tmp.toString().toLocal8Bit().constData());
		*/

		QMimeData* clipData = new QMimeData;
		clipData->setText(linesT.join("\n"));
		clipData->setHtml(linesH.join("<br>"));
		QApplication::clipboard()->setMimeData(clipData);
		ev->accept();
	}
	else {
		QListView::keyPressEvent(ev);
	}
#else
	QListView::keyPressEvent(ev);
#endif
}
