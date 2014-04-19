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
/** \file   QFilterWidget.h
 *  \brief  Declaration of class QFilterWidget
 *  \date   05.05.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *
 *  The classes in this file are based on Qt4 code (from Qt Designer)
 *  which is also published as open source code licensed under the
 *  conditions of the LGPLv2.1. See Qt4Copyright.txt for more infos.
 */

#ifndef QFILTERWIDGET_H
#define QFILTERWIDGET_H

#include <QLineEdit>
#include <QToolButton>

QT_BEGIN_NAMESPACE

class QToolButton;

/// size of the reset button icon
#define ICONBUTTON_SIZE 16

/// This is a simple helper class that represents clickable icons
class QIconButton: public QToolButton
{
	Q_OBJECT
	/// fade property, controls icon transparency
	Q_PROPERTY(float fader READ fader WRITE setFader)
public:
	/// default constructor
	QIconButton(QWidget* parent /** parent widget */);
	/// handle fade parameter
	void paintEvent(QPaintEvent* event /** paint event */);
	/// get fade value
	float fader() { return m_fader; }
	/// set fade value
	void setFader(float value /** new value */) { m_fader = value; update(); }
	/// fade in or out
	void animateShow(bool visible /** fade in if true */);

private:
	/// fading state
	float m_fader;
};

/// FilterWidget: For filtering item views, with reset button
class  QFilterWidget : public QLineEdit
{
	Q_OBJECT
public:
	/// default constructor
	explicit QFilterWidget(QWidget* parent = 0 /** parent widget */);
	/// handle resize
	/** adapts icon button position */
	void resizeEvent(QResizeEvent* event /** resize event */);

private slots:
	/// check for fade in/out of the icon button
	void checkButton(const QString& text);

private:
	QIconButton*_resetButton; ///< reset button
	QString _oldText;         ///< text cache (for checkButton(QString))
};

QT_END_NAMESPACE

#endif
