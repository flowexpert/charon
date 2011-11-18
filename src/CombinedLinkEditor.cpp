/*  Copyright (C) 2011 Jens-Malte Gottfried

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

#include "CombinedLinkEditor.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QtEvents>
#include <QCompleter>
#include <QStringListModel>

#include <QDirModel>

CombinedLinkEditor::CombinedLinkEditor(QWidget* edit, QStringList completerList, QWidget* parent) :
		QLineEdit(parent) {
	setContentsMargins(0, 0, height(), 0);

	_customEdit = edit;
	_customEdit->setParent(this);

	QPalette p = palette();
	p.setColor(QPalette::Base, QColor(127,255,127));
	setPalette(p);

	_toggleButton = new QToolButton(this);
	_toggleButton->setCursor(QCursor(Qt::ArrowCursor));
	_toggleButton->setFocusPolicy(Qt::NoFocus);
	_toggleButton->setCheckable(true);
	
	connect(
		_toggleButton, SIGNAL(toggled(bool)),_customEdit, SLOT(setHidden(bool)));

	QCompleter* complt = new QCompleter(this);

	complt->setModel(new QStringListModel(
			completerList, complt));


	/*
	complt->setModel(new QDirModel(
			QStringList(),
			QDir::AllDirs | QDir::NoDotAndDotDot,
			QDir::Name, complt));
	setCompleter(complt);
	*/

	setCompleter(complt);

}

QWidget* CombinedLinkEditor::getCustomEdit() {
	return _customEdit;
}

void CombinedLinkEditor::resizeEvent(QResizeEvent* e) {
	int w = e->size().width();
	int h = e->size().height();
	_customEdit->setFixedSize(w - h, h);
	_customEdit->move(0, 0);
	_toggleButton->resize(h, h);
	_toggleButton->move(w - h, 0);
}

#include "CombinedLinkEditor.moc"