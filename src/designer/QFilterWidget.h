/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class QToolButton;

#define ICONBUTTON_SIZE 16

/// This is a simple helper class that represents clickable icons

class QIconButton: public QToolButton
{
	Q_OBJECT
	Q_PROPERTY(float fader READ fader WRITE setFader)
public:
	QIconButton(QWidget *parent);
	void paintEvent(QPaintEvent *event);
	float fader() { return m_fader; }
	void setFader(float value) { m_fader = value; update(); }
	void animateShow(bool visible);

private:
	float m_fader;
};

/// FilterWidget: For filtering item views, with reset button
class  QFilterWidget : public QLineEdit
{
	Q_OBJECT
public:
	explicit QFilterWidget(QWidget *parent = 0);
	void resizeEvent(QResizeEvent *);

signals:
	void filterChanged(const QString &);

private slots:
	void checkButton(const QString &text);

private:
	QIconButton *m_button;
	QString m_oldText;
};

QT_END_NAMESPACE

#endif
