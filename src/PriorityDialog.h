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
/** \file   PriorityDialog.h
 *  \brief  Declaration of class PriorityDialog.
 *  \date   22.03.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

#ifndef PRIORITYDIALOG_H_
#define PRIORITYDIALOG_H_

#include <QDialog>

namespace Ui {
	class PriorityDialog;
}

/// Priority Dialog.
/** This dialog contains a ComboBox and buttons to change the
 *  priority of an parameter from the ObjectInspector.
 */
class PriorityDialog : public QDialog {
	Q_OBJECT

public:
	/// Default constructor, setting parent widget.
	/** @param  parent     parent widget
	 *  @param  selection  index of preselected value
	 */
	PriorityDialog(QWidget* parent = 0, int selection = 0);

	/// default destructor
	virtual ~PriorityDialog();

	/// Get selected index
	int selection() const;

public slots:
	/// close and update selection
	void on_setButton_clicked();

	/// close and set selection to 0
	void on_resetButton_clicked();

	/// close and don't change selection
	void on_cancelButton_clicked();

protected:
	/// designer gui
	Ui::PriorityDialog* _ui;

private:
	/// var to store selection
	int _selection;
};

#endif /* PRIORITYDIALOG_H_ */
