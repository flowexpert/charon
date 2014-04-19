/*  Copyright (C) 2012 David Stronczek

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
/** \file   PriorityBox.cpp
 *  \brief  Implementation of class PriorityBox
 *  \date   23.03.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

#include "PriorityBox.h"

#include <QComboBox>

PriorityBox::PriorityBox(QWidget* pp, uint prio, bool strings) :
	QComboBox(pp)
{
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/icons/priorityGrey.png"),
		QSize(), QIcon::Normal, QIcon::Off);
	QIcon icon2;
	icon2.addFile(QString::fromUtf8(":/icons/priorityGreen.png"),
		QSize(), QIcon::Normal, QIcon::Off);
	QIcon icon3;
	icon3.addFile(QString::fromUtf8(":/icons/priorityYellow.png"),
		QSize(), QIcon::Normal, QIcon::Off);
	QIcon icon4;
	icon4.addFile(QString::fromUtf8(":/icons/priorityRed.png"),
		QSize(), QIcon::Normal, QIcon::Off);

	if (strings) {
		addItem(icon1, QString("Standard"));
		addItem(icon2, QString("Low"));
		addItem(icon3, QString("Medium"));
		addItem(icon4, QString("High"));
	} else {
		addItem(icon1, QString());
		addItem(icon2, QString());
		addItem(icon3, QString());
		addItem(icon4, QString());
	}

	setCurrentIndex(prio);
}

uint PriorityBox::priority() const {
	return currentIndex();
}

void PriorityBox::setPriority(uint value) {
	setCurrentIndex(value);
}
