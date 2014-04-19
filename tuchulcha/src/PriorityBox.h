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
/** \file   PriorityBox.h
 *  \brief  Declaration of class PriorityBox.
 *  \date   23.03.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

#ifndef PRIORITYBOX_H_
#define PRIORITYBOX_H_

#include <QComboBox>

/// Priority Box..
/** This ComboBox contains four icons for priorities
 *  and optionally corresponding strings.
 */
class PriorityBox : public QComboBox {
	Q_OBJECT

	/// priority value shown/modified in this combo box
	Q_PROPERTY(uint priority READ priority WRITE setPriority USER true)

public:
	/// Default constructor, setting parent widget.
	/** @param  parent    parent widget
	 *  @param  priority  index of preselected value
	 *  @param  strings   if true, descriptions will be added
	 */
	PriorityBox(QWidget* parent = 0, uint priority = 0, bool strings = true);

	/// Get selected priority
	uint priority() const;

	/// Set selected priority
	void setPriority(uint value);
};

#endif /* PRIORITYBOX_H_ */
