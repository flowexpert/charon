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
/** \file LogViewProxyModel.h
 *  \brief Declaration of class LogViewProxyModel
 *
 *  \date 29.05.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef LOG_VIEW_PROXY_MODEL_H
#define LOG_VIEW_PROXY_MODEL_H

#include <QSortFilterProxyModel>

/// class for highlighting and handling filtering of log output
class LogViewProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	/// constructor
	explicit LogViewProxyModel(QObject* parent=0);
	virtual ~LogViewProxyModel();

	/// handle highlighting
	/** \param index     model index, i.e. row/col info
	 *  \param role      display role like display text, color, icons etc.
	 *  \returns         the data stored under the given role for
	 *                   the item referred to by the index.
	 */
	virtual QVariant data(
		const QModelIndex& index, int role=Qt::DisplayRole) const;
};

#endif // LOG_VIEW_PROXY_MODEL_H
