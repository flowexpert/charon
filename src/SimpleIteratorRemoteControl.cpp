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
/** \file SimpleIteratorRemoteControl.cpp
 *  Definition of the SimpleIterator's Remote Control.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 06.03.2011
 */

#include <charon/SimpleIteratorRemoteControl.h>

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

SimpleIteratorRemoteControl::SimpleIteratorRemoteControl(
		QString caption, QWidget* pp) : QDialog(pp)
{
	setWindowTitle(caption);
	QHBoxLayout* layout = new QHBoxLayout(this);
	btnStep = new QPushButton("STEP");
	btnStep->setObjectName("btnStep");
	layout->addWidget(btnStep);
	btnBreak = new QPushButton("BREAK");
	btnBreak->setObjectName("btnBreak");
	layout->addWidget(btnBreak);
	btnContinue = new QPushButton("CONTINUE");
	btnContinue->setObjectName("btnContinue");
	layout->addWidget(btnContinue);

	setLayout( layout );

	QMetaObject::connectSlotsByName( this );
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
