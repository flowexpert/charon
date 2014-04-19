/*  Copyright (C) 2012 David Stronczek

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
/** \file   CentralMdiArea.h
 *  \brief  Declaration of class CentralMdiArea.
 *  \date   29.03.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

#ifndef CENTRALMDIAREA_H_
#define CENTRALMDIAREA_H_

#include <QMdiArea>

/// Central Mdi Area.
/** This class overwrites QMdiArea and adds drop support
 */
class CentralMdiArea : public QMdiArea {
	Q_OBJECT

public:
	/// Default constructor, setting parent widget.
	/** @param  parent     parent widget
	 */
	CentralMdiArea(QWidget* parent = 0);

signals:
	/// files dropped inside area
	void filesDropped(const QStringList list);

protected:
	/// drag enter event
	virtual void dragEnterEvent(QDragEnterEvent* event);

	/// drage move event
	virtual void dragMoveEvent(QDragMoveEvent* event);

	/// drop event
	virtual void dropEvent(QDropEvent* event);
};

#endif /* CENTRALMDIAREA_H_ */
