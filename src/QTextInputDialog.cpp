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
/** \file   QTextInputDialog.h
 *  \brief  Declaration of class QTextInputDialog
 *  \date   04.07.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "QTextInputDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>

QTextInputDialog::QTextInputDialog(QWidget *pp, Qt::WindowFlags flags) :
		QDialog(pp,flags)
{
	QVBoxLayout *verticalLayout;
	QDialogButtonBox *bBox;

	verticalLayout = new QVBoxLayout(this);
	lText = new QLabel(this);
	lText->setObjectName(QString::fromUtf8("lText"));
	verticalLayout->addWidget(lText);

	eText = new QLineEdit(this);
	eText->setObjectName(QString::fromUtf8("eText"));
	verticalLayout->addWidget(eText);

	bBox = new QDialogButtonBox(this);
	bBox->setObjectName(QString::fromUtf8("bBox"));
	bBox->setOrientation(Qt::Horizontal);
	bBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
	verticalLayout->addWidget(bBox);

	connect(bBox, SIGNAL(accepted()), SLOT(accept()));
	connect(bBox, SIGNAL(rejected()), SLOT(reject()));
	connect(eText, SIGNAL(textChanged(QString)), SIGNAL(textValueChanged(QString)));

	QMetaObject::connectSlotsByName(this);
}

QTextInputDialog::~QTextInputDialog() {
}

QString QTextInputDialog::getText(
		QWidget *parent, const QString &title,
		const QString &label, QLineEdit::EchoMode mode,
		const QString &text, bool *ok, Qt::WindowFlags flags,
		Qt::InputMethodHints inputMethodHints,
		QValidator* val) {
	QTextInputDialog dialog(parent, flags);
	dialog.setWindowTitle(title);
	dialog.setLabelText(label);
	dialog.setTextValue(text);
	dialog.setTextEchoMode(mode);
	dialog.setInputMethodHints(inputMethodHints);
	dialog.setValidator(val);

	int ret = dialog.exec();
	if (ok)
		*ok = !!ret;
	if (ret) {
		return dialog.textValue();
	} else {
		return QString();
	}
}

void QTextInputDialog::done (int res) {
	QDialog::done(res);
	if (res) {
		emit textValueSelected(textValue());
	}
}

void QTextInputDialog::setTextEchoMode(QLineEdit::EchoMode mode) {
	eText->setEchoMode(mode);
}

void QTextInputDialog::setLabelText(const QString& text) {
	lText->setText(text);
}

void QTextInputDialog::setTextValue(const QString& value) {
	eText->setText(value);
}

QString QTextInputDialog::textValue() const {
	return eText->text();
}

QString QTextInputDialog::labelText() const {
	return lText->text();
}

QLineEdit::EchoMode QTextInputDialog::textEchoMode() const {
	return eText->echoMode();
}

void QTextInputDialog::setValidator(const QValidator* val) {
	eText->setValidator(val);
}

const QValidator* QTextInputDialog::validator() const {
	return eText->validator();
}
