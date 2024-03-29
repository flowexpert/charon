/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file   QDirEdit.cpp
 *  @brief  Implementation of class DirEdit.
 *  @date   15.04.2010
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "QDirEdit.h"
#include <QCompleter>
#include <QDirModel>
#include <QFileDialog>
#include <QToolButton>
#include <QtEvents>

void QDirEdit::_init() {
	// initial values
	_files = false;
	_writeFile = false;

	// tool button to open browse dialog
	_browseButton = new QToolButton(this);
	_browseButton->setText(tr("..."));
	_browseButton->setCursor(QCursor(Qt::ArrowCursor));
	_browseButton->setFocusPolicy(Qt::NoFocus);
	QObject::connect(
			_browseButton, SIGNAL(clicked()),this, SLOT(fileDialog()));

	// completer
	QCompleter* complt = new QCompleter(this);
	complt->setModel(new QDirModel(
			QStringList(),
			QDir::AllDirs | QDir::NoDotAndDotDot,
			QDir::Name, complt));
	setCompleter(complt);
}

QDirEdit::QDirEdit(QWidget* p) :
		QLineEdit(p) {
	_init();
}

QDirEdit::QDirEdit(const QString& content, QWidget* p) :
		QLineEdit(content, p) {
	_init();
}

void QDirEdit::fileDialog() {
	emit dialogOpen(true);
	QString newVal;
	if (_files) {
		if (_writeFile)
			newVal = QFileDialog::getSaveFileName(
					this,tr("select file to save to"), text());
		else {
			QStringList list = QFileDialog::getOpenFileNames(
				this, tr("select one or more file(s) to open"), text());
			if (list.size() > 0) {
				newVal = list.join(";");
			}
		}
	}
	else
		newVal = QFileDialog::getExistingDirectory(
				this, tr("select directory"),text());
	emit dialogOpen(false);
	if(!newVal.isEmpty())
	setText(newVal);
}

void QDirEdit::acceptFiles(bool files, bool write) {
	_files = files;
	_writeFile = write;
	QDirModel* dirModel = qobject_cast<QDirModel*>(completer()->model());
	Q_ASSERT(dirModel);
	if (_files)
		dirModel->setFilter(
				QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
	else
		dirModel->setFilter(
				QDir::AllDirs               | QDir::NoDotAndDotDot);
}

void QDirEdit::resizeEvent(QResizeEvent* e) {
	// make the tool button fit on the right side
	int h = e->size().height();
	// move the line edit to make room for the tool button
	setContentsMargins(0, 0, h, 0);
	// put the tool button in its place
	_browseButton->resize(h, h);
	_browseButton->move(width() - h, 0);
}
