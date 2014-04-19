/*  Copyright (C) 2012 David Stronczek

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
/** \file   PriorityDialog.cpp
 *  \brief  Implementation of class PriorityDialog
 *  \date   22.03.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

#include "PriorityDialog.h"
#include "ui_PriorityDialog.h"

#include <QDialog>

PriorityDialog::PriorityDialog(QWidget* pp, int sel) :
	QDialog(pp)
{
	_ui = new Ui::PriorityDialog;
	_ui->setupUi(this);

	_selection = sel;
	_ui->priorityBox->setCurrentIndex(sel);
}

PriorityDialog::~PriorityDialog() {
	delete _ui;
}

int PriorityDialog::selection() const {
	return _selection;
}

void PriorityDialog::accept() {
	_selection = _ui->priorityBox->currentIndex();
	QDialog::accept();
}

void PriorityDialog::on_buttonBox_clicked(QAbstractButton* button) const {
	switch (_ui->buttonBox->buttonRole(button)) {
	case QDialogButtonBox::ResetRole:
		_ui->priorityBox->setCurrentIndex(0);
		break;
	default:
		break;
	}
}
