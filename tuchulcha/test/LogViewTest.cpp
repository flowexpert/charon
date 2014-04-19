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
#include <QFile>
#include <QDebug>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QStringListModel>
#include <QTest>

#ifndef TEST_DIR
#error TEST_DIR not defined
#define TEST_DIR
#endif

void LogViewTest::initTestCase() {
	_viewWidget = new QCopyListView;
	_model = new QStringListModel(_viewWidget);
	_filter = new LogViewProxyModel(_model);
	_filter->setSourceModel(_model);
	_viewWidget->setModel(_filter);
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
	/*
	// show the widget
	_viewWidget->show();
	QApplication::exec();
	//*/

	delete _viewWidget; _viewWidget = 0;
}

void LogViewTest::cleanup() {
	_viewWidget->clearSelection();
	_filter->setFilterRegExp(QString());
	QApplication::clipboard()->setMimeData(new QMimeData());
}

void LogViewTest::checkData() {
	QAbstractItemModel* model = _viewWidget->model();
	// text line
	QVERIFY(model->rowCount() > 0);
	QModelIndex idx = model->index(0,0);
	QCOMPARE(model->data(idx,Qt::DisplayRole).toString(),QString("line1"));

	// debug line
	idx = model->index(1,0);
	QVERIFY(model->data(idx,Qt::DisplayRole).toString().startsWith("(DD)"));
	QVERIFY(model->data(idx,Qt::UserRole).isValid());
	QCOMPARE(model->data(idx,Qt::UserRole).toString(),QString("debug"));
	QVERIFY(model->data(idx,Qt::ForegroundRole).isValid());
	QBrush fg(model->data(idx,Qt::ForegroundRole).value<QBrush>());
	QVERIFY(fg.color().red()   < 180); QVERIFY(fg.color().red()   > 150);
	QVERIFY(fg.color().green() < 180); QVERIFY(fg.color().green() > 150);
	QVERIFY(fg.color().blue()  < 180); QVERIFY(fg.color().blue()  > 150);

	// test red background of error line
	idx = model->index(4,0);
	QVERIFY(model->data(idx,Qt::BackgroundRole).isValid());
	QBrush bg(model->data(idx,Qt::BackgroundRole).value<QBrush>());
	//qDebug() << "Background:" << bg.color().name() << "(should be red)";
	QVERIFY(model->data(idx,Qt::UserRole).isValid());
	QCOMPARE(model->data(idx,Qt::UserRole).toString(),QString("error"));
	QCOMPARE(bg.color().red(),255);
	QVERIFY(bg.color().green() < 255);
	QVERIFY(bg.color().blue() < 255);

	// same for strange error line
	idx = model->index(5,0);
	QVERIFY(model->data(idx,Qt::BackgroundRole).isValid());
	bg = model->data(idx,Qt::BackgroundRole).value<QBrush>();
	QVERIFY(model->data(idx,Qt::UserRole).isValid());
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
}

void LogViewTest::checkFilter() {
	_filter->setFilterRegExp(LogViewProxyModel::debugFilterRegex());

	QAbstractItemModel* model = _viewWidget->model();
	QVERIFY(model->rowCount() > 0);

	// check filtered out debug line
	QModelIndex idx = model->index(1,0);
	QVERIFY(!model->data(idx,Qt::DisplayRole).toString().startsWith("(DD)"));

	// test gray foreground of info line
	idx = model->index(1,0);
	QVERIFY(model->data(idx,Qt::UserRole).isValid());
	QCOMPARE(model->data(idx,Qt::UserRole).toString(),QString("info"));
	QVERIFY(model->data(idx,Qt::ForegroundRole).isValid());
	QBrush fg(model->data(idx,Qt::ForegroundRole).value<QBrush>());
	QVERIFY(fg.color().red()   < 150); QVERIFY(fg.color().red()   > 80);
	QVERIFY(fg.color().green() < 150); QVERIFY(fg.color().green() > 80);
	QVERIFY(fg.color().blue()  < 150); QVERIFY(fg.color().blue()  > 80);

	// test red background of error line
	idx = model->index(3,0);
	QVERIFY(model->data(idx,Qt::UserRole).isValid());
	QCOMPARE(model->data(idx,Qt::UserRole).toString(),QString("error"));
	QVERIFY(model->data(idx,Qt::BackgroundRole).isValid());
	QBrush bg(model->data(idx,Qt::BackgroundRole).value<QBrush>());
	//qDebug() << "Background:" << bg.color().name() << "(should be red)";
	QCOMPARE(bg.color().red(),255);
	QVERIFY(bg.color().green() < 255);
	QVERIFY(bg.color().blue() < 255);
}

QTEST_MAIN(LogViewTest)
