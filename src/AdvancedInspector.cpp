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

/** @file   AdvancedInspector.cpp
 *  @brief  Implementation of class AdvancedInspector
 *  @date   17.10.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "AdvancedInspector.h"

#include <QtGui>
#include "ParameterFileModel.h"
#include "MetaData.h"

#include "AdvancedInspector.moc"
#include "ui_ObjectInspector.h"

AdvancedInspector::AdvancedInspector(QWidget* myParent,
		ParameterFileModel* newModel) :
		ObjectInspector(myParent, 0) {
	init();
	setModel(newModel);
	setEdit(true);
}

AdvancedInspector::~AdvancedInspector() {
}

void AdvancedInspector::init() {
	// init edit widget
	_advancedFrame  = new QFrame();
	_advancedFrame->setFrameStyle(QFrame::Box | QFrame::Sunken);
	_ui->verticalLayout->addWidget(_advancedFrame);

	QGridLayout* editLayout = new QGridLayout(_advancedFrame);
	QLabel* label2  = new QLabel(tr("Editing"));
	_addButton      = new QPushButton(tr("&Add"));
	_delButton      = new QPushButton(tr("&Delete"));
	_clearButton    = new QPushButton(tr("&Clear"));
	_useMetadata    = new QCheckBox(tr("use &metadata"));
	_onlyparams     = new QCheckBox(tr("show &editable parameters only"));
	_useMetadata->setEnabled(false);
	_onlyparams->setEnabled(false);
	connect(_useMetadata, SIGNAL(toggled(bool)),
		_onlyparams, SLOT(setEnabled(bool)));

	connect(_addButton, SIGNAL(clicked()), this, SLOT(addParam()));
	connect(_delButton, SIGNAL(clicked()), this, SLOT(delParam()));
	connect(_clearButton, SIGNAL(clicked()), this, SLOT(clear()));
	editLayout->addWidget(label2,       0, 0, 1, -1);
	editLayout->addWidget(_addButton,   1, 0);
	editLayout->addWidget(_delButton,   1, 1);
	editLayout->addWidget(_clearButton, 1, 2);
	editLayout->addWidget(_useMetadata, 2, 0, 1, -1);
	editLayout->addWidget(_onlyparams,  3, 0, 1, -1);
}

void AdvancedInspector::setModel(ParameterFileModel* newModel) {
	if(newModel == model())
		return;

	// disconnect old connections
	if(model()) {
		disconnect(model(), 0, 0, 0);
		disconnect(_useMetadata, 0, model(), 0);
		disconnect(_onlyparams,  0, model(), 0);
	}

	// set up objectInspector model
	ObjectInspector::setModel(newModel);

	Q_ASSERT(model());

	// update values
	_useMetadata->setEnabled(model()->metaInfo());
	_useMetadata->setChecked(model()->useMetaInfo());
	_onlyparams->setChecked(model()->onlyParams());

	// connect additional widgets
	connect(model(), SIGNAL(metaInfoChanged(bool)),
		_useMetadata, SLOT(setEnabled(bool)));
	connect(_useMetadata, SIGNAL(clicked(bool)),
		model(), SLOT(setUseMetaInfo(bool)));
	connect(model(), SIGNAL(useMetaInfoChanged(bool)),
		_useMetadata, SLOT(setChecked(bool)));
	connect(_onlyparams, SIGNAL(clicked(bool)),
		model(), SLOT(setOnlyParams(bool)));
	connect(model(), SIGNAL(onlyParamsChanged(bool)),
		_onlyparams, SLOT(setChecked(bool)));
}

void AdvancedInspector::setEdit(bool on) {
	ObjectInspector::setEdit(on);
	_advancedFrame->setVisible(on);
}
