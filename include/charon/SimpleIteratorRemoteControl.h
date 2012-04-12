/*  Copyright (C) 2012 The regents of the University of Heidelberg

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License
    as published by the Free Software Foundation, either version 3
    of the License, or (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file SimpleIteratorRemoteControl.h
 *  Declaration of the SimpleIterator's Remote Control.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 06.03.2011
 */

#ifndef _SIMPLEITERATOR_REMOTECONTROL_H
#define _SIMPLEITERATOR_REMOTECONTROL_H

#include <QDialog>

class QLabel;
class QPushButton;
class QProgressBar;

/// gui class to provide step control of iterations
class SimpleIteratorRemoteControl
	: public QDialog
{
	Q_OBJECT
public:
	/// default constructor, setting the dialog caption
	SimpleIteratorRemoteControl(
		QString caption,
		unsigned int maxRuns,
		QWidget* parent = 0);
	/// set current iteration (e.g. for progressbar)
	void setCurrentIteration( unsigned int currentIteration );
public slots:
	/// \name handle button clicks
	//\{
	void on_btnStep_clicked();
	void on_btnBreak_clicked();
	void on_btnContinue_clicked();
	//\}
private:
	/// \name ui element refrences
	//\{
	QLabel *lbl;
	QPushButton *btnStep, *btnBreak, *btnContinue;
	QProgressBar *progress;
	//\}
signals:
	void pbValueChanged( int val );
};

#endif

