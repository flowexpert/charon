/*  Copyright (C) 2013 Jens-Malte Gottfried

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
/** \file   QTextInputDialog.h
 *  \brief  Declaration of class QTextInputDialog
 *  \date   04.07.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef QTEXTINPUTDIALOG_H
#define QTEXTINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>

class QLabel;

/// dialog to query a string variable, similar to QInputDialog
/** but with ability to set a validator to the line edit */
class QTextInputDialog : public QDialog
{
	Q_OBJECT
	/// text displayed to the user
	Q_PROPERTY(QString labelText READ labelText WRITE setLabelText)
	/// text entered by the user
	Q_PROPERTY(QString textValue READ textValue WRITE setTextValue NOTIFY textValueChanged)
	/// text echo mode of the input line edit
	Q_PROPERTY(QLineEdit::EchoMode textEchoMode READ textEchoMode WRITE setTextEchoMode)

public:
	/// default constructor
	/** \param parent  parent widget
	 *  \param flags   window flags
	 */
	explicit QTextInputDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~QTextInputDialog();

	/// get text
	/** copy of QInputDialog::getText interface
	 *  \param parent  parent widget
	 *  \param title   dialog window title
	 *  \param label   label for text input
	 *  \param mode    text input mode
	 *  \param text    input text content
	 *  \param ok      true if ok has been pressed, false if canceled
	 *  \param flags   window flags
	 *  \param iHints  input method hints
	 *  \param val     validator
	 *  \returns       entered text or empty string if canceled
	 */
	static QString getText (
			QWidget * parent, const QString & title,
			const QString & label,
			QLineEdit::EchoMode mode = QLineEdit::Normal,
			const QString & text = QString(),
			bool * ok = 0,
			Qt::WindowFlags flags = 0,
			Qt::InputMethodHints iHints = Qt::ImhNone,
			QValidator* val = 0);

	///\name property setter/getter
	///\{
	QString labelText() const;                  ///< label text
	void setLabelText(const QString&);          ///< set label text
	QString textValue() const;                  ///< line edit text
	void setTextValue(const QString&);          ///< set line edit text
	QLineEdit::EchoMode textEchoMode() const;   ///< text echo mode
	void setTextEchoMode(QLineEdit::EchoMode);  ///< set text echo mode
	const QValidator* validator() const;        ///< text validator
	void setValidator(const QValidator*);       ///< set validator
	///\}

signals:
	void textValueChanged(const QString&);      ///< changed text
	void textValueSelected(const QString &);    ///< text selected

protected:
	QLabel *lText;                              ///< text label
	QLineEdit *eText;                           ///< text line edit
	virtual void done (int result);             ///< dialog done
};

#endif // QTEXTINPUTDIALOG_H
