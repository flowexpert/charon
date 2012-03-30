/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/** \file   ParameterTable.h
 *  \brief  Declaration of class ParameterTable.
 *  \date   30.03.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

#ifndef PARAMETERTABLE_H_
#define PARAMETERTABLE_H_

#include <QTableView>

/// Parameter Table class
/** This class overwrites QTableView and adds drop support
 */
class ParameterTable : public QTableView {
	Q_OBJECT

public:
	/// Default constructor, setting parent widget.
	/** @param  parent     parent widget
	 */
	ParameterTable(QWidget* parent = 0);

signals:
	/// files dropped inside area
	void contentDropped(const QModelIndex &index, const QString &content);

protected:
	/// drag enter event
	virtual void dragEnterEvent(QDragEnterEvent* event);

	/// drage move event
	virtual void dragMoveEvent(QDragMoveEvent* event);

	/// drop event
	virtual void dropEvent(QDropEvent* event);
};

#endif /* PARAMETERTABLE_H_ */
