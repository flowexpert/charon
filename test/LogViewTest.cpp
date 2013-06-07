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
/** \file   LogViewTest.cpp
 *  \brief  Implementation of test class LogViewTest
 *  \date   07.06.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "LogViewTest.h"
#include "LogViewProxyModel.h"

#include "QCopyListView.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QStringListModel>
#include <QtTest/QTest>

#ifndef TEST_DIR
#error TEST_DIR not defined
#define TEST_DIR
#endif

void LogViewTest::initTestCase() {
	_viewWidget = new QCopyListView;
	_model = new QStringListModel(_viewWidget);
	LogViewProxyModel* proxy = new LogViewProxyModel(_model);
	proxy->setSourceModel(_model);
	_viewWidget->setModel(proxy);
	_viewWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_viewWidget->setFont(QFont("monospace"));
	_viewWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);

	QFile logFile(TEST_DIR "/testLog.log");
	logFile.open(QFile::ReadOnly | QFile::Text);
	QString logContent = QString::fromUtf8(logFile.readAll());
	QVERIFY(!logContent.isEmpty());
	_model->setStringList(logContent.split("\n"));
	QVERIFY(_model->rowCount() > 0);
}

void LogViewTest::cleanupTestCase() {
	delete _viewWidget; _viewWidget = 0;
}

void LogViewTest::checkData() {
	QAbstractItemModel* model = _viewWidget->model();
	// text line
	QModelIndex idx = model->index(0,0);
	QVERIFY(model->rowCount() > 0);
	QCOMPARE(model->data(idx,Qt::DisplayRole).toString(),QString("line1"));

	// test red background of error line
	idx = model->index(4,0);
	QBrush bg(model->data(idx,Qt::BackgroundRole).value<QBrush>());
	//qDebug() << "Background:" << bg.color().name() << "(should be red)";
	QCOMPARE(model->data(idx,Qt::UserRole).toString(),QString("error"));
	QCOMPARE(bg.color().red(),255);
	QVERIFY(bg.color().green() < 255);
	QVERIFY(bg.color().blue() < 255);

	// same for strane error line
	idx = model->index(5,0);
	bg = model->data(idx,Qt::BackgroundRole).value<QBrush>();
	QCOMPARE(model->data(idx,Qt::UserRole).toString(),QString("error"));
	QCOMPARE(bg.color().red(),255);
	QVERIFY(bg.color().green() < 255);
	QVERIFY(bg.color().blue() < 255);
}

void LogViewTest::checkCopy() {
	// select some error line and copy to clipboard
	QItemSelectionModel* sel = _viewWidget->selectionModel();
	const QAbstractItemModel* model = sel->model();
	sel->setCurrentIndex(model->index(4,0),QItemSelectionModel::SelectCurrent);
	_viewWidget->setSelectionModel(sel);
	QTest::keyClick(_viewWidget,Qt::Key_C,Qt::ControlModifier); // copy
	_viewWidget->clearSelection();

	// check clipboard content
	QString html = QApplication::clipboard()->mimeData()->html();
	QVERIFY(!html.isEmpty());
	QRegExp span("<span class=\".*\" style=\"(.*)\">(.*)</span>");
	QVERIFY(html.contains(span));
	QVERIFY(span.cap(2).startsWith("(EE)"));
	QRegExp bgcol("background-color:(#[0-9a-f]+);");
	QVERIFY(span.cap(1).contains(bgcol));
	//qDebug() << span.cap(1) << span.cap(2) << bgcol.cap(1);
	QColor bg(bgcol.cap(1));
	QCOMPARE(bg.red(),255);
	QVERIFY(bg.green() < 255);
	QVERIFY(bg.blue() < 255);

	/*
	// show the widget
	_viewWidget->show();
	QApplication::exec();
	//*/
}

QTEST_MAIN(LogViewTest)
