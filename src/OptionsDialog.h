/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file   OptionsDialog.h
 *  \brief  Declaration of class OptionsDialog
 *
 *  \date   15.11.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui {class OptionsDialog;}
class QAbstractButton;
class QTableWidgetItem;

/// option dialog widget
class OptionsDialog : public QDialog
{
	Q_OBJECT
public:
	/// constructor
	/** \param parent   parent widget
	 *  \param f        widget flags */
	explicit OptionsDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
	virtual ~OptionsDialog();

	/// Configures plugin paths
	/** \returns true if errors have been found */
	static bool check();

signals:
	/// request help on specified page
	void helpRequested(QString);

public slots:
	/// refresh from saved settings
	void refresh();
	/// apply new settings values
	void apply();
	/// restore default values
	void restore();

private slots:
	/// handle button click events
	void on_bBox_clicked(QAbstractButton* button);
	/// handle help request
	void on_bBox_helpRequested();
	/// handle exclude list changes
	void on_tableExclude_itemChanged(QTableWidgetItem* item) const;
	/// set exclude list
	void _setExcludes(QStringList list) const;

private:
	/// designer gui
	Ui::OptionsDialog* _ui;
	/// get exclude list
	QStringList _getExcludes() const;
};

#endif // OPTIONSDIALOG_H
