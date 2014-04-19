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
/** \file LogViewProxyModel.cpp
 *  \brief Implementation of class LogViewProxyModel
 *
 *  \date 29.05.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "LogViewProxyModel.h"

#include <QBrush>
#include <QColor>

LogViewProxyModel::LogViewProxyModel(QObject* pp) :
		QSortFilterProxyModel(pp) {
}

LogViewProxyModel::~LogViewProxyModel() {
}

QVariant LogViewProxyModel::data(const QModelIndex& idx, int rr) const {
	QRegExp logLevel("\\((\\w+)\\).*");
	switch(rr) {
	case Qt::UserRole:
		if (logLevel.exactMatch(
				QSortFilterProxyModel::data(idx,Qt::DisplayRole).toString())) {
			QString level = logLevel.cap(1);
			if (level == "DD") {
				return QString("debug");
			}
			else if (level == "II") {
				return QString("info");
			}
			else if (level == "WW") {
				return QString("warning");
			}
			else if (level == "EE") {
				return QString("error");
			}
			else {
				return QString("normal");
			}
		}
		break;
	case Qt::ForegroundRole:
		if (logLevel.exactMatch(
				QSortFilterProxyModel::data(idx,Qt::DisplayRole).toString())) {
			QString level = logLevel.cap(1);
			if (level == "DD") {
				return QBrush(QColor("#AAA")); // light gray
			}
			else if (level == "II") {
				return QBrush(QColor("#777")); // very dark gray
			}
			else if (level == "WW") {
				return QBrush(QColor("#F70")); // orange
			}
		}
		break;
	case Qt::BackgroundRole:
		if (logLevel.exactMatch(
				QSortFilterProxyModel::data(idx,Qt::DisplayRole).toString())) {
			QString level = logLevel.cap(1);
			if (level == "EE") {
				return QBrush(QColor("#F87")); // lighter red
			}
		}
		break;
	default:
		break;
	}

	return QSortFilterProxyModel::data(idx,rr);
}

QRegExp LogViewProxyModel::debugFilterRegex() {
	return QRegExp("^\\((?!DD)|^(?!\\()");
}
