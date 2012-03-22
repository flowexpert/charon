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
/** \file   PriorityDialog.cpp
 *  \brief  Implementation of class PriorityDialog
 *  \date   22.03.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

#include "PriorityDialog.h"
#include "PriorityDialog.moc"
#include "ui_PriorityDialog.h"

#include <QDialog>

PriorityDialog::PriorityDialog(QWidget* parent, int selection) :
	QDialog(parent)
{
	_ui = new Ui::PriorityDialog;
	_ui->setupUi(this);

	_selection = selection;
	_ui->priorityBox->setCurrentIndex(selection);
}

PriorityDialog::~PriorityDialog() {
	delete _ui;
}

int PriorityDialog::getPriority(QWidget* parent, int selection) {
	PriorityDialog dlg(parent, selection);
	dlg.exec();
	return dlg._selection;
}

void PriorityDialog::on_setButton_clicked() {
	_selection = _ui->priorityBox->currentIndex();
	accept();
}

void PriorityDialog::on_resetButton_clicked() {
	_selection = 0;
	accept();
}

void PriorityDialog::on_cancelButton_clicked() {
	reject();
}
