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
/** \file   QFilterWidget.cpp
 *  \brief  Implementation of class QFilterWidget
 *  \date   05.05.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *
 *  The classes in this file are based on Qt4 code (from Qt Designer)
 *  which is also published as open source code licensed under the
 *  conditions of the LGPLv2.1. See Qt4Copyright.txt for more infos.
 */

#include "QFilterWidget.h"

#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtCore/QPropertyAnimation>

QT_BEGIN_NAMESPACE

QIconButton::QIconButton(QWidget *pp) : QToolButton(pp) {
	setCursor(Qt::ArrowCursor);
}

void QIconButton::paintEvent(QPaintEvent*) {
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
	_resetButton(new QIconButton(this))
{
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

	_resetButton->setIcon(icon);
	_resetButton->setToolTip(tr("Clear text"));
	connect(_resetButton, SIGNAL(clicked()), this, SLOT(clear()));
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(checkButton(QString)));
}

void QFilterWidget::checkButton(const QString &txt)
{
	if (_oldText.isEmpty() || txt.isEmpty())
		_resetButton->animateShow(!text().isEmpty());
	_oldText = txt;
}

void QFilterWidget::resizeEvent(QResizeEvent *)
{
	QRect contentRect = rect();
	if (layoutDirection() == Qt::LeftToRight) {
		const int iconoffset = textMargins().right() + 4;
		_resetButton->setGeometry(contentRect.adjusted(width() - iconoffset, 0, 0, 0));
	} else {
		const int iconoffset = textMargins().left() + 4;
		_resetButton->setGeometry(contentRect.adjusted(0, 0, -width() + iconoffset, 0));
	}
}

QT_END_NAMESPACE
