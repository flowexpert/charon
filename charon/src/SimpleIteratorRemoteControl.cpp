/*  Copyright (C) 2012 The regents of the University of Heidelberg

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3
    of the License, or (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file SimpleIteratorRemoteControl.cpp
 *  Definition of the SimpleIterator's Remote Control.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 06.03.2011
 */

#include <charon/SimpleIteratorRemoteControl.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>

#include <iostream>

SimpleIteratorRemoteControl::SimpleIteratorRemoteControl(
	QString caption,
	unsigned int maxRuns,
	QWidget* pp) : QDialog(pp)
{
	setWindowTitle(caption);

	QVBoxLayout *windowLayout = new QVBoxLayout(0);
	QHBoxLayout *buttons = new QHBoxLayout(0);
	btnStep = new QPushButton("STEP");
	btnStep->setObjectName("btnStep");
	buttons->addWidget(btnStep);
	btnBreak = new QPushButton("BREAK");
	btnBreak->setObjectName("btnBreak");
	buttons->addWidget(btnBreak);
	btnContinue = new QPushButton("CONTINUE");
	btnContinue->setObjectName("btnContinue");
	buttons->addWidget(btnContinue);
	progress = new QProgressBar();
	progress->setMinimum( 0 );
	progress->setMaximum( maxRuns );

	windowLayout->addWidget( progress );
	windowLayout->addLayout( buttons );

	this->setLayout(windowLayout);

	QMetaObject::connectSlotsByName( this );
	QObject::connect( this, SIGNAL( pbValueChanged(int) ),
	                  progress, SLOT( setValue(int) ) );
}

void SimpleIteratorRemoteControl::on_btnStep_clicked() {
	done(1);
}

void SimpleIteratorRemoteControl::on_btnBreak_clicked() {
	done(2);
}

void SimpleIteratorRemoteControl::on_btnContinue_clicked() {
	done(3);
}

void SimpleIteratorRemoteControl::setCurrentIteration(
	unsigned int currentIteration )
{
	emit pbValueChanged( (int)currentIteration );
}

