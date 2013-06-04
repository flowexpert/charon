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

#include "QFilterWidget.h"

#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtCore/QPropertyAnimation>

QT_BEGIN_NAMESPACE

QIconButton::QIconButton(QWidget *pp) : QToolButton(pp)
{
	setCursor(Qt::ArrowCursor);
}

void QIconButton::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	// Note isDown should really use the active state but in most styles
	// this has no proper feedback
	QIcon::Mode state = QIcon::Disabled;
	if (isEnabled())
		state = isDown() ? QIcon::Selected : QIcon::Normal;
	QPixmap iconpixmap = icon().pixmap(QSize(ICONBUTTON_SIZE, ICONBUTTON_SIZE),
									   state, QIcon::Off);
	QRect pixmapRect = QRect(0, 0, iconpixmap.width(), iconpixmap.height());
	pixmapRect.moveCenter(rect().center());
	painter.setOpacity(m_fader);
	painter.drawPixmap(pixmapRect, iconpixmap);
}

void QIconButton::animateShow(bool visible)
{
	if (visible) {
		QPropertyAnimation *animation = new QPropertyAnimation(this, "fader");
		animation->setDuration(160);
		animation->setEndValue(1.0);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	} else {
		QPropertyAnimation *animation = new QPropertyAnimation(this, "fader");
		animation->setDuration(160);
		animation->setEndValue(0.0);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	}
}

// ------------------- FilterWidget
QFilterWidget::QFilterWidget(QWidget *pp)  :
	QLineEdit(pp),
	m_button(new QIconButton(this))
{
	setPlaceholderText(tr("Filter"));

	// Let the style determine minimum height for our widget
	QSize wsize(ICONBUTTON_SIZE + 6, ICONBUTTON_SIZE + 2);

	// Note KDE does not reserve space for the highlight color
	if (style()->inherits("OxygenStyle")) {
		wsize = wsize.expandedTo(QSize(24, 0));
	}

	// Make room for clear icon
	QMargins margins = textMargins();
	if (layoutDirection() == Qt::LeftToRight)
		margins.setRight(wsize.width());
	else
		margins.setLeft(wsize.width());

	setTextMargins(margins);

	// KDE has custom icons for this. Notice that icon namings are counter intuitive
	// If these icons are not avaiable we use the freedesktop standard name before
	// falling back to a bundled resource
	QIcon icon = QIcon::fromTheme(layoutDirection() == Qt::LeftToRight ?
					 QLatin1String("edit-clear-locationbar-rtl") :
					 QLatin1String("edit-clear-locationbar-ltr"),
					 QIcon::fromTheme("edit-clear", QIcon(":/icons/edit-clear.png")));

	m_button->setIcon(icon);
	m_button->setToolTip(tr("Clear text"));
	connect(m_button, SIGNAL(clicked()), this, SLOT(clear()));
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(checkButton(QString)));
	connect(this, SIGNAL(textEdited(QString)), this, SIGNAL(filterChanged(QString)));
}

void QFilterWidget::checkButton(const QString &txt)
{
	if (m_oldText.isEmpty() || txt.isEmpty())
		m_button->animateShow(!text().isEmpty());
	m_oldText = txt;
}

void QFilterWidget::resizeEvent(QResizeEvent *)
{
	QRect contentRect = rect();
	if (layoutDirection() == Qt::LeftToRight) {
		const int iconoffset = textMargins().right() + 4;
		m_button->setGeometry(contentRect.adjusted(width() - iconoffset, 0, 0, 0));
	} else {
		const int iconoffset = textMargins().left() + 4;
		m_button->setGeometry(contentRect.adjusted(0, 0, -width() + iconoffset, 0));
	}
}

QT_END_NAMESPACE
